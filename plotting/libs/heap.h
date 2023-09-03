#ifndef PLOTTING_HEAP_H_
#define PLOTTING_HEAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "utils.h"

typedef struct heap
{
    size_t size;
    size_t capacity;
    void **data;
    void (*print)(void *);
    int (*compare)(void *, void *);
} heap_t;

heap_t *hp_create(size_t capacity, void (*print)(void *),
                  int (*compare)(void *, void *));

uint32_t hp_get_parent_index(heap_t *heap, uint32_t i);

uint32_t hp_get_left_index(heap_t *heap, uint32_t i);

uint32_t hp_get_right_index(heap_t *heap, uint32_t i);

void *hp_item_of(heap_t *heap, uint32_t i);

void hp_enqueue(heap_t *heap, void *item, uint32_t *counter);

void hp_max_heapify(heap_t *heap, uint32_t origin);

void *hp_dequeue(heap_t *heap);

void hp_print(heap_t *heap);

void *hp_clear(heap_t *heap);

#endif