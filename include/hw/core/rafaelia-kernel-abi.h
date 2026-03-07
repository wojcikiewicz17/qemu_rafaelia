/*
 * RAFAELIA Kernel ABI - stable low-level boundary for infrastructure calls
 */

#ifndef HW_RAFAELIA_KERNEL_ABI_H
#define HW_RAFAELIA_KERNEL_ABI_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum rafaelia_kernel_route_id {
    RAFAELIA_KERNEL_ROUTE_ID_PORTABLE = 0,
    RAFAELIA_KERNEL_ROUTE_ID_X86_64_KVM_HOT,
    RAFAELIA_KERNEL_ROUTE_ID_X86_64_SOFTMMU,
    RAFAELIA_KERNEL_ROUTE_ID_AARCH64_KVM_HOT,
    RAFAELIA_KERNEL_ROUTE_ID_AARCH64_SOFTMMU,
    RAFAELIA_KERNEL_ROUTE_ID_PPC64,
    RAFAELIA_KERNEL_ROUTE_ID_RISCV
} rafaelia_kernel_route_id_t;

typedef struct rafaelia_kernel_instrument_snapshot {
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
} rafaelia_kernel_instrument_snapshot_t;

typedef struct rafaelia_kernel_route_decision {
    rafaelia_kernel_route_id_t id;
    const char *name;
    const char *arch;
    bool has_kvm_accel;
    uint32_t cpu_online;
    uint32_t page_bytes;
} rafaelia_kernel_route_decision_t;

typedef struct rafaelia_kernel_abi {
    void (*memzero)(void *ptr, size_t len);
    void (*strlcpy)(char *dst, const char *src, size_t dst_size);
    size_t (*strlen_bytes)(const char *str);
    int (*memcmp_bytes)(const void *a, const void *b, size_t len);

    void *(*alloc_zero)(size_t size);
    void (*free_mem)(void *ptr);

    void *(*pool_create)(size_t element_size, uint32_t capacity,
                         uint32_t alignment);
    void (*pool_destroy)(void *pool);
    void *(*pool_alloc_uninitialized)(void *pool);
    bool (*pool_owns)(const void *pool, const void *ptr);
    void (*pool_free)(void *pool, void *ptr);

    bool (*collect_instruments)(rafaelia_kernel_instrument_snapshot_t *snapshot);
    const rafaelia_kernel_route_decision_t *
    (*route_select)(const rafaelia_kernel_instrument_snapshot_t *snapshot);

    void (*rng_seed)(uint32_t seed);
    uint32_t (*rng_next)(void);
} rafaelia_kernel_abi_t;

#endif /* HW_RAFAELIA_KERNEL_ABI_H */
