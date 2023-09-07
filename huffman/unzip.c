#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "libs/binary_tree.h"

#define DEBUG 1
#define MAX_FILENAME_SIZE 256

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

uint64_t get_file_size_in_bytes(char filename[MAX_FILENAME_SIZE])
{
    struct stat sb;
    if (stat(filename, &sb) == -1)
    {
        printf("It was not possible to get the size of the zipped file.\n");
        exit(EXIT_FAILURE);
    }
    return (uint64_t)sb.st_size;
}

/**
 * @brief Get the unziped file path string
 *
 * @param unzipped
 * @param zipped
 */
bool get_unzipped_path(char unzipped[MAX_FILENAME_SIZE], char zipped[MAX_FILENAME_SIZE])
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

binary_tree_t *get_tree_from_preorder(uint16_t *i, uint16_t tree_size, uint8_t preorder_tree[tree_size])
{
    // printf("(i=%d) byte %d (next=%d)\n", *i, preorder_tree[*i], preorder_tree[*i + 1]);
    uint8_t *item = malloc(sizeof(uint8_t));
    if (preorder_tree[*i] == '*')
    {
        *item = '*';
        *i += 1;
        binary_tree_t *left = get_tree_from_preorder(i, tree_size, preorder_tree);
        binary_tree_t *right = get_tree_from_preorder(i, tree_size, preorder_tree);
        return create_binary_tree((void *)item, left, right);
    }
    else
    {
        if (preorder_tree[*i] == '\\')
        {
            *item = preorder_tree[*i + 1];
            *i += 2;
        }
        else
        {
            *item = preorder_tree[*i];
            *i += 1;
        }
        return create_binary_tree((void *)item, NULL, NULL);
    }
}

// void compare(int *i, uint16_t tree_size, uint8_t preorder_tree[tree_size], binary_tree_t *ht)
// {

//     if (ht != NULL)
//     {
//         if (preorder_tree[*i] == '\\')
//         {
//             *i += 1;
//         }
//         if (*(unsigned char *)ht->item == preorder_tree[*i])
//         {
//             printf("%d x %d (i=%d)\n", *(unsigned char *)ht->item, preorder_tree[*i], *i);
//         }
//         else
//         {
//             printf(">>>>%d x %d (i=%d)\n", *(unsigned char *)ht->item, preorder_tree[*i], *i);
//         }
//         *i += 1;
//         compare(i, tree_size, preorder_tree, ht->left);
//         compare(i, tree_size, preorder_tree, ht->right);
//     }
// }

void print_byte(void *item)
{
    printf("%c", *(uint8_t *)item);
}

int main(void)
{
    char zipped_path[MAX_FILENAME_SIZE];
    if (DEBUG)
    {
        strcpy(zipped_path, "examples/bocchi.jpg.huff");
    }
    else
    {
        printf("Enter the name of the file to be compressed (relative to cwd): ");
        scanf("%s", zipped_path);
    }

    char unzipped_path[MAX_FILENAME_SIZE];
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

    uint8_t preorder_tree[tree_size];
    fread(preorder_tree, sizeof(uint8_t), tree_size, input);

    if (DEBUG)
    {
        printf("Preorder tree from header:\n");
        for (int i = 0; i < tree_size; i++)
        {
            printf("%c", preorder_tree[i]);
        }
        printf("\n");
    }

    // uint16_t tree_size = 11;
    // char preorder_tree[11] = "**CB***FEDA";
    uint16_t aux_index = 0;
    binary_tree_t *ht = get_tree_from_preorder(&aux_index, tree_size, preorder_tree);

    if (DEBUG)
    {
        printf("Reconstructed preorder tree:\n");
        print_pre_order(ht, print_byte);
    }

    uint8_t byte;
    int count = 0;
    while (fread(&byte, sizeof(uint8_t), 1, input) != 0)
    {
        count++;
    }
    uint64_t zipped_bytes_size = get_file_size_in_bytes(zipped_path) - 2 - tree_size;
    printf("\ncount=%d - statsize=%lld\n", count, zipped_bytes_size);

    fclose(input);
    return 0;
}