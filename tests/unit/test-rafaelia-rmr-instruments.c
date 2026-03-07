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
    g_test_add_func("/rafaelia/rmr-instruments/collect-null",
                    test_rafaelia_rmr_collect_instruments_null);

    return g_test_run();
}
