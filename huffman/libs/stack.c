#include "stack.h"

stack_t *stack_create()
{
    stack_t *stack = malloc(sizeof(stack_t));
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

bool stack_is_empty(stack_t *stack)
{
    return stack->size == 0;
}

void stack_push(stack_t *stack, void *item)
{
    stack_node_t *node = malloc(sizeof(stack_node_t));
    node->item = item;
    node->next = stack->top;
    stack->top = node;
    stack->size++;
}

void stack_pop(stack_t *stack)
{
    if (stack_is_empty(stack))
    {
        return;
    }

    stack_node_t *node = stack->top;
    stack->top = node->next;
    free(node);
    stack->size--;
}

void *stack_peek(stack_t *stack)
{
    if (stack_is_empty(stack))
    {
        return NULL;
    }

    return stack->top->item;
}
