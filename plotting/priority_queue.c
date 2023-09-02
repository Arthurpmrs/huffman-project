#include "priority_queue.h"

priority_queue_t *pq_create(void (*print)(void *))
{
    priority_queue_t *new_pq = malloc(sizeof(priority_queue_t));
    new_pq->head = NULL;
    new_pq->tail = NULL;
    new_pq->size = 0;
    new_pq->print = print;
    return new_pq;
}

int pq_is_empty(priority_queue_t *pq)
{
    return pq->head == NULL;
}

void pq_enqueue(priority_queue_t *pq, void *item, uint32_t priority)
{
    node_t *new_node = malloc(sizeof(node_t));
    new_node->item = item;
    new_node->priority = priority;

    if (pq_is_empty(pq))
    {
        new_node->next = NULL;
        pq->head = new_node;
        pq->tail = new_node;
    }
    else
    {
        if (priority > pq->head->priority)
        {
            new_node->next = pq->head;
            pq->head = new_node;
        }
        else
        {
            node_t *current = pq->head;
            while (current->next != NULL &&
                   priority <= current->next->priority)
            {
                current = current->next;
            }
            new_node->next = current->next;
            current->next = new_node;
        }
    }
    pq->size += 1;
}

void *pq_dequeue(priority_queue_t *pq)
{
    if (pq_is_empty(pq))
    {
        printf("The queue is empty.\n");
        return NULL;
    }

    node_t *deletion_p = pq->head;
    void *item = pq->head->item;

    if (pq->head == pq->tail)
    {
        pq->head = NULL;
        pq->tail = NULL;
    }
    else
    {
        pq->head = pq->head->next;
    }

    free(deletion_p);
    pq->size -= 1;

    return item;
}

void pq_print(priority_queue_t *pq)
{
    if (pq_is_empty(pq))
    {
        printf("The queue is empty.\n");
        return;
    }

    node_t *current = pq->head;
    while (current != NULL)
    {

        pq->print(current->item);
        printf("(p=%d)", current->priority);
        if (current->next != NULL)
        {
            printf("->");
        }

        current = current->next;
    }
    printf("\n");
}