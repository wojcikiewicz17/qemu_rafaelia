/*
 * RAFAELIA QEMU shell adapter
 * Bridges kernel ABI calls to QEMU/RMR infrastructure.
 */

#include "qemu/osdep.h"
#include "hw/core/rafaelia-qemu-shell.h"
#include "hw/core/rafaelia-rmr.h"
#include "hw/core/rafaelia-rmr-lowlevel.h"
#include "hw/core/rafaelia-route-table.h"

static void abi_memzero(void *ptr, size_t len)
{
    rafaelia_rmr_memzero(ptr, len);
}

static void abi_strlcpy(char *dst, const char *src, size_t dst_size)
{
    rafaelia_rmr_strlcpy(dst, src, dst_size);
}

static size_t abi_strlen_bytes(const char *str)
{
    return rafaelia_rmr_strlen(str);
}

static int abi_memcmp_bytes(const void *a, const void *b, size_t len)
{
    return rafaelia_rmr_memcmp(a, b, len);
}

static void *abi_alloc_zero(size_t size)
{
    return g_malloc0(size);
}

static void abi_free_mem(void *ptr)
{
    g_free(ptr);
}

static void *abi_pool_create(size_t element_size, uint32_t capacity,
                             uint32_t alignment)
{
    return rafaelia_rmr_pool_create(element_size, capacity, alignment);
}

static void abi_pool_destroy(void *pool)
{
    rafaelia_rmr_pool_destroy(pool);
}

static void *abi_pool_alloc_uninitialized(void *pool)
{
    return rafaelia_rmr_pool_alloc_uninitialized(pool);
}

static bool abi_pool_owns(const void *pool, const void *ptr)
{
    return rafaelia_rmr_pool_owns(pool, ptr);
}

static void abi_pool_free(void *pool, void *ptr)
{
    rafaelia_rmr_pool_free(pool, ptr);
}

static bool abi_collect_instruments(rafaelia_kernel_instrument_snapshot_t *snapshot)
{
    return rafaelia_rmr_collect_instruments((rafaelia_rmr_instrument_snapshot_t *)snapshot);
}

static const rafaelia_kernel_route_decision_t *
abi_route_select(const rafaelia_kernel_instrument_snapshot_t *snapshot)
{
    return (const rafaelia_kernel_route_decision_t *)
        rafaelia_route_select((const rafaelia_rmr_instrument_snapshot_t *)snapshot);
}

static void abi_rng_seed(uint32_t seed)
{
    rafaelia_rmr_rng_seed(seed);
}

static uint32_t abi_rng_next(void)
{
    return rafaelia_rmr_rng_next();
}

static const rafaelia_kernel_abi_t qemu_shell_abi = {
    .memzero = abi_memzero,
    .strlcpy = abi_strlcpy,
    .strlen_bytes = abi_strlen_bytes,
    .memcmp_bytes = abi_memcmp_bytes,
    .alloc_zero = abi_alloc_zero,
    .free_mem = abi_free_mem,
    .pool_create = abi_pool_create,
    .pool_destroy = abi_pool_destroy,
    .pool_alloc_uninitialized = abi_pool_alloc_uninitialized,
    .pool_owns = abi_pool_owns,
    .pool_free = abi_pool_free,
    .collect_instruments = abi_collect_instruments,
    .route_select = abi_route_select,
    .rng_seed = abi_rng_seed,
    .rng_next = abi_rng_next,
};

const rafaelia_kernel_abi_t *rafaelia_qemu_shell_abi(void)
{
    return &qemu_shell_abi;
}
