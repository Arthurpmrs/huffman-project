#include "test_binary_tree.h"

// Test suite initialization function
int tbt_init_suite(void)
{
    return 0; // Return 0 on success
}

// Test suite cleanup function
int tbt_clean_suite(void)
{
    return 0; // Return 0 on success
}

void tbt_test_create_empty_binary_tree(void)
{
    binary_tree_t *bt = create_empty_binary_tree();

    CU_ASSERT_PTR_NULL(bt->left);
    CU_ASSERT_PTR_NULL(bt->right);
}

void tbt_test_reconstruct_tree(void)
{
    // Preorder tree: **CB***FEDA
    uint8_t preorder_tree[11];
    preorder_tree[0] = '*';
    preorder_tree[1] = '*';
    preorder_tree[2] = 'C';
    preorder_tree[3] = 'B';
    preorder_tree[4] = '*';
    preorder_tree[5] = '*';
    preorder_tree[6] = '*';
    preorder_tree[7] = 'F';
    preorder_tree[8] = 'E';
    preorder_tree[9] = 'D';
    preorder_tree[10] = 'A';

    binary_tree_t *bt = reconstruct_tree(preorder_tree);

    CU_ASSERT_EQUAL(*(uint8_t *)bt->item, '*');
    CU_ASSERT_EQUAL(*(uint8_t *)bt->right->item, '*');
    CU_ASSERT_EQUAL(*(uint8_t *)bt->left->left->item, 'C');
    CU_ASSERT_EQUAL(*(uint8_t *)bt->right->right->item, 'A');
    CU_ASSERT_EQUAL(*(uint8_t *)bt->right->left->right->item, 'D');
    CU_ASSERT_EQUAL(*(uint8_t *)bt->right->left->left->left->item, 'F');
}

void tbt_test_create_binary_tree(void)
{
    int value_1 = 12;
    int value_2 = 100;
    binary_tree_t *bt = create_binary_tree((void *)&value_1,
                                           create_binary_tree((void *)&value_2,
                                                              NULL,
                                                              NULL),
                                           NULL);

    CU_ASSERT_EQUAL(*(int *)bt->item, 12);

    CU_ASSERT_PTR_NULL_FATAL(bt->right);
    CU_ASSERT_PTR_NOT_NULL_FATAL(bt->left);

    CU_ASSERT_EQUAL(*(int *)bt->left->item, 100);
    CU_ASSERT_PTR_NULL(bt->left->right);
    CU_ASSERT_PTR_NULL(bt->left->left);
}
