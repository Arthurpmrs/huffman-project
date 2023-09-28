#ifndef PLOTTING_HEAP_H_
#define PLOTTING_HEAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct huff_node huff_node_t;
struct huff_node
{
    void *byte;
    uint64_t frequency;
    huff_node_t *next;
    huff_node_t *left;
    huff_node_t *right;
};

typedef struct huff_queue huff_queue_t;
struct huff_queue
{
    uint16_t size;
    huff_node_t *head;
};

/**
 * Creates a new huffman queue.
 *
 * @return huff_queue_t* Pointer to the created huffman queue.
 */
huff_queue_t *huff_create_queue();

/**
 * Enqueues a new byte into a huffman queue.
 *
 * @param hq Pointer to a huffman queue.
 * @param byte The byte to enqueue.
 * @param frequency The frequency of the byte.
 * @param left The left child of the node.
 * @param right The right child of the node.
 */
void huff_enqueue(huff_queue_t *hq, void *byte, uint64_t frequency,
                  huff_node_t *left, huff_node_t *right);

/**
 * Dequeues a node from a huffman queue.
 *
 * @param hq Pointer to a huffman queue.
 * @return huff_node_t* Pointer to the dequeued node.
 */
huff_node_t *huff_dequeue(huff_queue_t *hq);

/**
 * Generates a huffman tree from a huffman queue.
 *
 * @param hq Pointer to a huffman queue.
 * @return huff_node_t* Pointer to the root of the huffman tree
 */
huff_node_t *huff_get_tree_from_queue(huff_queue_t *hq);

/**
 * Calculates the size of a huffman tree.
 *
 * @param ht Pointer to the root of a huffman tree.
 * @return uint16_t Size of the huffman tree.
 */
uint16_t huff_get_tree_size(huff_node_t *ht);

/**
 * Writes a huffman tree to a file.
 *
 * @param ht Pointer to the root of a huffman tree.
 * @param output Pointer to the output file.
 */
void huff_write_tree_to_file(huff_node_t *ht, FILE *output);

/**
 * Prints the huffman tree in preorder.
 *
 * @param ht Pointer to the root of a huffman tree.
 */
void huff_print_tree(huff_node_t *ht);

/**
 * Checks if a node is a special node.
 *
 * @param node Pointer to a huffman node.
 * @return true if the node is a special node.
 * @return false if the node is not a special node.
 */
bool is_special_node(huff_node_t *node);
#endif