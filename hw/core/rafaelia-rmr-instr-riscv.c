#include "qemu/osdep.h"
#include "hw/core/rafaelia-rmr-instr.h"

bool rafaelia_rmr_collect_instruments_riscv(rafaelia_rmr_instrument_snapshot_t *snapshot)
{
    if (!snapshot) {
        return false;
    }

#if defined(__riscv)
    if (snapshot->page_bytes < 4096u) {
        snapshot->page_bytes = 4096u;
    }

#if __riscv_xlen == 64
    if (snapshot->pointer_bits < 64u) {
        snapshot->pointer_bits = 64u;
    }
#endif
#endif

    return true;
}
