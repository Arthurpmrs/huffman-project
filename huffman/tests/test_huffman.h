#ifndef TEST_HUFFMAN_
#define TEST_HUFFMAN_

#include <stdio.h>
#include <stdint.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "../libs/huffman.h"

typedef struct byte_info
{
    uint8_t byte;
    uint64_t frequency;
} byte_info_t;

int th_init_suite(void);

int th_clean_suite(void);

void th_test_huff_create_queue(void);

void th_test_huff_enqueue(void);

void th_test_huff_dequeue(void);

void th_test_huff_get_tree_from_queue(void);

void th_test_huff_get_tree_size(void);

#endif // TEST_HUFFMAN_