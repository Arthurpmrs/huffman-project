#ifndef TEST_LINKED_LIST_
#define TEST_LINKED_LIST_

#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../libs/linked_list.h"

int tll_init_suite(void);

int tll_clean_suite(void);

void tll_test_create_empty_linked_list(void);

void tll_test_list_is_empty(void);

void tll_test_list_add_to_head(void);

#endif // TEST_LINKED_LIST_