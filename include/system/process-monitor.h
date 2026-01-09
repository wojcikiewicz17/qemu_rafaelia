/*
 * QEMU Process Performance Monitor
 *
 * Copyright (c) 2026 QEMU contributors
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#ifndef QEMU_PROCESS_MONITOR_H
#define QEMU_PROCESS_MONITOR_H

/*
 * Initialize the process monitor subsystem
 */
void qemu_process_monitor_init(void);

/*
 * Record various process events for performance tracking
 */
void qemu_process_monitor_record_main_loop(void);
void qemu_process_monitor_record_cpu_kick(void);
void qemu_process_monitor_record_runstate_change(void);
void qemu_process_monitor_record_bql_contention(void);

/*
 * Get current statistics
 */
void qemu_process_monitor_get_stats(uint64_t *loops, uint64_t *kicks,
                                    uint64_t *transitions, uint64_t *contentions);

/*
 * Get performance rates (per second)
 */
void qemu_process_monitor_get_rates(double *loop_rate, double *kick_rate,
                                    double *transition_rate);

/*
 * Reset all counters
 */
void qemu_process_monitor_reset(void);

#endif /* QEMU_PROCESS_MONITOR_H */
