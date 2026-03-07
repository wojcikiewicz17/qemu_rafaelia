/*
 * RAFAELIA RMR instrument snapshot tests
 */

#include "qemu/osdep.h"
#include "hw/core/rafaelia-rmr.h"
#include "hw/core/rafaelia-rmr-lowlevel.h"

static void test_rafaelia_rmr_collect_instruments_basics(void)
{
    rafaelia_rmr_instrument_snapshot_t snap;
    bool ok = rafaelia_rmr_collect_instruments(&snap);

    g_assert_true(ok);
    g_assert_nonnull(snap.arch);
    g_assert_nonnull(snap.os);
    g_assert_cmpuint(snap.pointer_bits, >, 0);
    g_assert_cmpuint(snap.page_bytes, >, 0);
}

static void test_rafaelia_rmr_collect_instruments_idempotent(void)
{
    rafaelia_rmr_instrument_snapshot_t first;
    rafaelia_rmr_instrument_snapshot_t second;

    memset(&first, 0xA5, sizeof(first));
    memset(&second, 0x5A, sizeof(second));

    g_assert_true(rafaelia_rmr_collect_instruments(&first));
    g_assert_true(rafaelia_rmr_collect_instruments(&second));

    g_assert_cmpstr(second.arch, ==, first.arch);
    g_assert_cmpstr(second.os, ==, first.os);
    g_assert_cmpuint(second.pointer_bits, ==, first.pointer_bits);
    g_assert_cmpuint(second.page_bytes, ==, first.page_bytes);
    g_assert_cmpuint(second.cpu_online, ==, first.cpu_online);
}

static void test_rafaelia_rmr_collect_instruments_invariants(void)
{
    rafaelia_rmr_instrument_snapshot_t snap;

    g_assert_true(rafaelia_rmr_collect_instruments(&snap));

    g_assert_true(snap.pointer_bits == 32 || snap.pointer_bits == 64);
    g_assert_cmpuint(snap.page_bytes, >, 0);

    if (snap.total_ram_kib > 0 && snap.free_ram_kib > 0) {
        g_assert_cmpuint(snap.free_ram_kib, <=, snap.total_ram_kib);
    }
}

static void test_rafaelia_rmr_snapshot_string_fields_nul_terminated(void)
{
    rafaelia_rmr_instrument_snapshot_t snap;
    const char long_kernel[] =
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
    const char long_machine[] =
        "cccccccccccccccccccccccccccccccc"
        "dddddddddddddddddddddddddddddddd";

    memset(&snap, 0, sizeof(snap));

    rafaelia_rmr_strlcpy(snap.kernel_release, long_kernel,
                         sizeof(snap.kernel_release));
    rafaelia_rmr_strlcpy(snap.machine, long_machine, sizeof(snap.machine));

    g_assert_cmpint(snap.kernel_release[sizeof(snap.kernel_release) - 1], ==, '\0');
    g_assert_cmpint(snap.machine[sizeof(snap.machine) - 1], ==, '\0');
    g_assert_cmpuint(strlen(snap.kernel_release), ==,
                     sizeof(snap.kernel_release) - 1);
    g_assert_cmpuint(strlen(snap.machine), ==, sizeof(snap.machine) - 1);
}

static void test_rafaelia_rmr_collect_instruments_no_linux_kvm(void)
{
    rafaelia_rmr_instrument_snapshot_t snap;

    g_assert_true(rafaelia_rmr_collect_instruments(&snap));

#if !defined(CONFIG_LINUX)
    g_assert_false(snap.has_kvm_accel);
#else
    if (access("/dev/kvm", R_OK | W_OK) != 0) {
        g_assert_false(snap.has_kvm_accel);
    } else {
        g_test_skip("Linux com /dev/kvm acessível: caso não se aplica");
    }
#endif
}

static void test_rafaelia_rmr_collect_instruments_null(void)
{
    g_assert_false(rafaelia_rmr_collect_instruments(NULL));
}

int main(int argc, char **argv)
{
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/rafaelia/rmr-instruments/collect-basics",
                    test_rafaelia_rmr_collect_instruments_basics);
    g_test_add_func("/rafaelia/rmr-instruments/collect-idempotent",
                    test_rafaelia_rmr_collect_instruments_idempotent);
    g_test_add_func("/rafaelia/rmr-instruments/collect-invariants",
                    test_rafaelia_rmr_collect_instruments_invariants);
    g_test_add_func("/rafaelia/rmr-instruments/string-fields-nul-terminated",
                    test_rafaelia_rmr_snapshot_string_fields_nul_terminated);
    g_test_add_func("/rafaelia/rmr-instruments/no-linux-kvm",
                    test_rafaelia_rmr_collect_instruments_no_linux_kvm);
    g_test_add_func("/rafaelia/rmr-instruments/collect-null",
                    test_rafaelia_rmr_collect_instruments_null);

    return g_test_run();
}
