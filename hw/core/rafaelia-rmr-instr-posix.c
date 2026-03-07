#include "qemu/osdep.h"
#include "hw/core/rafaelia-rmr-lowlevel.h"
#include "hw/core/rafaelia-rmr-instr.h"

#ifdef CONFIG_POSIX
#include <sys/utsname.h>
#include <unistd.h>
#endif

#if defined(CONFIG_LINUX)
#include <sys/sysinfo.h>
#endif

bool rafaelia_rmr_collect_instruments_posix(rafaelia_rmr_instrument_snapshot_t *snapshot)
{
#ifdef CONFIG_POSIX
    struct utsname uts;
#endif
#if defined(CONFIG_LINUX)
    struct sysinfo info;
#endif

    if (!snapshot) {
        return false;
    }

#if defined(_SC_NPROCESSORS_ONLN)
    if (snapshot->cpu_online == 0) {
        long cpus = sysconf(_SC_NPROCESSORS_ONLN);
        if (cpus > 0 && cpus <= UINT32_MAX) {
            snapshot->cpu_online = (uint32_t)cpus;
        }
    }
#endif

#ifdef CONFIG_POSIX
    if (snapshot->kernel_release[0] == '\0' || snapshot->machine[0] == '\0') {
        if (uname(&uts) == 0) {
            if (snapshot->kernel_release[0] == '\0') {
                rafaelia_rmr_strlcpy(snapshot->kernel_release, uts.release,
                                     sizeof(snapshot->kernel_release));
            }
            if (snapshot->machine[0] == '\0') {
                rafaelia_rmr_strlcpy(snapshot->machine, uts.machine,
                                     sizeof(snapshot->machine));
            }
        }
    }
#endif

#if defined(CONFIG_LINUX)
    if (snapshot->uptime_seconds == 0 ||
        snapshot->total_ram_kib == 0 ||
        snapshot->free_ram_kib == 0) {
        if (sysinfo(&info) == 0) {
            if (snapshot->uptime_seconds == 0) {
                snapshot->uptime_seconds = (uint64_t)info.uptime;
            }
            if (snapshot->total_ram_kib == 0) {
                snapshot->total_ram_kib = ((uint64_t)info.totalram * info.mem_unit) / 1024u;
            }
            if (snapshot->free_ram_kib == 0) {
                snapshot->free_ram_kib = ((uint64_t)info.freeram * info.mem_unit) / 1024u;
            }
        }
    }

    snapshot->has_kvm_accel = access("/dev/kvm", R_OK | W_OK) == 0;
#else
    snapshot->has_kvm_accel = false;
#endif

    return true;
}
