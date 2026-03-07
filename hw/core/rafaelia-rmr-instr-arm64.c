#include "qemu/osdep.h"
#include "hw/core/rafaelia-rmr-instr.h"

bool rafaelia_rmr_collect_instruments_arm64(rafaelia_rmr_instrument_snapshot_t *snapshot)
{
    (void)snapshot;
    return true;
}
