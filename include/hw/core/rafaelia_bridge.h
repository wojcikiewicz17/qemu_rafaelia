/*
 * RAFAELIA Bridge API
 * Lightweight C bridge for Android/JNI consumers.
 */

#ifndef HW_RAFAELIA_BRIDGE_H
#define HW_RAFAELIA_BRIDGE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct rafaelia_bridge_metrics {
    float cpu_usage_percent;
    uint64_t memory_used_mb;
    uint64_t memory_total_mb;
    float disk_read_mbps;
    float disk_write_mbps;
    float network_rx_kbps;
    float network_tx_kbps;
    uint32_t fps;
    bool vnc_connected;
} rafaelia_bridge_metrics_t;

bool rafaelia_bridge_start_vm(const char *vm_id);
bool rafaelia_bridge_stop_vm(void);
bool rafaelia_bridge_collect_metrics(rafaelia_bridge_metrics_t *metrics);

#endif /* HW_RAFAELIA_BRIDGE_H */
