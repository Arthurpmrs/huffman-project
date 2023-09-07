#ifndef HUFFMAN_BINARY_TREE_H_
#define HUFFMAN_BINARY_TREE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct binary_tree binary_tree_t;
struct binary_tree
{
    void *item;
    binary_tree_t *left;
    binary_tree_t *right;
};

binary_tree_t *create_empty_binary_tree();

binary_tree_t *create_binary_tree(void *item, binary_tree_t *left, binary_tree_t *right);

void print_pre_order(binary_tree_t *bt, void (*print_fn)(void *));

#endif // HUFFMAN_BINARY_TREE_H_