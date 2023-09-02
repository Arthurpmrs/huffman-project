#ifndef PLOTTING_PRIORITY_QUEUE_H_
#define PLOTTING_PRIORITY_QUEUE_H_

#include <stdio.h>
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

priority_queue_t *pq_create(void (*print)(void *));

int pq_is_empty(priority_queue_t *pq);

void pq_enqueue(priority_queue_t *pq, void *item,
                uint32_t priority, uint32_t *counter);

void *pq_dequeue(priority_queue_t *pq);

void pq_print(priority_queue_t *pq);

#endif