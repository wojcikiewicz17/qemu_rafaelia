/*
 * RAFAELIA standalone shell adapter
 * Keeps integration tests buildable without full QEMU configure/meson bootstrap.
 */

#include "hw/core/rafaelia-qemu-shell.h"
#include "hw/core/rafaelia-rmr-lowlevel.h"

#include <stdlib.h>
#include <string.h>

#ifdef __linux__
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <unistd.h>
#endif

typedef struct rafaelia_standalone_pool {
    uint32_t capacity;
    uint32_t in_use;
    size_t stride;
    void **free_list;
    uint8_t *buffer;
} rafaelia_standalone_pool_t;

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
    return calloc(1, size);
}

static void abi_free_mem(void *ptr)
{
    free(ptr);
}

static size_t round_up(size_t value, size_t align)
{
    return ((value + align - 1u) / align) * align;
}

static void *abi_pool_create(size_t element_size, uint32_t capacity, uint32_t alignment)
{
    rafaelia_standalone_pool_t *pool;
    size_t stride;

    if (!element_size || !capacity) {
        return NULL;
    }

    if (!alignment) {
        alignment = sizeof(void *);
    }

    stride = round_up(element_size < sizeof(void *) ? sizeof(void *) : element_size,
                     alignment);
    pool = calloc(1, sizeof(*pool));
    if (!pool) {
        return NULL;
    }

    pool->buffer = calloc(capacity, stride);
    if (!pool->buffer) {
        free(pool);
        return NULL;
    }

    pool->capacity = capacity;
    pool->stride = stride;
    for (uint32_t i = 0; i < capacity; i++) {
        void **slot = (void **)(pool->buffer + (i * stride));
        *slot = pool->free_list;
        pool->free_list = slot;
    }
    return pool;
}

static void abi_pool_destroy(void *raw_pool)
{
    rafaelia_standalone_pool_t *pool = raw_pool;

    if (!pool) {
        return;
    }
    free(pool->buffer);
    free(pool);
}

static void *abi_pool_alloc_uninitialized(void *raw_pool)
{
    rafaelia_standalone_pool_t *pool = raw_pool;
    void *entry;

    if (!pool || !pool->free_list) {
        return NULL;
    }
    entry = pool->free_list;
    pool->free_list = *(void **)entry;
    pool->in_use++;
    return entry;
}

static bool abi_pool_owns(const void *raw_pool, const void *ptr)
{
    const rafaelia_standalone_pool_t *pool = raw_pool;
    uintptr_t begin;
    uintptr_t end;
    uintptr_t addr;

    if (!pool || !ptr) {
        return false;
    }

    begin = (uintptr_t)pool->buffer;
    end = begin + (pool->capacity * pool->stride);
    addr = (uintptr_t)ptr;

    return addr >= begin && addr < end && ((addr - begin) % pool->stride) == 0;
}

static void abi_pool_free(void *raw_pool, void *ptr)
{
    rafaelia_standalone_pool_t *pool = raw_pool;

    if (!abi_pool_owns(pool, ptr)) {
        return;
    }

    *(void **)ptr = pool->free_list;
    pool->free_list = ptr;
    if (pool->in_use > 0) {
        pool->in_use--;
    }
}

static bool abi_collect_instruments(rafaelia_kernel_instrument_snapshot_t *snapshot)
{
    if (!snapshot) {
        return false;
    }

    memset(snapshot, 0, sizeof(*snapshot));
    snapshot->arch = "portable";
    snapshot->os = "unknown";
    snapshot->pointer_bits = (uint32_t)(sizeof(void *) * 8u);
    snapshot->page_bytes = 4096u;
    snapshot->cpu_online = 1u;

#ifdef __linux__
    {
        struct utsname uts;
        struct sysinfo si;

        if (uname(&uts) == 0) {
            snapshot->arch = uts.machine;
            snapshot->os = uts.sysname;
            abi_strlcpy(snapshot->kernel_release, uts.release,
                        sizeof(snapshot->kernel_release));
            abi_strlcpy(snapshot->machine, uts.machine, sizeof(snapshot->machine));
        }

        snapshot->cpu_online = (uint32_t)sysconf(_SC_NPROCESSORS_ONLN);
        snapshot->page_bytes = (uint32_t)sysconf(_SC_PAGESIZE);
        if (sysinfo(&si) == 0) {
            snapshot->uptime_seconds = (uint64_t)si.uptime;
            snapshot->total_ram_kib = (uint64_t)(si.totalram / 1024u);
            snapshot->free_ram_kib = (uint64_t)(si.freeram / 1024u);
        }
    }
#endif

    return true;
}

static const rafaelia_kernel_route_decision_t *
abi_route_select(const rafaelia_kernel_instrument_snapshot_t *snapshot)
{
    static rafaelia_kernel_route_decision_t decision;

    if (!snapshot) {
        return NULL;
    }

    decision.id = RAFAELIA_KERNEL_ROUTE_ID_PORTABLE;
    decision.name = "portable";
    decision.arch = snapshot->arch;
    decision.has_kvm_accel = false;
    decision.cpu_online = snapshot->cpu_online;
    decision.page_bytes = snapshot->page_bytes;
    return &decision;
}

static const rafaelia_kernel_abi_t standalone_abi = {
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
    .rng_seed = rafaelia_rmr_rng_seed,
    .rng_next = rafaelia_rmr_rng_next,
};

const rafaelia_kernel_abi_t *rafaelia_qemu_shell_abi(void)
{
    return &standalone_abi;
}
