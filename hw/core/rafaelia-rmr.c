/*
 * RAFAELIA RMR - Low-overhead memory and runtime tuning helpers
 */

#include "qemu/osdep.h"
#include "qemu/cacheinfo.h"
#include "qemu/log.h"
#include "hw/core/rafaelia-rmr.h"
#include "hw/core/rafaelia-rmr-lowlevel.h"

static rafaelia_rmr_memalign_fn rafaelia_rmr_memalign_alloc = qemu_memalign;

void rafaelia_rmr_pool_set_memalign_for_test(rafaelia_rmr_memalign_fn fn)
{
    rafaelia_rmr_memalign_alloc = fn ? fn : qemu_memalign;
}

void rafaelia_rmr_pool_reset_memalign_for_test(void)
{
    rafaelia_rmr_memalign_alloc = qemu_memalign;
}

static size_t rafaelia_rmr_round_up(size_t value, size_t align)
{
    if (align == 0) {
        return value;
    }
    return (value + align - 1) / align * align;
}

static uint32_t rafaelia_rmr_default_alignment(void)
{
    if (qemu_dcache_linesize > 0) {
        return (uint32_t)qemu_dcache_linesize;
    }
    return 64;
}

rafaelia_rmr_pool_t *rafaelia_rmr_pool_create(size_t element_size,
                                              uint32_t capacity,
                                              uint32_t alignment)
{
    rafaelia_rmr_pool_t *pool;
    size_t stride;
    size_t total;
    uint8_t *cursor;
    void **slot;

    if (element_size == 0 || capacity == 0) {
        return NULL;
    }

    stride = element_size;
    if (stride < sizeof(void *)) {
        stride = sizeof(void *);
    }

    if (alignment == 0) {
        alignment = rafaelia_rmr_default_alignment();
    }

    stride = rafaelia_rmr_round_up(stride, alignment);

    if (capacity > SIZE_MAX / stride) {
        return NULL;
    }

    total = stride * capacity;

    pool = g_new0(rafaelia_rmr_pool_t, 1);
    pool->buffer = rafaelia_rmr_memalign_alloc(alignment, total);
    if (!pool->buffer) {
        g_free(pool);
        return NULL;
    }

    pool->element_size = element_size;
    pool->stride = stride;
    pool->capacity = capacity;
    pool->alignment = alignment;
    pool->begin = (uintptr_t)pool->buffer;
    pool->end = pool->begin + total;

    cursor = (uint8_t *)pool->buffer;
    for (uint32_t i = 0; i < capacity; i++) {
        slot = (void **)cursor;
        *slot = pool->free_list;
        pool->free_list = slot;
        cursor += stride;
    }

    return pool;
}

void rafaelia_rmr_pool_destroy(rafaelia_rmr_pool_t *pool)
{
    if (!pool) {
        return;
    }
    qemu_vfree(pool->buffer);
    g_free(pool);
}

void *rafaelia_rmr_pool_alloc(rafaelia_rmr_pool_t *pool)
{
    void *entry;

    if (!pool || !pool->free_list) {
        return NULL;
    }

    entry = rafaelia_rmr_pool_alloc_uninitialized(pool);
    if (entry) {
        rafaelia_rmr_memzero(entry, pool->element_size);
    }
    return entry;
}

void *rafaelia_rmr_pool_alloc_uninitialized(rafaelia_rmr_pool_t *pool)
{
    void *entry;

    if (!pool || !pool->free_list) {
        return NULL;
    }

    entry = pool->free_list;
    pool->free_list = *(void **)entry;
    pool->in_use++;
    return entry;
}

void rafaelia_rmr_pool_free(rafaelia_rmr_pool_t *pool, void *ptr)
{
    if (!pool || !ptr) {
        return;
    }

    if (!rafaelia_rmr_pool_owns(pool, ptr)) {
#ifdef DEBUG_RAFAELIA_RMR
        qemu_log_mask(LOG_GUEST_ERROR,
                      "rafaelia-rmr: ignoring free of non-pool pointer %p\n",
                      ptr);
#endif
        return;
    }

    *(void **)ptr = pool->free_list;
    pool->free_list = ptr;
    if (pool->in_use > 0) {
        pool->in_use--;
    }
}

bool rafaelia_rmr_pool_owns(const rafaelia_rmr_pool_t *pool, const void *ptr)
{
    uintptr_t addr;

    if (!pool || !ptr) {
        return false;
    }

    addr = (uintptr_t)ptr;
    if (addr < pool->begin || addr >= pool->end) {
        return false;
    }

    return ((addr - pool->begin) % pool->stride) == 0;
}

void rafaelia_rmr_detect(rafaelia_rmr_hw_profile_t *profile)
{
    if (!profile) {
        return;
    }

    rafaelia_rmr_memzero(profile, sizeof(*profile));

#if defined(__x86_64__) || defined(_M_X64)
    profile->arch = "x86_64";
#elif defined(__aarch64__)
    profile->arch = "aarch64";
#elif defined(__arm__) || defined(_M_ARM)
    profile->arch = "arm";
#elif defined(__ppc64__) || defined(__powerpc64__)
    profile->arch = "ppc64";
#elif defined(__riscv)
    profile->arch = "riscv";
#else
    profile->arch = "unknown";
#endif

    profile->pointer_bits = (uint32_t)(sizeof(void *) * 8);
    profile->cacheline_bytes = rafaelia_rmr_default_alignment();
    profile->alignment_bytes = profile->cacheline_bytes;
    profile->page_bytes = (uint32_t)getpagesize();

#if defined(__GNUC__) || defined(__clang__)
    profile->has_prefetch = true;
#else
    profile->has_prefetch = false;
#endif
}
