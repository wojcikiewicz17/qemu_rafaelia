/*
 * RAFAELIA Route Table - deterministic host/arch execution routing
 */

#include "qemu/osdep.h"
#include "hw/core/rafaelia-route-table.h"
#include "hw/core/rafaelia-rmr-lowlevel.h"

typedef struct rafaelia_route_rule {
    rafaelia_route_decision_t route;
} rafaelia_route_rule_t;

static const rafaelia_route_rule_t rafaelia_route_table[] = {
    {
        .route = {
            .id = RAFAELIA_ROUTE_ID_X86_64_KVM_HOT,
            .name = "x86_64-kvm-hot",
            .arch = "x86_64",
            .has_kvm_accel = true,
            .cpu_online = 4,
            .page_bytes = 4096,
        },
    },
    {
        .route = {
            .id = RAFAELIA_ROUTE_ID_X86_64_SOFTMMU,
            .name = "x86_64-softmmu",
            .arch = "x86_64",
            .has_kvm_accel = false,
            .cpu_online = 1,
            .page_bytes = 4096,
        },
    },
    {
        .route = {
            .id = RAFAELIA_ROUTE_ID_AARCH64_KVM_HOT,
            .name = "aarch64-kvm-hot",
            .arch = "aarch64",
            .has_kvm_accel = true,
            .cpu_online = 4,
            .page_bytes = 4096,
        },
    },
    {
        .route = {
            .id = RAFAELIA_ROUTE_ID_AARCH64_SOFTMMU,
            .name = "aarch64-softmmu",
            .arch = "aarch64",
            .has_kvm_accel = false,
            .cpu_online = 1,
            .page_bytes = 4096,
        },
    },
    {
        .route = {
            .id = RAFAELIA_ROUTE_ID_PPC64,
            .name = "ppc64-generic",
            .arch = "ppc64",
            .has_kvm_accel = false,
            .cpu_online = 1,
            .page_bytes = 65536,
        },
    },
    {
        .route = {
            .id = RAFAELIA_ROUTE_ID_RISCV,
            .name = "riscv-generic",
            .arch = "riscv",
            .has_kvm_accel = false,
            .cpu_online = 1,
            .page_bytes = 4096,
        },
    },
};

static const rafaelia_route_decision_t rafaelia_route_fallback = {
    .id = RAFAELIA_ROUTE_ID_PORTABLE,
    .name = "portable-fallback",
    .arch = "unknown",
    .has_kvm_accel = false,
    .cpu_online = 1,
    .page_bytes = 4096,
};

static bool rafaelia_route_arch_match(const char *rule_arch, const char *snapshot_arch)
{
    size_t rule_len;
    size_t snapshot_len;

    if (!rule_arch || !snapshot_arch) {
        return false;
    }

    rule_len = rafaelia_rmr_strlen(rule_arch);
    snapshot_len = rafaelia_rmr_strlen(snapshot_arch);

    if (rule_len == 0 || snapshot_len == 0) {
        return false;
    }

    if (rafaelia_rmr_memcmp(rule_arch, snapshot_arch, rule_len) == 0 &&
        snapshot_arch[rule_len] == '\0') {
        return true;
    }

    if (rafaelia_rmr_memcmp(rule_arch, "x86_64", 6) == 0 &&
        ((rafaelia_rmr_memcmp(snapshot_arch, "x86", 3) == 0 &&
          snapshot_arch[3] == '\0') ||
         (rafaelia_rmr_memcmp(snapshot_arch, "i386", 4) == 0 &&
          snapshot_arch[4] == '\0'))) {
        return true;
    }

    if (rafaelia_rmr_memcmp(rule_arch, "riscv", 5) == 0 &&
        rafaelia_rmr_memcmp(snapshot_arch, "riscv", 5) == 0) {
        return true;
    }

    return false;
}

static bool rafaelia_route_rule_match(const rafaelia_route_decision_t *route,
                                      const rafaelia_rmr_instrument_snapshot_t *snapshot)
{
    if (!route || !snapshot) {
        return false;
    }

    if (!rafaelia_route_arch_match(route->arch, snapshot->arch)) {
        return false;
    }

    if (route->has_kvm_accel != snapshot->has_kvm_accel) {
        return false;
    }

    if (snapshot->cpu_online < route->cpu_online) {
        return false;
    }

    if (snapshot->page_bytes < route->page_bytes) {
        return false;
    }

    return true;
}

const rafaelia_route_decision_t *
rafaelia_route_select(const rafaelia_rmr_instrument_snapshot_t *snapshot)
{
    size_t i;

    if (!snapshot) {
        return &rafaelia_route_fallback;
    }

    for (i = 0; i < ARRAY_SIZE(rafaelia_route_table); i++) {
        const rafaelia_route_decision_t *route = &rafaelia_route_table[i].route;

        if (rafaelia_route_rule_match(route, snapshot)) {
            return route;
        }
    }

    return &rafaelia_route_fallback;
}
