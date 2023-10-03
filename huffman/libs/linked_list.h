#ifndef HUFFMAN_LINKED_LIST_H_
#define HUFFMAN_LINKED_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct list_node list_node_t;
struct list_node
{
    void *item;
    list_node_t *next;
};

typedef struct list list_t;
struct list
{
    list_node_t *head;
    uint64_t size;
};

/**
 * Creates a new linked list.
 *
 * @param print Function to print a list item.
 * @return list_t* Pointer to the new linked list.
 */
list_t *list_create();

/**
 * Checks if a linked list is empty.
 *
 * @param list Pointer to a linked list.
 */
bool list_is_empty(list_t *list);

/**
 * Adds an item to the head of a linked list.
 *
 * @param list Pointer to a linked list.
 * @param item The item to be added.
 */
void list_add_to_head(list_t *list, void *item);

#endif // HUFFMAN_LINKED_LIST_H_