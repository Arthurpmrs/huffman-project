#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "test_binary_tree.h"

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

    // Run the tests
    CU_basic_run_tests();

    // Cleanup and return the test status
    CU_cleanup_registry();
    return CU_get_error();
}