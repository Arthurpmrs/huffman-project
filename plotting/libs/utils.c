#include "utils.h"

void swap(void **a, void **b)
{
    void *tmp = *a;
    *a = *b;
    *b = tmp;
}

void print_int(void *item)
{
    printf("%d", *(int *)item);
}

int compare_int(void *a, void *b)
{
    return *(int *)a - *(int *)b;
}