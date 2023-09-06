#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define DEBUG 1

uint16_t set_bit(uint16_t n, uint8_t i)
{
    uint16_t mask = 1 << i;
    return mask | n;
}

bool is_bit_set(uint16_t n, uint8_t i)
{
    uint16_t mask = 1 << i;
    return mask & n;
}

bool print_as_bin(int n, size_t size)
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

/**
 * @brief Get the unziped file path string
 *
 * @param unzipped
 * @param zipped
 */
bool get_unzipped_path(char unzipped[256], char zipped[256])
{
    char *dot = strrchr(zipped, '.');
    if (dot && strcmp(dot, ".huff") != 0)
    {
        printf("File must end in .huff\n");
        return false;
    }

    strcpy(unzipped, zipped);
    uint16_t path_size = strlen(zipped);
    unzipped[path_size - 5] = '\0';

    if (DEBUG)
    {
        printf("%s\n", unzipped);
    }
}

/**
 * @brief Get the sizes from header object
 *
 * @param input
 * @param trash_size
 * @param tree_size
 */
bool get_sizes_from_header(FILE *input, uint16_t *trash_size, uint16_t *tree_size)
{
    if (input == NULL)
    {
        return false;
    }

    uint8_t header_bytes[2];
    fread(header_bytes, sizeof(uint8_t), 2, input);

    // get trash size
    *trash_size = header_bytes[0] >> 5;

    // get tree size
    uint8_t mask = 0b00011111;
    *tree_size = (header_bytes[0] & mask) << 8 | header_bytes[1];

    if (DEBUG)
    {
        print_as_bin(header_bytes[0], 8);
        print_as_bin(header_bytes[1], 8);
        printf("trash_size = %d - ", *trash_size);
        print_as_bin(*trash_size, 8);
        printf("tree_size = %d - ", *tree_size);
        print_as_bin(*tree_size, 16);
    }

    return true;
}

int main(void)
{
    char zipped_path[256];
    if (DEBUG)
    {
        strcpy(zipped_path, "examples/bocchi.jpg.huff");
    }
    else
    {
        printf("Enter the name of the file to be compressed (relative to cwd): ");
        scanf("%s", zipped_path);
    }

    char unzipped_path[256];
    get_unzipped_path(unzipped_path, zipped_path);

    FILE *input = fopen(zipped_path, "rb");
    if (!input)
    {
        printf("File \"%s\" could not be found or accessed.\n", zipped_path);
        exit(EXIT_FAILURE);
    }

    uint16_t trash_size;
    uint16_t tree_size;
    get_sizes_from_header(input, &trash_size, &tree_size);

    fclose(input);

    // Construct output filename string

    // read and process the first two bytes (get tree_size and trash_size)

    // recreate tree from header (pre-order)

    // unzip file
    return 0;
}