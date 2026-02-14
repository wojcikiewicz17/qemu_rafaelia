/*
 * RAFAELIA hyper stack copy boundaries
 */

#include "qemu/osdep.h"
#include "hw/core/rafaelia-core.h"

static void test_hyper_stack_copy_len_size_minus_one(void)
{
    rafaelia_core_t core;
    static uint8_t payload[RAFAELIA_STACK_SIZE_HYPER - 1];
    bool copied;

    memset(&core, 0, sizeof(core));
    memset(payload, 0xA5, sizeof(payload));

    copied = rafaelia_hyper_stack_copy_binary(&core, payload, sizeof(payload));

    g_assert_true(copied);
    g_assert_cmpuint(core.stack_ptr, ==, sizeof(payload));
    g_assert_cmpmem(core.hyper_stack, sizeof(payload), payload, sizeof(payload));
}

static void test_hyper_stack_copy_len_size(void)
{
    rafaelia_core_t core;
    static uint8_t payload[RAFAELIA_STACK_SIZE_HYPER];
    bool copied;

    memset(&core, 0, sizeof(core));
    memset(payload, 0x5A, sizeof(payload));

    copied = rafaelia_hyper_stack_copy_binary(&core, payload, sizeof(payload));

    g_assert_true(copied);
    g_assert_cmpuint(core.stack_ptr, ==, sizeof(payload));
    g_assert_cmpmem(core.hyper_stack, sizeof(payload), payload, sizeof(payload));
}

static void test_hyper_stack_copy_len_size_plus_one(void)
{
    rafaelia_core_t core;
    static uint8_t payload[RAFAELIA_STACK_SIZE_HYPER + 1];
    uint8_t original_stack[16];
    bool copied;

    memset(&core, 0x3C, sizeof(core));
    core.stack_ptr = 7;
    memcpy(original_stack, core.hyper_stack, sizeof(original_stack));
    memset(payload, 0xC3, sizeof(payload));

    copied = rafaelia_hyper_stack_copy_binary(&core, payload, sizeof(payload));

    g_assert_false(copied);
    g_assert_cmpuint(core.stack_ptr, ==, 7);
    g_assert_cmpmem(core.hyper_stack, sizeof(original_stack),
                    original_stack, sizeof(original_stack));
}

int main(int argc, char **argv)
{
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/rafaelia/hyper-stack/len-size-minus-one",
                    test_hyper_stack_copy_len_size_minus_one);
    g_test_add_func("/rafaelia/hyper-stack/len-size",
                    test_hyper_stack_copy_len_size);
    g_test_add_func("/rafaelia/hyper-stack/len-size-plus-one",
                    test_hyper_stack_copy_len_size_plus_one);

    return g_test_run();
}
