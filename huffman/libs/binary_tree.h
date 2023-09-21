#ifndef HUFFMAN_BINARY_TREE_H_
#define HUFFMAN_BINARY_TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct binary_tree binary_tree_t;
struct binary_tree
{
    void *item;
    binary_tree_t *left;
    binary_tree_t *right;
};

/**
 * @brief Creates an empty binary tree
 *
 * @return binary_tree_t* Pointer to the created binary tree
 */
binary_tree_t *create_empty_binary_tree();

/**
 * @brief Creates a binary tree
 *
 * @param item Item to be added to the binary tree
 * @param left Pointer to the left child of the binary tree
 * @param right Pointer to the right child of the binary tree
 * @return binary_tree_t* Pointer to the created binary tree
 */
binary_tree_t *create_binary_tree(void *item, binary_tree_t *left,
                                  binary_tree_t *right);

/**
 * @brief Reconstructs the Huffman tree from preorder string
 *
 * @param preorder_tree String holding the Huffman tree in preorder
 * @return binary_tree_t* Pointer to the root of the reconstructed huffman tree
 */
binary_tree_t *reconstruct_tree(uint8_t preorder_tree[]);

/**
 * @brief Prints the binary tree in preorder
 *
 * @param bt Pointer to the binary tree
 * @param print_fn Function to print an item of the binary tree
 */
void print_pre_order(binary_tree_t *bt, void (*print_fn)(void *));

#endif // HUFFMAN_BINARY_TREE_H_