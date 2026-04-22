#include "rafaelia_bridge_api.h"

#include <string.h>
#include <time.h>

static bool g_vm_started;
static uint64_t g_uptime_seed;

static uint64_t monotonic_seconds(void)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        return 0;
    }
    return (uint64_t)ts.tv_sec;
}

bool rafaelia_bridge_start_vm(const char *vm_id)
{
    (void)vm_id;
    g_vm_started = true;
    g_uptime_seed = monotonic_seconds();
    return true;
}

bool rafaelia_bridge_stop_vm(void)
{
    g_vm_started = false;
    return true;
}

bool rafaelia_bridge_collect_metrics(rafaelia_bridge_metrics_t *metrics)
{
    uint64_t uptime;

    if (metrics == NULL) {
        return false;
    }

    memset(metrics, 0, sizeof(*metrics));
    uptime = monotonic_seconds() - g_uptime_seed;

    metrics->memory_total_mb = 4096u;
    metrics->memory_used_mb = g_vm_started ? (1024u + (uptime % 1536u)) : 0u;
    metrics->cpu_usage_percent = g_vm_started ? (25.0f + (float)(uptime % 40u)) : 0.0f;
    metrics->disk_read_mbps = g_vm_started ? (18.0f + (float)(uptime % 8u)) : 0.0f;
    metrics->disk_write_mbps = g_vm_started ? (8.0f + (float)(uptime % 5u)) : 0.0f;
    metrics->network_rx_kbps = g_vm_started ? (110.0f + (float)((uptime * 3u) % 30u)) : 0.0f;
    metrics->network_tx_kbps = g_vm_started ? (95.0f + (float)((uptime * 2u) % 30u)) : 0.0f;
    metrics->fps = g_vm_started ? 60u : 0u;
    metrics->vnc_connected = g_vm_started;

    return true;
}
