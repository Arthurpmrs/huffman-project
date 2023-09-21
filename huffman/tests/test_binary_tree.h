#ifndef TEST_BINARY_TREE_
#define TEST_BINARY_TREE_

#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../libs/binary_tree.h"

int tbt_init_suite(void);

int tbt_clean_suite(void);

void tbt_test_create_empty_binary_tree(void);

void tbt_test_reconstruct_tree(void);

void tbt_test_create_binary_tree(void);

#endif // TEST_BINARY_TREE_