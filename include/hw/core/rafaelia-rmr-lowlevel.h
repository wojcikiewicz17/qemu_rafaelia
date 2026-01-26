/*
 * RAFAELIA RMR Lowlevel Helpers
 * Deterministic primitives for memory, strings, and RNG.
 */

#ifndef HW_RAFAELIA_RMR_LOWLEVEL_H
#define HW_RAFAELIA_RMR_LOWLEVEL_H

#include <stddef.h>
#include <stdint.h>

void rafaelia_rmr_memzero(void *ptr, size_t len);
void rafaelia_rmr_memcpy(void *dst, const void *src, size_t len);
int rafaelia_rmr_memcmp(const void *a, const void *b, size_t len);
size_t rafaelia_rmr_strlen(const char *str);
void rafaelia_rmr_strlcpy(char *dst, const char *src, size_t dst_size);

void rafaelia_rmr_rng_seed(uint32_t seed);
uint32_t rafaelia_rmr_rng_next(void);

#endif /* HW_RAFAELIA_RMR_LOWLEVEL_H */
