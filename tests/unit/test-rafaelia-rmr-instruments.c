/*
 * RAFAELIA RMR instrument snapshot tests
 */

#include "qemu/osdep.h"
#include "hw/core/rafaelia-rmr.h"

static void test_rafaelia_rmr_collect_instruments_basics(void)
{
    rafaelia_rmr_instrument_snapshot_t snap;
    bool ok = rafaelia_rmr_collect_instruments(&snap);

    g_assert_true(ok);
    g_assert_nonnull(snap.arch);
    g_assert_nonnull(snap.os);
    g_assert_cmpuint(snap.pointer_bits, >, 0);
    g_assert_cmpuint(snap.page_bytes, >, 0);

    if (snap.total_ram_kib > 0 && snap.free_ram_kib > 0) {
        g_assert_cmpuint(snap.total_ram_kib, >=, snap.free_ram_kib);
    }
}

static void test_rafaelia_rmr_collect_instruments_null(void)
{
    g_assert_false(rafaelia_rmr_collect_instruments(NULL));
}

static void test_rafaelia_rmr_route_select_null(void)
{
    rafaelia_rmr_instrument_snapshot_t snap;
    rafaelia_rmr_route_decision_t decision;

    g_assert_false(rafaelia_rmr_route_select(NULL, &decision));
    g_assert_false(rafaelia_rmr_route_select(&snap, NULL));
}

static void test_rafaelia_rmr_route_select_deterministic(void)
{
    rafaelia_rmr_instrument_snapshot_t snap;
    rafaelia_rmr_route_decision_t d0;
    rafaelia_rmr_route_decision_t d1;

    memset(&snap, 0, sizeof(snap));
    snap.arch = "x86_64";
    snap.os = "linux";
    snap.pointer_bits = 64;
    snap.cpu_online = 16;
    snap.page_bytes = 4096;
    snap.total_ram_kib = 64u * 1024u * 1024u;
    snap.free_ram_kib = 8u * 1024u * 1024u;
    snap.has_kvm_accel = true;

    g_assert_true(rafaelia_rmr_route_select(&snap, &d0));
    g_assert_true(rafaelia_rmr_route_select(&snap, &d1));

    g_assert_cmpuint(d0.route, ==, RAFAELIA_RMR_ROUTE_KVM_ACCEL);
    g_assert_cmpuint(d0.lane_id, ==, 3);
    g_assert_cmpuint(d0.route_score, ==, d1.route_score);
    g_assert_cmpuint(d0.route, ==, d1.route);
    g_assert_cmpuint(d0.lane_id, ==, d1.lane_id);
}

int main(int argc, char **argv)
{
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/rafaelia/rmr-instruments/collect-basics",
                    test_rafaelia_rmr_collect_instruments_basics);
    g_test_add_func("/rafaelia/rmr-instruments/collect-null",
                    test_rafaelia_rmr_collect_instruments_null);
    g_test_add_func("/rafaelia/rmr-instruments/route-null",
                    test_rafaelia_rmr_route_select_null);
    g_test_add_func("/rafaelia/rmr-instruments/route-deterministic",
                    test_rafaelia_rmr_route_select_deterministic);

    return g_test_run();
}
