#include "test_huffman.h"

byte_info_t bytes[6];

int th_init_suite(void)
{
    return 0;
}

int th_clean_suite(void)
{
    return 0;
}

void th_test_huff_create_queue(void)
{
    huff_queue_t *hq = huff_create_queue();
    CU_ASSERT_PTR_NOT_NULL_FATAL(hq);
    CU_ASSERT_EQUAL(hq->size, 0);
    CU_ASSERT_PTR_NULL(hq->head);
}

void th_test_huff_enqueue(void)
{
    huff_queue_t *hq = huff_create_queue();
    uint8_t byte_1 = 'A';
    uint8_t byte_2 = 'Z';
    huff_enqueue(hq, (void *)&byte_1, 100, NULL, NULL);
    huff_enqueue(hq, (void *)&byte_2, 3, NULL, NULL);
    CU_ASSERT_EQUAL(hq->size, 2);

    CU_ASSERT_EQUAL(*(uint8_t *)hq->head->byte, 'Z');
    CU_ASSERT_EQUAL(hq->head->frequency, 3);
}

void th_test_huff_dequeue(void)
{
    huff_queue_t *hq = huff_create_queue();
    uint8_t byte_1 = 'A';
    uint8_t byte_2 = 'Z';
    huff_enqueue(hq, (void *)&byte_1, 100, NULL, NULL);
    huff_enqueue(hq, (void *)&byte_2, 3, NULL, NULL);
    CU_ASSERT_EQUAL(hq->size, 2);

    huff_node_t *hn_1 = huff_dequeue(hq);
    CU_ASSERT_EQUAL(hq->size, 1);
    CU_ASSERT_EQUAL(*(uint8_t *)hn_1->byte, 'Z');
    CU_ASSERT_EQUAL(hn_1->frequency, 3);

    huff_node_t *hn_2 = huff_dequeue(hq);
    CU_ASSERT_EQUAL(hq->size, 0);
    CU_ASSERT_EQUAL(*(uint8_t *)hn_2->byte, 'A');
    CU_ASSERT_EQUAL(hn_2->frequency, 100);

    CU_ASSERT_PTR_NULL(hq->head);
}

void _setup_huff_tree(huff_queue_t *hq, huff_node_t **ht)
{
    bytes[0] = (byte_info_t){.byte = 'A', .frequency = 6};
    bytes[1] = (byte_info_t){.byte = 'B', .frequency = 5};
    bytes[2] = (byte_info_t){.byte = 'C', .frequency = 4};
    bytes[3] = (byte_info_t){.byte = 'D', .frequency = 3};
    bytes[4] = (byte_info_t){.byte = 'E', .frequency = 2};
    bytes[5] = (byte_info_t){.byte = 'F', .frequency = 1};

    for (int i = 0; i < 6; i++)
    {
        huff_enqueue(hq, (void *)&bytes[i].byte, bytes[i].frequency, NULL, NULL);
    }

    *ht = huff_get_tree_from_queue(hq);
}

void th_test_huff_get_tree_from_queue(void)
{
    huff_queue_t *hq = huff_create_queue();
    huff_node_t *ht = NULL;

    _setup_huff_tree(hq, &ht);

    CU_ASSERT_EQUAL(*(uint8_t *)ht->byte, '*');
    CU_ASSERT_EQUAL(*(uint8_t *)ht->right->byte, '*');
    CU_ASSERT_EQUAL(*(uint8_t *)ht->left->left->byte, 'C');
    CU_ASSERT_EQUAL(*(uint8_t *)ht->right->right->byte, 'A');
    CU_ASSERT_EQUAL(*(uint8_t *)ht->right->left->right->byte, 'D');
    CU_ASSERT_EQUAL(*(uint8_t *)ht->right->left->left->left->byte, 'F');
}

void th_test_huff_get_tree_size(void)
{
    huff_queue_t *hq = huff_create_queue();
    huff_node_t *ht = NULL;

    _setup_huff_tree(hq, &ht);

    CU_ASSERT_EQUAL(huff_get_tree_size(ht), 11);
}