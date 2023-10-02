#include "test_linked_list.h"

void tll_print_int(void *item)
{
    printf("%d ", *(int *)item);
}

// Test suite initialization function
int tll_init_suite(void)
{
    return 0; // Return 0 on success
}

// Test suite cleanup function
int tll_clean_suite(void)
{
    return 0; // Return 0 on success
}

void tll_test_create_empty_linked_list(void)
{
    list_t *list = list_create(tll_print_int);

    CU_ASSERT_PTR_NOT_NULL_FATAL(list)
    CU_ASSERT_PTR_NULL(list->head);
    CU_ASSERT_EQUAL(list->size, 0);
}

void tll_test_list_is_empty(void)
{
    list_t *list = list_create(tll_print_int);

    CU_ASSERT_TRUE(list_is_empty(list));
}

void tll_test_list_add_to_head(void)
{
    list_t *list = list_create(tll_print_int);

    int value = 12;
    list_add_to_head(list, (void *)&value);

    CU_ASSERT_EQUAL(*(int *)list->head->item, 12);
    CU_ASSERT_EQUAL(list->size, 1);

    int value_2 = 100;
    list_add_to_head(list, (void *)&value_2);

    CU_ASSERT_EQUAL(*(int *)list->head->item, 100);
    CU_ASSERT_EQUAL(list->size, 2);
}
