#include "qemu/osdep.h"
#include "hw/core/rafaelia-rmr-instr.h"

#if defined(__i386__) || defined(__x86_64__)
static inline void rafaelia_rmr_cpuid(uint32_t leaf, uint32_t subleaf,
                                      uint32_t *eax, uint32_t *ebx,
                                      uint32_t *ecx, uint32_t *edx)
{
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;

    __asm__ volatile("cpuid"
                     : "=a"(a), "=b"(b), "=c"(c), "=d"(d)
                     : "a"(leaf), "c"(subleaf));

    *eax = a;
    *ebx = b;
    *ecx = c;
    *edx = d;
}
#endif

bool rafaelia_rmr_collect_instruments_x86(rafaelia_rmr_instrument_snapshot_t *snapshot)
{
#if defined(__i386__) || defined(__x86_64__)
    uint32_t eax, ebx, ecx, edx;

    if (!snapshot) {
        return false;
    }

    rafaelia_rmr_cpuid(1u, 0u, &eax, &ebx, &ecx, &edx);
    if (snapshot->cpu_online == 0) {
        uint32_t logical = (ebx >> 16) & 0xffu;
        if (logical > 0) {
            snapshot->cpu_online = logical;
        }
    }
#else
    (void)snapshot;
#endif
    return true;
}
