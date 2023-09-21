#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t set_bit(uint8_t n, uint8_t i)
{
    uint8_t mask = 1 << i;
    return mask | n;
}

bool is_bit_set(uint8_t n, uint8_t i)
{
    uint8_t mask = 1 << i;
    return mask & n;
}

void print_as_bin(int n, size_t size)
{
    uint8_t aux[size];
    for (int i = size - 1; i >= 0; i--)
    {
        if (n == 0)
        {
            aux[i] = 0;
        }
        else
        {
            aux[i] = n % 2;
            n = n / 2;
        }
    }

    for (int i = 0; i < size; i++)
    {
        if (i % 8 == 0 && i != 0)
        {
            printf(" ");
        }
        printf("%d", aux[i]);
    }
    printf("\n");
}

void print_byte(void *item)
{
    printf("%c", *(uint8_t *)item);
}