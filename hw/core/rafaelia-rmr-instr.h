#ifndef HW_RAFAELIA_RMR_INSTR_H
#define HW_RAFAELIA_RMR_INSTR_H

#include "hw/core/rafaelia-rmr.h"

bool rafaelia_rmr_collect_instruments_x86(rafaelia_rmr_instrument_snapshot_t *snapshot);
bool rafaelia_rmr_collect_instruments_arm64(rafaelia_rmr_instrument_snapshot_t *snapshot);
bool rafaelia_rmr_collect_instruments_riscv(rafaelia_rmr_instrument_snapshot_t *snapshot);
bool rafaelia_rmr_collect_instruments_posix(rafaelia_rmr_instrument_snapshot_t *snapshot);

#endif
