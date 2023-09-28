#ifndef HUFFMAN_STACK_H_
#define HUFFMAN_STACK_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "linked_list.h"

typedef struct stack_node stack_node_t;
struct stack_node
{
    void *item;
    stack_node_t *next;
};

typedef struct stack stack_t;
struct stack
{
    stack_node_t *top;
    uint32_t size;
    void (*print)(void *);
};

/**
 * Creates a new stack.
 *
 * @param print Function to print a stack item.
 * @return stack_t* Pointer to the new stack.
 */
stack_t *stack_create();

/**
 * Checks if a stack is empty.
 *
 * @param stack Pointer to a stack.
 * @return true if the stack is empty, false otherwise.
 */
bool stack_is_empty(stack_t *stack);

/**
 * Pushes an item to a stack.
 *
 * @param stack Pointer to a stack.
 * @param item The item to be pushed.
 */
void stack_push(stack_t *stack, void *item);

/**
 * Pops an item from a stack.
 *
 * @param stack Pointer to a stack.
 * @return void* Pointer to the popped item.
 */
void stack_pop(stack_t *stack);

/**
 * Retrieves the item from the top of a stack.
 *
 * @param stack Pointer to a stack.
 * @return void* Pointer to the item on the top of the stack.
 */
void *stack_peek(stack_t *stack);

/**
 * Prints a stack.
 *
 * @param stack Pointer to a stack.
 */
void stack_print(stack_t *stack);

/**
 * Pushes a uint8_t value onto a stack. Manually allocates space for the value.
 *
 * @param stack Pointer to a stack.
 * @param bit_value The value to be pushed.
 */
void stack_push_bit(stack_t *stack, uint8_t bit_value);

/**
 * Copies the items from a stack to a list.
 *
 * @param stack Pointer to a stack to copy from.
 * @param list Pointer to a list to copy to.
 */
void stack_copy_to_list(stack_t *stack, list_t *list);

#endif // HUFFMAN_STACK_H_