#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "utils.c"

typedef struct node node_t;
struct node
{
    void *item;
    uint32_t priority;
    struct node *next;
};

typedef struct priority_queue
{
    node_t *head;
    node_t *tail;
    size_t size;
    void (*print)(void *);
} priority_queue_t;

priority_queue_t *create_priority_queue(void (*print)(void *))
{
    priority_queue_t *new_pq = malloc(sizeof(priority_queue_t));
    new_pq->head = NULL;
    new_pq->tail = NULL;
    new_pq->size = 0;
    new_pq->print = print;
    return new_pq;
}

int is_empty(priority_queue_t *pq)
{
    return pq->head == NULL;
}

void enqueue(priority_queue_t *pq, void *item, uint32_t priority)
{
    node_t *new_node = malloc(sizeof(node_t));
    new_node->item = item;
    new_node->priority = priority;

    if (is_empty(pq))
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

void *dequeue(priority_queue_t *pq)
{
    if (is_empty(pq))
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

void print_queue(priority_queue_t *pq)
{
    if (is_empty(pq))
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

int main(void)
{
    priority_queue_t *pq = create_priority_queue(print_int);

    int values[10] = {45, 87, 96, 78, 12, 1, 0, 47, 3, 5};
    int priorities[10] = {5, 98, 15, 98, 45, 12, 13, 77, 42, 7};
    for (int i = 0; i < 10; i++)
    {
        enqueue(pq, (void *)&values[i], priorities[i]);
    }

    int newValue = 9999;
    int newValue2 = 7777;
    enqueue(pq, (void *)&newValue, 1);
    enqueue(pq, (void *)&newValue2, 100);
    print_queue(pq);
    printf("size: %ld\n", pq->size);

    dequeue(pq);
    dequeue(pq);
    print_queue(pq);
    printf("size: %ld\n", pq->size);
    return 0;
}