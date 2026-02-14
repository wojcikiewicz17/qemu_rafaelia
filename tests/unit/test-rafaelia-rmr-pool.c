/*
 * RAFAELIA RMR pool tests
 */

#include "qemu/osdep.h"
#include "hw/core/rafaelia-rmr.h"

static void test_rafaelia_rmr_pool_free_valid_pointer(void)
{
    rafaelia_rmr_pool_t *pool;
    void *entry;

    pool = rafaelia_rmr_pool_create(sizeof(uint64_t), 4, 0);
    g_assert_nonnull(pool);

    entry = rafaelia_rmr_pool_alloc_uninitialized(pool);
    g_assert_nonnull(entry);
    g_assert_cmpuint(pool->in_use, ==, 1);

    rafaelia_rmr_pool_free(pool, entry);
    g_assert_cmpuint(pool->in_use, ==, 0);
    g_assert_true(pool->free_list == entry);

    rafaelia_rmr_pool_destroy(pool);
}

static void test_rafaelia_rmr_pool_free_external_pointer(void)
{
    rafaelia_rmr_pool_t *pool;
    void *entry;
    void *free_list_before;
    uint32_t in_use_before;
    uint64_t external = 0;

    pool = rafaelia_rmr_pool_create(sizeof(uint64_t), 4, 0);
    g_assert_nonnull(pool);

    entry = rafaelia_rmr_pool_alloc_uninitialized(pool);
    g_assert_nonnull(entry);

    free_list_before = pool->free_list;
    in_use_before = pool->in_use;

    rafaelia_rmr_pool_free(pool, &external);
    g_assert_true(pool->free_list == free_list_before);
    g_assert_cmpuint(pool->in_use, ==, in_use_before);

    rafaelia_rmr_pool_free(pool, entry);
    g_assert_cmpuint(pool->in_use, ==, 0);

    rafaelia_rmr_pool_destroy(pool);
}

int main(int argc, char **argv)
{
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/rafaelia/rmr-pool/free-valid-pointer",
                    test_rafaelia_rmr_pool_free_valid_pointer);
    g_test_add_func("/rafaelia/rmr-pool/free-external-pointer",
                    test_rafaelia_rmr_pool_free_external_pointer);

    return g_test_run();
}
