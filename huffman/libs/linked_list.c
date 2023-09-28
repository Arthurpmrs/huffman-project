#include "linked_list.h"

list_t *list_create()
{
    list_t *new_list = malloc(sizeof(list_t));
    new_list->head = NULL;
    new_list->size = 0;
    return new_list;
}

bool list_is_empty(list_t *list)
{
    return list->size == 0;
}

void list_add_to_head(list_t *list, void *item)
{
    list_node_t *new_node = malloc(sizeof(list_node_t));
    new_node->item = item;
    new_node->next = list->head;

    list->head = new_node;

    list->size += 1;
}

// TODO: remove
void list_print(list_t *list)
{
    printf("List: ");
    list_node_t *current_node = list->head;
    while (current_node != NULL)
    {
        printf("%d", *(int *)current_node->item);
        current_node = current_node->next;
    }
    printf("\n");
}