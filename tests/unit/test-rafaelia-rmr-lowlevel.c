/*
 * RAFAELIA RMR lowlevel primitive tests
 */

#include "qemu/osdep.h"
#include "hw/core/rafaelia-rmr-lowlevel.h"

static void test_rafaelia_rmr_lowlevel_mem_primitives(void)
{
    uint8_t a[64];
    uint8_t b[64];

    memset(a, 0xAB, sizeof(a));
    memset(b, 0x00, sizeof(b));

    rafaelia_rmr_memzero(a, sizeof(a));
    g_assert_cmpint(memcmp(a, b, sizeof(a)), ==, 0);

    memset(a, 0x55, sizeof(a));
    rafaelia_rmr_memcpy(b, a, sizeof(a));
    g_assert_cmpint(rafaelia_rmr_memcmp(a, b, sizeof(a)), ==, 0);

    b[17] = 0x42;
    g_assert_cmpint(rafaelia_rmr_memcmp(a, b, sizeof(a)), >, 0);
}

static void test_rafaelia_rmr_lowlevel_string_primitives(void)
{
    char out[8];

    g_assert_cmpuint(rafaelia_rmr_strlen("RAFAELIA"), ==, 8);
    g_assert_cmpuint(rafaelia_rmr_strlen(NULL), ==, 0);

    memset(out, 0, sizeof(out));
    rafaelia_rmr_strlcpy(out, "ABCDEFGH123", sizeof(out));
    g_assert_cmpstr(out, ==, "ABCDEFG");

    out[0] = 'Z';
    rafaelia_rmr_strlcpy(out, NULL, sizeof(out));
    g_assert_cmpint(out[0], ==, 'Z');
}

static void test_rafaelia_rmr_lowlevel_rng_deterministic(void)
{
    uint32_t seq0[4];
    uint32_t seq1[4];
    int i;

    rafaelia_rmr_rng_seed(0x12345678u);
    for (i = 0; i < 4; i++) {
        seq0[i] = rafaelia_rmr_rng_next();
    }

    rafaelia_rmr_rng_seed(0x12345678u);
    for (i = 0; i < 4; i++) {
        seq1[i] = rafaelia_rmr_rng_next();
    }

    for (i = 0; i < 4; i++) {
        g_assert_cmpuint(seq0[i], ==, seq1[i]);
    }
}

int main(int argc, char **argv)
{
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/rafaelia/rmr-lowlevel/mem", test_rafaelia_rmr_lowlevel_mem_primitives);
    g_test_add_func("/rafaelia/rmr-lowlevel/str", test_rafaelia_rmr_lowlevel_string_primitives);
    g_test_add_func("/rafaelia/rmr-lowlevel/rng", test_rafaelia_rmr_lowlevel_rng_deterministic);

    return g_test_run();
}
