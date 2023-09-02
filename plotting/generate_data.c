#include "heap.h"
#include "priority_queue.h"
#include "utils.h"

void hp_enqueue_int(heap_t *heap, int item)
{
    int *mal_item = malloc(sizeof(int));
    *mal_item = item;
    hp_enqueue(heap, (void *)mal_item);
}

int main(void)
{
    heap_t *heap = hp_create(15, print_int, compare_int);
    hp_enqueue_int(heap, 14);
    hp_enqueue_int(heap, 2);
    hp_enqueue_int(heap, 1);
    hp_enqueue_int(heap, 4);
    hp_enqueue_int(heap, 8);
    hp_enqueue_int(heap, 7);
    hp_enqueue_int(heap, 3);
    hp_enqueue_int(heap, 9);
    hp_enqueue_int(heap, 16);
    hp_enqueue_int(heap, 10);

    hp_print(heap);
    printf("size=%ld\n", heap->size);

    int old_size = heap->size;
    for (int i = 0; i < old_size; i++)
    {
        void *res = hp_dequeue(heap);
        if (res != NULL)
        {
            printf("%d\n", *(int *)res);
        }
    }

    priority_queue_t *pq = pq_create(print_int);

    int values[10] = {45, 87, 96, 78, 12, 1, 0, 47, 3, 5};
    int priorities[10] = {5, 98, 15, 98, 45, 12, 13, 77, 42, 7};
    for (int i = 0; i < 10; i++)
    {
        pq_enqueue(pq, (void *)&values[i], priorities[i]);
    }

    int newValue = 9999;
    int newValue2 = 7777;
    pq_enqueue(pq, (void *)&newValue, 1);
    pq_enqueue(pq, (void *)&newValue2, 100);
    pq_print(pq);
    printf("size: %ld\n", pq->size);

    pq_dequeue(pq);
    pq_dequeue(pq);
    pq_print(pq);
    printf("size: %ld\n", pq->size);
    return 0;
}