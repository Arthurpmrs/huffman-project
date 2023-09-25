#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

#include "libs/heap.h"
#include "libs/priority_queue.h"
#include "libs/utils.h"

void hp_enqueue_int(heap_t *heap, uint32_t item, uint32_t *counter)
{
    int *mal_item = malloc(sizeof(int));
    *mal_item = item;
    hp_enqueue(heap, (void *)mal_item, counter);
}

void pq_enqueue_int(priority_queue_t *pq, uint32_t priority, uint32_t *counter)
{
    int *mal_item = malloc(sizeof(int));
    *mal_item = 1;
    pq_enqueue(pq, mal_item, priority, counter);
}

uint32_t get_random_int(uint32_t min, uint32_t max)
{
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

void counting(FILE *input, FILE *output, uint32_t size, uint32_t max_sample_size)
{
    uint8_t n_enqueue_samples = 60;

    heap_t *hp = hp_create(size + 10 * n_enqueue_samples, print_int, compare_int);
    priority_queue_t *pq = pq_create(print_int);

    // Initial population
    uint32_t item;
    uint32_t counter = 0;
    for (int i = 0; i < size; i++)
    {
        fscanf(input, "%d", &item);
        hp_enqueue_int(hp, item, &counter);
        pq_enqueue_int(pq, item, &counter);
    }

    // enqueue random samples
    uint32_t random_sample;
    uint32_t min = (uint32_t)(0.6 * max_sample_size);
    uint32_t max = (uint32_t)(1.6 * max_sample_size);
    uint32_t hp_counter;
    uint32_t pq_counter;

    for (int i = 0; i < n_enqueue_samples; i++)
    {
        hp_counter = 0;
        pq_counter = 0;

        random_sample = get_random_int(min, max);

        pq_enqueue_int(pq, random_sample, &pq_counter);
        hp_enqueue_int(hp, random_sample, &hp_counter);

        printf("(n=%d) %dª Drawn number: %d\n", size, i, random_sample);
        printf("    Priority Queue: %d comparisons\n", pq_counter);
        printf("    Heap: %d comparisons\n", hp_counter);
        fprintf(output, "%d,%d,%d,%d\n", size, random_sample, hp_counter, pq_counter);
    }

    hp_clear(hp);
    pq_clear(pq);
}

int main(void)
{
    srand(time(NULL));

    FILE *input = fopen("data/counting_samples.txt", "r");
    FILE *output = fopen("data/processed.txt", "w");

    uint32_t max_sample_size;
    fscanf(input, "%d\n", &max_sample_size);
    printf("max_sample_size: %d\n", max_sample_size);

    uint32_t size;
    while (fscanf(input, "%d", &size) != EOF)
    {
        counting(input, output, size, max_sample_size);
    }

    fclose(input);
    fclose(output);

    return 0;
}