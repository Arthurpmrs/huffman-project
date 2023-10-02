#ifndef HUFFMAN_UTILS_H
#define HUFFMAN_UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Set the bit
 *
 * @param n byte to set the bit
 * @param i index of the bit to be set
 * @return uint8_t
 */
uint8_t set_bit(uint8_t n, uint8_t i);

/**
 * @brief Check if the bit is set
 *
 * @param n byte to check the bit
 * @param i index of the bit to be checked
 * @return true if the bit is set
 * @return false if the bit is not set
 */
bool is_bit_set(uint8_t n, uint8_t i);

#endif