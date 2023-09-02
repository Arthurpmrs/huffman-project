#include "heap.h"

heap_t *hp_create(size_t capacity, void (*print)(void *),
                  int (*compare)(void *, void *))
{
    void **data = malloc(capacity * sizeof(void *));

    if (data == NULL)
    {
        exit(EXIT_FAILURE);
    }

    heap_t *new_heap = malloc(sizeof(heap_t));
    new_heap->size = 0;
    new_heap->capacity = capacity;
    new_heap->data = data;
    new_heap->compare = compare;
    new_heap->print = print;
    return new_heap;
}

uint32_t hp_get_parent_index(heap_t *heap, uint32_t i)
{
    return i >> 1;
}

uint32_t hp_get_left_index(heap_t *heap, uint32_t i)
{
    return i << 1;
}

uint32_t hp_get_right_index(heap_t *heap, uint32_t i)
{
    return (i << 1) + 1;
}

void *hp_item_of(heap_t *heap, uint32_t i)
{
    return heap->data[i];
}

void hp_enqueue(heap_t *heap, void *item, uint32_t *counter)
{
    if (heap == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if (heap->size >= heap->capacity)
    {
        printf("Heap overflow.\n");
        return;
    }

    heap->size += 1;
    uint32_t new_item_index = heap->size;
    heap->data[new_item_index] = item;

    uint32_t parent_index = hp_get_parent_index(heap, heap->size);

    while (parent_index >= 1 &&
           heap->compare(heap->data[new_item_index], heap->data[parent_index]) > 0)
    {
        *counter += 1;
        swap(&heap->data[new_item_index], &heap->data[parent_index]);
        new_item_index = parent_index;
        parent_index = hp_get_parent_index(heap, new_item_index);
    }
}

void hp_max_heapify(heap_t *heap, uint32_t origin)
{
    if (heap == NULL)
    {
        exit(EXIT_FAILURE);
    }

    uint32_t largest;
    uint32_t left_index = hp_get_left_index(heap, origin);
    uint32_t right_index = hp_get_right_index(heap, origin);

    if (left_index <= heap->size &&
        heap->compare(heap->data[left_index], heap->data[origin]) > 0)
    {
        largest = left_index;
    }
    else
    {
        largest = origin;
    }

    if (right_index <= heap->size &&
        heap->compare(heap->data[right_index], heap->data[largest]) > 0)
    {
        largest = right_index;
    }

    if (heap->compare(heap->data[origin], heap->data[largest]) != 0)
    {
        swap(&heap->data[origin], &heap->data[largest]);
        hp_max_heapify(heap, largest);
    }
}

void *hp_dequeue(heap_t *heap)
{
    if (heap == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if (heap->size == 0)
    {
        printf("Heap underflow.\n");
        return NULL;
    }

    void *item = heap->data[1];
    heap->data[1] = heap->data[heap->size];
    heap->size -= 1;

    // from root
    hp_max_heapify(heap, 1);

    return item;
}

void hp_print(heap_t *heap)
{
    for (int i = 1; i < heap->size + 1; i++)
    {
        heap->print(heap->data[i]);
        printf(" ");
    }
    printf("\n");
}