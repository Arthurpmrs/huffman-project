#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "utils.c"

typedef struct heap
{
    size_t size;
    size_t capacity;
    void **data;
    void (*print)(void *);
    int (*compare)(void *, void *);
} heap_t;

void swap(void **a, void **b)
{
    void *tmp = *a;
    *a = *b;
    *b = tmp;
}

heap_t *create_heap(size_t capacity, void (*print)(void *),
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

uint32_t get_parent_index(heap_t *heap, uint32_t i)
{
    return i >> 1;
}

uint32_t get_left_index(heap_t *heap, uint32_t i)
{
    return i << 1;
}

uint32_t get_right_index(heap_t *heap, uint32_t i)
{
    return (i << 1) + 1;
}

void *item_of(heap_t *heap, uint32_t i)
{
    return heap->data[i];
}

void enqueue(heap_t *heap, void *item)
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

    uint32_t parent_index = get_parent_index(heap, heap->size);

    while (parent_index >= 1 &&
           heap->compare(heap->data[new_item_index], heap->data[parent_index]) > 0)
    {
        swap(&heap->data[new_item_index], &heap->data[parent_index]);
        new_item_index = parent_index;
        parent_index = get_parent_index(heap, new_item_index);
    }
}

void max_heapify(heap_t *heap, uint32_t origin)
{
    if (heap == NULL)
    {
        exit(EXIT_FAILURE);
    }

    uint32_t largest;
    uint32_t left_index = get_left_index(heap, origin);
    uint32_t right_index = get_right_index(heap, origin);

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
        max_heapify(heap, largest);
    }
}

void *dequeue(heap_t *heap)
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
    max_heapify(heap, 1);

    return item;
}

void print_heap(heap_t *heap)
{
    for (int i = 1; i < heap->size + 1; i++)
    {
        heap->print(heap->data[i]);
        printf(" ");
    }
    printf("\n");
}

void enqueue_int(heap_t *heap, int item)
{
    int *mal_item = malloc(sizeof(int));
    *mal_item = item;
    enqueue(heap, (void *)mal_item);
}

int main(void)
{
    heap_t *heap = create_heap(15, print_int, compare_int);
    enqueue_int(heap, 14);
    enqueue_int(heap, 2);
    enqueue_int(heap, 1);
    enqueue_int(heap, 4);
    enqueue_int(heap, 8);
    enqueue_int(heap, 7);
    enqueue_int(heap, 3);
    enqueue_int(heap, 9);
    enqueue_int(heap, 16);
    enqueue_int(heap, 10);

    print_heap(heap);
    printf("size=%ld\n", heap->size);

    int old_size = heap->size;
    for (int i = 0; i < old_size; i++)
    {
        void *res = dequeue(heap);
        if (res != NULL)
        {
            printf("%d\n", *(int *)res);
        }
    }
    return 0;
}