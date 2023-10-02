#include "utils.h"

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