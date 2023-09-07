#include "binary_tree.h"

binary_tree_t *create_empty_binary_tree()
{
    binary_tree_t *bt = malloc(sizeof(binary_tree_t));
    bt->left = NULL;
    bt->right = NULL;
    return bt;
}

binary_tree_t *create_binary_tree(void *item, binary_tree_t *left, binary_tree_t *right)
{
    binary_tree_t *bt = create_empty_binary_tree();
    bt->left = left;
    bt->right = right;
    bt->item = item;
    return bt;
}

void print_pre_order(binary_tree_t *bt, void (*print_fn)(void *))
{
    if (bt != NULL)
    {
        print_fn(bt->item);
        print_pre_order(bt->left, print_fn);
        print_pre_order(bt->right, print_fn);
    }
}