/*
 * RAFAELIA Runtime Hook
 */

#include "qemu/osdep.h"
#include <inttypes.h>
#include <math.h>
#include "qemu/error-report.h"
#include "qemu/main-loop.h"
#include "qemu/notify.h"
#include "qemu/timer.h"
#include "crypto/hash.h"
#include "system/runstate.h"
#include "hw/core/rafaelia-core.h"
#include "hw/core/rafaelia-integration.h"
#include "hw/core/rafaelia-runtime.h"
#include "trace.h"

#define RAFAELIA_RUNTIME_TICK_CAP 100u
#define RAFAELIA_RUNTIME_TICK_HISTORY 128u
#define RAFAELIA_RUNTIME_OVERHEAD_CAP_US 2000u
#define RAFAELIA_RUNTIME_OVERHEAD_BACKOFF_MAX 6u
#define RAFAELIA_RUNTIME_ADAPTIVE_SCALE_MAX 8u
#define RAFAELIA_RUNTIME_ADAPTIVE_STABLE_TICKS 5u
#define RAFAELIA_RUNTIME_ENTROPY_DELTA_STABLE 0.0025
#define RAFAELIA_RUNTIME_COHERENCE_DELTA_STABLE 0.0015
#define RAFAELIA_RUNTIME_EWMA_ALPHA 0.2

typedef struct rafaelia_runtime_state {
    rafaelia_runtime_config_t config;
    rafaelia_core_t core;
    rafaelia_integration_hub_t hub;
    bool hub_initialized;
    bool initialized;
    bool running;
    uint64_t ticks_total;
    uint64_t tick_ns_total;
    uint64_t last_tick_duration_ns;
    uint64_t tick_ns_history[RAFAELIA_RUNTIME_TICK_HISTORY];
    size_t tick_ns_index;
    size_t tick_ns_count;
    unsigned int overhead_backoff;
    unsigned int overhead_throttle_count;
    double tick_avg_us;
    double tick_p95_us;
    double entropy_prev;
    double coherence_prev;
    double entropy_ewma;
    double coherence_ewma;
    double score_ec;
    unsigned int adaptive_scale;
    unsigned int adaptive_stable_ticks;
    double entropy_last;
    double coherence_last;
    int64_t last_tick_ms;
    RunState last_runstate;
    Notifier main_loop_notifier;
    Notifier shutdown_notifier;
    VMChangeStateEntry *vm_state_entry;
} rafaelia_runtime_state_t;

static rafaelia_runtime_state_t rafaelia_runtime_state;

static int rafaelia_runtime_compare_u64(const void *a, const void *b)
{
    uint64_t lhs = *(const uint64_t *)a;
    uint64_t rhs = *(const uint64_t *)b;

    if (lhs < rhs) {
        return -1;
    }
    if (lhs > rhs) {
        return 1;
    }
    return 0;
}

rafaelia_runtime_config_t rafaelia_runtime_config_default(void)
{
    return (rafaelia_runtime_config_t){
        .enabled = false,
        .debug = false,
        .tick_ms = 100,
        .mode = RAFAELIA_RUNTIME_MODE_SILENT,
    };
}

const char *rafaelia_runtime_mode_name(rafaelia_runtime_mode_t mode)
{
    switch (mode) {
    case RAFAELIA_RUNTIME_MODE_SILENT:
        return "silent";
    case RAFAELIA_RUNTIME_MODE_LOG:
        return "log";
    case RAFAELIA_RUNTIME_MODE_TRACE:
        return "trace";
    case RAFAELIA_RUNTIME_MODE_SYMBIOSIS:
        return "symbiosis";
    case RAFAELIA_RUNTIME_MODE_AUDIT:
        return "audit";
    case RAFAELIA_RUNTIME_MODE_BENCH:
        return "bench";
    default:
        return "unknown";
    }
}

bool rafaelia_runtime_parse_mode(const char *mode_str,
                                 rafaelia_runtime_mode_t *mode)
{
    if (!mode_str || !mode) {
        return false;
    }

    if (g_ascii_strcasecmp(mode_str, "silent") == 0) {
        *mode = RAFAELIA_RUNTIME_MODE_SILENT;
        return true;
    }
    if (g_ascii_strcasecmp(mode_str, "log") == 0) {
        *mode = RAFAELIA_RUNTIME_MODE_LOG;
        return true;
    }
    if (g_ascii_strcasecmp(mode_str, "trace") == 0) {
        *mode = RAFAELIA_RUNTIME_MODE_TRACE;
        return true;
    }
    if (g_ascii_strcasecmp(mode_str, "symbiosis") == 0) {
        *mode = RAFAELIA_RUNTIME_MODE_SYMBIOSIS;
        return true;
    }
    if (g_ascii_strcasecmp(mode_str, "audit") == 0) {
        *mode = RAFAELIA_RUNTIME_MODE_AUDIT;
        return true;
    }
    if (g_ascii_strcasecmp(mode_str, "bench") == 0) {
        *mode = RAFAELIA_RUNTIME_MODE_BENCH;
        return true;
    }

    return false;
}

static void rafaelia_runtime_sanitize_config(rafaelia_runtime_config_t *config)
{
    if (!config) {
        return;
    }

    if (config->tick_ms == 0) {
        config->tick_ms = 1;
    }
}

void rafaelia_runtime_set_config(const rafaelia_runtime_config_t *config)
{
    if (!config) {
        return;
    }

    rafaelia_runtime_state.config = *config;
    rafaelia_runtime_sanitize_config(&rafaelia_runtime_state.config);
}

static void rafaelia_runtime_log_tick(const rafaelia_runtime_state_t *state)
{
    if (!state) {
        return;
    }

    if (state->config.mode == RAFAELIA_RUNTIME_MODE_TRACE ||
        state->config.debug) {
        trace_rafaelia_runtime_tick(state->ticks_total,
                                    state->last_tick_duration_ns / 1000.0,
                                    state->tick_avg_us,
                                    state->tick_p95_us,
                                    state->score_ec,
                                    state->adaptive_scale,
                                    state->overhead_backoff,
                                    state->entropy_last,
                                    state->coherence_last,
                                    state->running,
                                    rafaelia_runtime_mode_name(state->config.mode));
    }

    if (state->config.mode == RAFAELIA_RUNTIME_MODE_LOG ||
        state->config.debug) {
        error_report("RAFAELIA runtime tick=%" PRIu64
                     " dt_last_us=%.3f dt_avg_us=%.3f dt_p95_us=%.3f"
                     " score_ec=%.6f scale=%u backoff=%u"
                     " entropy=%.6f coherence=%.6f running=%d mode=%s",
                     state->ticks_total,
                     state->last_tick_duration_ns / 1000.0,
                     state->tick_avg_us,
                     state->tick_p95_us,
                     state->score_ec,
                     state->adaptive_scale,
                     state->overhead_backoff,
                     state->entropy_last,
                     state->coherence_last,
                     state->running,
                     rafaelia_runtime_mode_name(state->config.mode));
    }
}

static void rafaelia_runtime_log_state_change(const rafaelia_runtime_state_t *state)
{
    if (!state) {
        return;
    }

    if (state->config.mode == RAFAELIA_RUNTIME_MODE_TRACE ||
        state->config.debug) {
        trace_rafaelia_runtime_state_change(state->running,
                                            RunState_str(state->last_runstate));
    }
}

static void rafaelia_runtime_tick_once(rafaelia_runtime_state_t *state)
{
    int64_t start_ns;
    int64_t end_ns;
    uint64_t tick_ns;

    start_ns = qemu_clock_get_ns(QEMU_CLOCK_REALTIME);
    rafaelia_loop_step(&state->core);
    end_ns = qemu_clock_get_ns(QEMU_CLOCK_REALTIME);
    tick_ns = end_ns > start_ns ? (uint64_t)(end_ns - start_ns) : 0u;
    state->ticks_total++;
    state->tick_ns_total += tick_ns;
    state->last_tick_duration_ns = tick_ns;
    state->tick_ns_history[state->tick_ns_index] = tick_ns;
    state->tick_ns_index = (state->tick_ns_index + 1) %
        RAFAELIA_RUNTIME_TICK_HISTORY;
    if (state->tick_ns_count < RAFAELIA_RUNTIME_TICK_HISTORY) {
        state->tick_ns_count++;
    }
    if (state->ticks_total > 0) {
        state->tick_avg_us = (double)state->tick_ns_total /
            (double)state->ticks_total / 1000.0;
    }
    if (state->tick_ns_count > 0) {
        uint64_t sorted[RAFAELIA_RUNTIME_TICK_HISTORY];
        size_t count = state->tick_ns_count;
        size_t idx;

        for (size_t i = 0; i < count; i++) {
            sorted[i] = state->tick_ns_history[i];
        }
        qsort(sorted, count, sizeof(sorted[0]),
              rafaelia_runtime_compare_u64);
        idx = (count * 95 + 99) / 100;
        if (idx == 0) {
            idx = 1;
        }
        state->tick_p95_us = (double)sorted[idx - 1] / 1000.0;
    }
    state->entropy_last = rafaelia_cycle_measure(&state->core.cycle);
    state->coherence_last = state->core.phi_ethica.coerencia;

    if (state->ticks_total == 1) {
        state->entropy_prev = state->entropy_last;
        state->coherence_prev = state->coherence_last;
        state->entropy_ewma = state->entropy_last;
        state->coherence_ewma = state->coherence_last;
        state->score_ec = state->coherence_last / (1.0 + fabs(state->entropy_last));
    } else {
        double entropy_delta = fabs(state->entropy_last - state->entropy_prev);
        double coherence_delta = fabs(state->coherence_last - state->coherence_prev);
        bool stable = entropy_delta < RAFAELIA_RUNTIME_ENTROPY_DELTA_STABLE &&
            coherence_delta < RAFAELIA_RUNTIME_COHERENCE_DELTA_STABLE;

        state->entropy_ewma = (RAFAELIA_RUNTIME_EWMA_ALPHA * state->entropy_last) +
            ((1.0 - RAFAELIA_RUNTIME_EWMA_ALPHA) * state->entropy_ewma);
        state->coherence_ewma = (RAFAELIA_RUNTIME_EWMA_ALPHA * state->coherence_last) +
            ((1.0 - RAFAELIA_RUNTIME_EWMA_ALPHA) * state->coherence_ewma);
        state->score_ec = state->coherence_ewma /
            (1.0 + fabs(state->entropy_ewma));

        if (state->running) {
            if (stable) {
                state->adaptive_stable_ticks++;
                if (state->adaptive_stable_ticks >=
                    RAFAELIA_RUNTIME_ADAPTIVE_STABLE_TICKS &&
                    state->adaptive_scale < RAFAELIA_RUNTIME_ADAPTIVE_SCALE_MAX) {
                    state->adaptive_scale++;
                    state->adaptive_stable_ticks = 0;
                }
            } else {
                state->adaptive_stable_ticks = 0;
                if (state->adaptive_scale > 1) {
                    state->adaptive_scale--;
                }
            }
        }

        state->entropy_prev = state->entropy_last;
        state->coherence_prev = state->coherence_last;
    }

    if ((tick_ns / 1000u) > RAFAELIA_RUNTIME_OVERHEAD_CAP_US) {
        state->overhead_throttle_count++;
        if (state->overhead_backoff < RAFAELIA_RUNTIME_OVERHEAD_BACKOFF_MAX) {
            state->overhead_backoff++;
        }
    } else if (state->overhead_backoff > 0 &&
               (tick_ns / 1000u) < (RAFAELIA_RUNTIME_OVERHEAD_CAP_US / 2)) {
        state->overhead_backoff--;
    }

    rafaelia_runtime_log_tick(state);
}

static uint32_t rafaelia_runtime_tick_interval_ms(const rafaelia_runtime_state_t *state)
{
    uint64_t tick_ms = state->config.tick_ms;
    uint64_t scale = state->adaptive_scale ? state->adaptive_scale : 1u;

    if (tick_ms > UINT64_MAX / scale) {
        tick_ms = UINT64_MAX;
    } else {
        tick_ms *= scale;
    }

    if (state->overhead_backoff > 0) {
        uint64_t backoff = 1ULL << state->overhead_backoff;
        if (tick_ms > UINT64_MAX / backoff) {
            tick_ms = UINT64_MAX;
        } else {
            tick_ms *= backoff;
        }
    }
    if (tick_ms > UINT32_MAX) {
        tick_ms = UINT32_MAX;
    }

    if (state->running) {
        return (uint32_t)tick_ms;
    }

    if (tick_ms > (UINT32_MAX / 5)) {
        return (uint32_t)tick_ms;
    }

    return (uint32_t)(tick_ms * 5);
}

static void rafaelia_runtime_maybe_tick(rafaelia_runtime_state_t *state)
{
    int64_t now_ms;
    int64_t elapsed_ms;
    uint32_t interval_ms;
    uint64_t ticks_to_run;

    if (!state || !state->initialized || !state->config.enabled) {
        return;
    }

    now_ms = qemu_clock_get_ms(QEMU_CLOCK_REALTIME);
    interval_ms = rafaelia_runtime_tick_interval_ms(state);
    if (state->last_tick_ms == 0) {
        state->last_tick_ms = now_ms;
        return;
    }

    elapsed_ms = now_ms - state->last_tick_ms;
    if (elapsed_ms < (int64_t)interval_ms) {
        return;
    }

    ticks_to_run = elapsed_ms / interval_ms;
    if (ticks_to_run > RAFAELIA_RUNTIME_TICK_CAP) {
        ticks_to_run = RAFAELIA_RUNTIME_TICK_CAP;
    }

    for (uint64_t i = 0; i < ticks_to_run; i++) {
        rafaelia_runtime_tick_once(state);
    }

    state->last_tick_ms += (int64_t)ticks_to_run * interval_ms;
}

static void rafaelia_runtime_main_loop_notify(Notifier *notifier, void *data)
{
    MainLoopPoll *poll = data;

    if (!poll || poll->state != MAIN_LOOP_POLL_OK) {
        return;
    }

    rafaelia_runtime_maybe_tick(container_of(notifier,
                                             rafaelia_runtime_state_t,
                                             main_loop_notifier));
}

static void rafaelia_runtime_vm_state_cb(void *opaque, bool running,
                                        RunState state)
{
    rafaelia_runtime_state_t *rt = opaque;

    if (!rt) {
        return;
    }

    rt->running = running;
    rt->last_runstate = state;
    rt->last_tick_ms = qemu_clock_get_ms(QEMU_CLOCK_REALTIME);

    rafaelia_runtime_log_state_change(rt);
}

static void rafaelia_runtime_shutdown_notify(Notifier *notifier, void *data)
{
    rafaelia_runtime_shutdown();
}

void rafaelia_runtime_init(void)
{
    rafaelia_runtime_state_t *state = &rafaelia_runtime_state;
    int hub_rc;

    if (state->initialized || !state->config.enabled) {
        return;
    }

    rafaelia_fiat_portal_init(&state->core);
    hub_rc = rafaelia_integration_hub_init(&state->hub, &state->core);
    if (hub_rc == 0) {
        state->hub_initialized = true;
        rafaelia_integration_hub_start(&state->hub);
    } else {
        error_report("RAFAELIA runtime: integration hub init failed");
    }

    state->running = runstate_is_running();
    state->last_runstate = runstate_get();
    state->last_tick_ms = qemu_clock_get_ms(QEMU_CLOCK_REALTIME);
    state->adaptive_scale = 1;
    state->adaptive_stable_ticks = 0;

    state->main_loop_notifier.notify = rafaelia_runtime_main_loop_notify;
    main_loop_poll_add_notifier(&state->main_loop_notifier);

    state->vm_state_entry = qemu_add_vm_change_state_handler(
        rafaelia_runtime_vm_state_cb, state);

    state->shutdown_notifier.notify = rafaelia_runtime_shutdown_notify;
    qemu_register_shutdown_notifier(&state->shutdown_notifier);

    state->initialized = true;

    rafaelia_runtime_log_state_change(state);
}

void rafaelia_runtime_tick(uint32_t dt_ms)
{
    rafaelia_runtime_state_t *state = &rafaelia_runtime_state;

    if (!state->initialized || !state->config.enabled) {
        return;
    }

    if (dt_ms == 0) {
        dt_ms = state->config.tick_ms;
    }

    state->last_tick_ms = qemu_clock_get_ms(QEMU_CLOCK_REALTIME);
    rafaelia_runtime_tick_once(state);
}

void rafaelia_runtime_shutdown(void)
{
    rafaelia_runtime_state_t *state = &rafaelia_runtime_state;
    g_autofree char *report = NULL;
    g_autofree char *digest = NULL;
    Error *local_err = NULL;

    if (!state->initialized) {
        return;
    }

    trace_rafaelia_runtime_shutdown(state->ticks_total,
                                    state->entropy_last,
                                    state->coherence_last);

    report = g_strdup_printf("ticks=%" PRIu64
                             " dt_avg_us=%.3f dt_p95_us=%.3f"
                             " score_ec=%.6f scale=%u backoff=%u"
                             " entropy=%.6f coherence=%.6f"
                             " throttles=%u mode=%s",
                             state->ticks_total,
                             state->tick_avg_us,
                             state->tick_p95_us,
                             state->score_ec,
                             state->adaptive_scale,
                             state->overhead_backoff,
                             state->entropy_last,
                             state->coherence_last,
                             state->overhead_throttle_count,
                             rafaelia_runtime_mode_name(state->config.mode));

    if (qcrypto_hash_digest(QCRYPTO_HASH_ALGO_SHA256,
                            report,
                            strlen(report),
                            &digest,
                            &local_err) < 0) {
        error_reportf_err(local_err, "RAFAELIA runtime hash failed: ");
    }

    error_report("RAFAELIA runtime shutdown: %s hash=%s",
                 report,
                 digest ? digest : "unavailable");

    if (state->vm_state_entry) {
        qemu_del_vm_change_state_handler(state->vm_state_entry);
        state->vm_state_entry = NULL;
    }

    main_loop_poll_remove_notifier(&state->main_loop_notifier);

    if (state->hub_initialized) {
        rafaelia_integration_hub_stop(&state->hub);
        rafaelia_integration_hub_cleanup(&state->hub);
        state->hub_initialized = false;
    }

    rafaelia_core_cleanup(&state->core);
    state->initialized = false;
}
