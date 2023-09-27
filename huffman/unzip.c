#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "libs/utils.c"
#include "libs/binary_tree.h"

#define DEBUG 0
#define MAX_FILENAME_SIZE 256

/**
 * @brief Get the file size in bytes using sys/stat.h
 *
 * @param filename Name of the file
 * @return uint64_t Number of bytes (64 bits) in the file
 */
uint64_t get_file_size_in_bytes(char filename[MAX_FILENAME_SIZE])
{
    struct stat sb;
    stat(filename, &sb);
    return (uint64_t)sb.st_size;
}

/**
 * @brief Get the unzipped file path string
 *
 * @param unzipped Buffer to the unzipped file path
 * @param zipped Zipped file path
 */
bool get_unzipped_path(char unzipped[MAX_FILENAME_SIZE], char zipped[MAX_FILENAME_SIZE])
{
    char *dot = strrchr(zipped, '.');
    if (dot && strcmp(dot, ".huff") != 0)
    {
        printf("File must end in .huff\n");
        return false;
    }

    char buffer[MAX_FILENAME_SIZE];
    char *filename = strrchr(zipped, '/');

    if (filename == NULL)
    {
        strcpy(buffer, "unzip_");
        strcpy(buffer + strlen(buffer), zipped);
    }
    else
    {
        filename = filename + 1;
        strcpy(buffer, zipped);
        buffer[strlen(buffer) - strlen(filename)] = '\0';
        strcpy(buffer + strlen(buffer), "unzip_\0");
        strcpy(buffer + strlen(buffer), filename);
    }
    buffer[strlen(buffer) - 5] = '\0';

    strcpy(unzipped, buffer);

    return true;
}

/**
 * @brief Get trash and tree sizes from the zipped file header.
 *
 * @param input Pointer to the zipped file
 * @param trash_size Buffer for the trash size
 * @param tree_size Buffer for the tree size
 * @return true If everything went ok
 * @return false If there something wrong with the file pointer
 */
bool get_sizes_from_header(FILE *input, uint8_t *trash_size, uint16_t *tree_size)
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

    return true;
}

/**
 * @brief Unzips the next byte from input into output.
 *
 * @param input File pointer to the zipped file
 * @param output File pointer to the unzipped file
 * @param subtree Pointer to the current location of the huffman binary tree
 * @param root Pointer to the root of the huffman binary tree
 * @param end_bit_index Index of the last bit to be read
 * @return binary_tree_t* Pointer to the updated current location of the
 * huffman tree
 */
binary_tree_t *unzip_next(FILE *input, FILE *output, binary_tree_t *subtree,
                          binary_tree_t *root, uint8_t end_bit_index)
{
    uint8_t compressed_byte = 0;
    fread(&compressed_byte, sizeof(uint8_t), 1, input);

    for (int8_t bit = 7; bit >= end_bit_index; bit--)
    {
        if (is_bit_set(compressed_byte, bit))
        {
            subtree = subtree->right;
        }
        else
        {
            subtree = subtree->left;
        }

        if (subtree->left == NULL && subtree->right == NULL)
        {
            fwrite((uint8_t *)subtree->item, sizeof(uint8_t), 1, output);
            subtree = root;
        }
    }

    return subtree;
}

/**
 * @brief Unzips the compressed content of the zipped file into the unzipped file.
 *
 * @param input Pointer to the zipped file
 * @param ht Pointer to the root of the huffman binary tree
 * @param zipped_bytes_size Total size of bytes to be unzipped
 * @param trash_size Size of the trash in the last Byte
 * @param unzipped_path Path to the unzipped file
 * @return true if everything went ok
 * @return false if input or tree are NULL or the output file could not be accessed.
 */
bool unzip(FILE *input, binary_tree_t *ht, uint64_t zipped_bytes_size, uint8_t trash_size,
           char unzipped_path[MAX_FILENAME_SIZE])
{
    if (input == NULL || ht == NULL)
    {
        return false;
    }

    FILE *output = fopen(unzipped_path, "wb");

    binary_tree_t *current = ht;

    for (uint64_t byte = 0; byte < zipped_bytes_size - 1; byte++)
    {
        current = unzip_next(input, output, current, ht, 0);
    }

    unzip_next(input, output, current, ht, trash_size);
    fclose(output);
    return true;
}

int main(void)
{
    char zipped_path[MAX_FILENAME_SIZE];
    printf("Enter the name of the file to be compressed (relative to cwd): ");
    scanf("%s", zipped_path);
    
    FILE *input = fopen(zipped_path, "rb");

    uint8_t trash_size;
    uint16_t tree_size;
    // read first two bytes
    get_sizes_from_header(input, &trash_size, &tree_size);

    // remaining tree bytes
    uint8_t preorder_tree[tree_size];
    fread(preorder_tree, sizeof(uint8_t), tree_size, input);

    // rest of tree
    binary_tree_t *ht = reconstruct_tree(preorder_tree);
    
    // 2 is first two bytes of the header
    uint16_t header_size = 2 + tree_size;
    uint64_t zipped_bytes_size = get_file_size_in_bytes(zipped_path) - header_size;

    char unzipped_path[MAX_FILENAME_SIZE];

    unzip(input, ht, zipped_bytes_size, trash_size, unzipped_path);

    printf("[SUCCESS] File %s unzipped to %s\n", zipped_path, unzipped_path);
    fclose(input);

    // tear down structures
    return 0;
}