#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "test_binary_tree.h"
#include "test_huffman.h"
#include "test_linked_list.h"

CU_ErrorCode tbt_setup_test(void)
{
    CU_pSuite suite = CU_add_suite("Binary Tree Test", tbt_init_suite,
                                   tbt_clean_suite);
    if (suite == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add the test case to the suite
    if (CU_add_test(suite, "1", tbt_test_create_empty_binary_tree) == NULL ||
        CU_add_test(suite, "2", tbt_test_create_binary_tree) == NULL ||
        CU_add_test(suite, "3", tbt_test_reconstruct_tree) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    return CUE_SUCCESS;
}

CU_ErrorCode th_setup_test(void)
{
    CU_pSuite suite = CU_add_suite("Huffman Test", th_init_suite,
                                   th_clean_suite);
    if (suite == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add the test case to the suite
    if (CU_add_test(suite, "1", th_test_huff_create_queue) == NULL ||
        CU_add_test(suite, "2", th_test_huff_enqueue) == NULL ||
        CU_add_test(suite, "3", th_test_huff_dequeue) == NULL ||
        CU_add_test(suite, "4", th_test_huff_get_tree_from_queue) == NULL ||
        CU_add_test(suite, "5", th_test_huff_get_tree_size) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    return CUE_SUCCESS;
}

CU_ErrorCode tll_setup_test(void)
{
    CU_pSuite suite = CU_add_suite("Linked List Test", tll_init_suite,
                                   tll_clean_suite);
    if (suite == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add the test case to the suite
    if (CU_add_test(suite, "1", tll_test_create_empty_linked_list) == NULL ||
        CU_add_test(suite, "2", tll_test_list_is_empty) == NULL ||
        CU_add_test(suite, "3", tll_test_list_add_to_head) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    return CUE_SUCCESS;
}

int main()
{
    // Initialize the CUnit test registry
    if (CU_initialize_registry() != CUE_SUCCESS)
    {
        printf("Initialization of CUnit Test Registry failed.");
        return CU_get_error();
    }

    if (tbt_setup_test() != CUE_SUCCESS)
    {
        printf("Setup of Binary Tree Test Suite failed.");
        return CU_get_error();
    }

    if (th_setup_test() != CUE_SUCCESS)
    {
        printf("Setup of Huffman Test Suite failed.");
        return CU_get_error();
    }

    if (tll_setup_test() != CUE_SUCCESS)
    {
        printf("Setup of Linked List Test Suite failed.");
        return CU_get_error();
    }

    // Run the tests
    CU_basic_run_tests();

    // Cleanup and return the test status
    CU_cleanup_registry();
    return CU_get_error();
}