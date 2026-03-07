/*
 * RAFAELIA RMR Lowlevel Helpers
 * Deterministic primitives for memory, strings, and RNG.
 */

#include "hw/core/rafaelia-rmr-lowlevel.h"

static uint32_t rafaelia_rmr_rng_state = 0xA53C9E5u;

void rafaelia_rmr_memzero(void *ptr, size_t len)
{
    uint8_t *out = ptr;

    if (!out || len == 0) {
        return;
    }

#if defined(__x86_64__) && (defined(__GNUC__) || defined(__clang__))
    __asm__ volatile("rep stosb"
                     : "+D"(out), "+c"(len)
                     : "a"(0)
                     : "memory");
    return;
#endif

    while (((uintptr_t)out & (sizeof(uintptr_t) - 1u)) != 0u && len > 0) {
        *out++ = 0;
        len--;
    }

    while (len >= sizeof(uintptr_t)) {
        *(uintptr_t *)out = 0;
        out += sizeof(uintptr_t);
        len -= sizeof(uintptr_t);
    }

    while (len--) {
        *out++ = 0;
    }
}

void rafaelia_rmr_memcpy(void *dst, const void *src, size_t len)
{
    uint8_t *out = dst;
    const uint8_t *in = src;

    if (!out || !in || len == 0) {
        return;
    }

#if defined(__x86_64__) && (defined(__GNUC__) || defined(__clang__))
    __asm__ volatile("rep movsb"
                     : "+D"(out), "+S"(in), "+c"(len)
                     :
                     : "memory");
    return;
#endif

    while ((((uintptr_t)out | (uintptr_t)in) & (sizeof(uintptr_t) - 1u)) != 0u && len > 0) {
        *out++ = *in++;
        len--;
    }

    while (len >= sizeof(uintptr_t)) {
        *(uintptr_t *)out = *(const uintptr_t *)in;
        out += sizeof(uintptr_t);
        in += sizeof(uintptr_t);
        len -= sizeof(uintptr_t);
    }

    while (len--) {
        *out++ = *in++;
    }
}

int rafaelia_rmr_memcmp(const void *a, const void *b, size_t len)
{
    const uint8_t *pa = a;
    const uint8_t *pb = b;

    if (!pa || !pb || len == 0) {
        return 0;
    }

    while (len >= sizeof(uintptr_t) &&
           ((((uintptr_t)pa | (uintptr_t)pb) & (sizeof(uintptr_t) - 1u)) == 0u)) {
        uintptr_t wa = *(const uintptr_t *)pa;
        uintptr_t wb = *(const uintptr_t *)pb;

        if (wa != wb) {
            break;
        }

        pa += sizeof(uintptr_t);
        pb += sizeof(uintptr_t);
        len -= sizeof(uintptr_t);
    }

    while (len--) {
        if (*pa != *pb) {
            return (int)*pa - (int)*pb;
        }
        pa++;
        pb++;
    }

    return 0;
}

size_t rafaelia_rmr_strlen(const char *str)
{
    size_t len = 0;

    if (!str) {
        return 0;
    }

    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void rafaelia_rmr_strlcpy(char *dst, const char *src, size_t dst_size)
{
    size_t i = 0;

    if (!dst || !src || dst_size == 0) {
        return;
    }

    while (i + 1 < dst_size && src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

void rafaelia_rmr_rng_seed(uint32_t seed)
{
    if (seed == 0) {
        seed = 0xA53C9E5u;
    }
    rafaelia_rmr_rng_state ^= seed + 0x9E3779B9u;
    if (rafaelia_rmr_rng_state == 0) {
        rafaelia_rmr_rng_state = 0x6D2B79F5u;
    }
}

uint32_t rafaelia_rmr_rng_next(void)
{
    uint32_t x = rafaelia_rmr_rng_state;

    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    rafaelia_rmr_rng_state = x;
    return x;
}
