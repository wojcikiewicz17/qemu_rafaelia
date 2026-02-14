/*
 * RAFAELIA runtime mode parser tests
 */

#include "qemu/osdep.h"
#include "hw/core/rafaelia-runtime.h"

static void test_runtime_mode_parse_numeric_valid(void)
{
    rafaelia_runtime_mode_t mode = RAFAELIA_RUNTIME_MODE_BENCH;

    g_assert_true(rafaelia_runtime_parse_mode("0", &mode));
    g_assert_cmpint(mode, ==, RAFAELIA_RUNTIME_MODE_SILENT);

    g_assert_true(rafaelia_runtime_parse_mode("5", &mode));
    g_assert_cmpint(mode, ==, RAFAELIA_RUNTIME_MODE_BENCH);
}

static void test_runtime_mode_parse_numeric_out_of_range(void)
{
    rafaelia_runtime_mode_t mode = RAFAELIA_RUNTIME_MODE_LOG;

    g_assert_false(rafaelia_runtime_parse_mode("999", &mode));
}

static void test_runtime_mode_parse_numeric_overflow(void)
{
    rafaelia_runtime_mode_t mode = RAFAELIA_RUNTIME_MODE_LOG;

    g_assert_false(rafaelia_runtime_parse_mode("18446744073709551616", &mode));
}

static void test_runtime_mode_parse_mixed_invalid(void)
{
    rafaelia_runtime_mode_t mode = RAFAELIA_RUNTIME_MODE_LOG;

    g_assert_false(rafaelia_runtime_parse_mode("5trace", &mode));
}

static void test_runtime_mode_parse_literal(void)
{
    rafaelia_runtime_mode_t mode = RAFAELIA_RUNTIME_MODE_SILENT;

    g_assert_true(rafaelia_runtime_parse_mode("symbiosis", &mode));
    g_assert_cmpint(mode, ==, RAFAELIA_RUNTIME_MODE_SYMBIOSIS);
}

int main(int argc, char **argv)
{
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/rafaelia/runtime-mode/parse-numeric-valid",
                    test_runtime_mode_parse_numeric_valid);
    g_test_add_func("/rafaelia/runtime-mode/parse-numeric-out-of-range",
                    test_runtime_mode_parse_numeric_out_of_range);
    g_test_add_func("/rafaelia/runtime-mode/parse-numeric-overflow",
                    test_runtime_mode_parse_numeric_overflow);
    g_test_add_func("/rafaelia/runtime-mode/parse-mixed-invalid",
                    test_runtime_mode_parse_mixed_invalid);
    g_test_add_func("/rafaelia/runtime-mode/parse-literal",
                    test_runtime_mode_parse_literal);

    return g_test_run();
}
