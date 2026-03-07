/*
 * RAFAELIA Route Table - deterministic host/arch execution routing
 */

#ifndef HW_RAFAELIA_ROUTE_TABLE_H
#define HW_RAFAELIA_ROUTE_TABLE_H

#include <stdbool.h>
#include <stdint.h>
#include "hw/core/rafaelia-rmr.h"

typedef enum rafaelia_route_id {
    RAFAELIA_ROUTE_ID_PORTABLE = 0,
    RAFAELIA_ROUTE_ID_X86_64_KVM_HOT,
    RAFAELIA_ROUTE_ID_X86_64_SOFTMMU,
    RAFAELIA_ROUTE_ID_AARCH64_KVM_HOT,
    RAFAELIA_ROUTE_ID_AARCH64_SOFTMMU,
    RAFAELIA_ROUTE_ID_PPC64,
    RAFAELIA_ROUTE_ID_RISCV
} rafaelia_route_id_t;

typedef struct rafaelia_route_decision {
    rafaelia_route_id_t id;
    const char *name;
    const char *arch;
    bool has_kvm_accel;
    uint32_t cpu_online;
    uint32_t page_bytes;
} rafaelia_route_decision_t;

const rafaelia_route_decision_t *
rafaelia_route_select(const rafaelia_rmr_instrument_snapshot_t *snapshot);

#endif /* HW_RAFAELIA_ROUTE_TABLE_H */
