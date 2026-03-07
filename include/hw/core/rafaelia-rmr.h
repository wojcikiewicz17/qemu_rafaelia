/*
 * RAFAELIA RMR - Low-overhead memory and runtime tuning helpers
 *
 * This module provides a small pool allocator and hardware profile probes
 * to reduce allocation overhead and encourage low-level interoperability.
 */

#ifndef HW_RAFAELIA_RMR_H
#define HW_RAFAELIA_RMR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct rafaelia_rmr_hw_profile {
    const char *arch;
    uint32_t pointer_bits;
    uint32_t cacheline_bytes;
    uint32_t alignment_bytes;
    uint32_t page_bytes;
    bool has_prefetch;
} rafaelia_rmr_hw_profile_t;

typedef struct rafaelia_rmr_instrument_snapshot {
    const char *arch;
    const char *os;
    char kernel_release[64];
    char machine[32];
    uint32_t pointer_bits;
    uint32_t cpu_online;
    uint32_t page_bytes;
    uint64_t uptime_seconds;
    uint64_t total_ram_kib;
    uint64_t free_ram_kib;
    bool has_kvm_accel;
} rafaelia_rmr_instrument_snapshot_t;

typedef enum rafaelia_rmr_route_class {
    RAFAELIA_RMR_ROUTE_FALLBACK = 0,
    RAFAELIA_RMR_ROUTE_PORTABLE,
    RAFAELIA_RMR_ROUTE_HOST_FAST,
    RAFAELIA_RMR_ROUTE_KVM_ACCEL,
} rafaelia_rmr_route_class_t;

typedef struct rafaelia_rmr_route_decision {
    rafaelia_rmr_route_class_t route;
    uint32_t route_score;
    uint32_t lane_id;
    bool prefers_large_pages;
} rafaelia_rmr_route_decision_t;

typedef void *(*rafaelia_rmr_memalign_fn)(size_t alignment, size_t size);

typedef struct rafaelia_rmr_pool {
    void *buffer;
    void *free_list;
    size_t element_size;
    size_t stride;
    uint32_t capacity;
    uint32_t in_use;
    uint32_t alignment;
    uintptr_t begin;
    uintptr_t end;
} rafaelia_rmr_pool_t;

rafaelia_rmr_pool_t *rafaelia_rmr_pool_create(size_t element_size,
                                              uint32_t capacity,
                                              uint32_t alignment);
void rafaelia_rmr_pool_destroy(rafaelia_rmr_pool_t *pool);
void rafaelia_rmr_pool_set_memalign_for_test(rafaelia_rmr_memalign_fn fn);
void rafaelia_rmr_pool_reset_memalign_for_test(void);
void *rafaelia_rmr_pool_alloc(rafaelia_rmr_pool_t *pool);
void *rafaelia_rmr_pool_alloc_uninitialized(rafaelia_rmr_pool_t *pool);
void rafaelia_rmr_pool_free(rafaelia_rmr_pool_t *pool, void *ptr);
bool rafaelia_rmr_pool_owns(const rafaelia_rmr_pool_t *pool, const void *ptr);

void rafaelia_rmr_detect(rafaelia_rmr_hw_profile_t *profile);
bool rafaelia_rmr_collect_instruments(rafaelia_rmr_instrument_snapshot_t *snapshot);
bool rafaelia_rmr_route_select(const rafaelia_rmr_instrument_snapshot_t *snapshot,
                              rafaelia_rmr_route_decision_t *decision);

static inline void rafaelia_rmr_prefetch(const void *ptr)
{
#if defined(__GNUC__) || defined(__clang__)
    __builtin_prefetch(ptr, 0, 1);
#else
    (void)ptr;
#endif
}

#endif /* HW_RAFAELIA_RMR_H */
