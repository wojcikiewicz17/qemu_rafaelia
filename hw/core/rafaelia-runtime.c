/*
 * RAFAELIA Runtime Hook
 */

#include "qemu/osdep.h"
#include <inttypes.h>
#include "qemu/error-report.h"
#include "qemu/main-loop.h"
#include "qemu/notify.h"
#include "qemu/timer.h"
#include "system/runstate.h"
#include "hw/core/rafaelia-core.h"
#include "hw/core/rafaelia-integration.h"
#include "hw/core/rafaelia-runtime.h"
#include "trace.h"

#define RAFAELIA_RUNTIME_TICK_CAP 100u

typedef struct rafaelia_runtime_state {
    rafaelia_runtime_config_t config;
    rafaelia_core_t core;
    rafaelia_integration_hub_t hub;
    bool hub_initialized;
    bool initialized;
    bool running;
    uint64_t ticks_total;
    double entropy_last;
    double coherence_last;
    int64_t last_tick_ms;
    RunState last_runstate;
    Notifier main_loop_notifier;
    Notifier shutdown_notifier;
    VMChangeStateEntry *vm_state_entry;
} rafaelia_runtime_state_t;

static rafaelia_runtime_state_t rafaelia_runtime_state;

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
                                    state->entropy_last,
                                    state->coherence_last,
                                    state->running,
                                    rafaelia_runtime_mode_name(state->config.mode));
    }

    if (state->config.mode == RAFAELIA_RUNTIME_MODE_LOG ||
        state->config.debug) {
        error_report("RAFAELIA runtime tick=%" PRIu64
                     " entropy=%.6f coherence=%.6f running=%d mode=%s",
                     state->ticks_total,
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
    rafaelia_loop_step(&state->core);
    state->ticks_total++;
    state->entropy_last = rafaelia_cycle_measure(&state->core.cycle);
    state->coherence_last = state->core.phi_ethica.coerencia;

    rafaelia_runtime_log_tick(state);
}

static uint32_t rafaelia_runtime_tick_interval_ms(const rafaelia_runtime_state_t *state)
{
    uint32_t tick_ms = state->config.tick_ms;

    if (state->running) {
        return tick_ms;
    }

    if (tick_ms > (UINT32_MAX / 5)) {
        return tick_ms;
    }

    return tick_ms * 5;
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

    if (!state->initialized) {
        return;
    }

    trace_rafaelia_runtime_shutdown(state->ticks_total,
                                    state->entropy_last,
                                    state->coherence_last);

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
