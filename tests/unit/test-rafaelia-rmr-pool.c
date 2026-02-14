/*
 * RAFAELIA RMR pool allocator defensive validation tests
 */

#include "qemu/osdep.h"
#include "hw/core/rafaelia-rmr.h"

static void *failing_memalign(size_t alignment, size_t size)
{
    (void)alignment;
    (void)size;
    return NULL;
}

static void test_pool_create_normal_parameters(void)
{
    rafaelia_rmr_pool_t *pool = rafaelia_rmr_pool_create(sizeof(uint64_t), 16, 64);
    void *ptr1;
    void *ptr2;

    g_assert_nonnull(pool);
    g_assert_nonnull(pool->buffer);
    g_assert_cmpuint(pool->capacity, ==, 16);
    g_assert_cmpuint(pool->in_use, ==, 0);

    ptr1 = rafaelia_rmr_pool_alloc_uninitialized(pool);
    ptr2 = rafaelia_rmr_pool_alloc_uninitialized(pool);
    g_assert_nonnull(ptr1);
    g_assert_nonnull(ptr2);
    g_assert_cmpuint(pool->in_use, ==, 2);

    rafaelia_rmr_pool_free(pool, ptr1);
    rafaelia_rmr_pool_free(pool, ptr2);
    g_assert_cmpuint(pool->in_use, ==, 0);

    rafaelia_rmr_pool_destroy(pool);
}

static void test_pool_create_overflow_capacity(void)
{
    g_assert_null(rafaelia_rmr_pool_create(SIZE_MAX, 2, 1));
}

static void test_pool_create_allocation_failure(void)
{
    rafaelia_rmr_pool_set_memalign_for_test(failing_memalign);
    g_assert_null(rafaelia_rmr_pool_create(sizeof(uint64_t), 16, 64));
    rafaelia_rmr_pool_reset_memalign_for_test();
}

int main(int argc, char **argv)
{
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/rafaelia/rmr-pool/create-normal-parameters",
                    test_pool_create_normal_parameters);
    g_test_add_func("/rafaelia/rmr-pool/create-overflow-capacity",
                    test_pool_create_overflow_capacity);
    g_test_add_func("/rafaelia/rmr-pool/create-allocation-failure",
                    test_pool_create_allocation_failure);

    return g_test_run();
}
