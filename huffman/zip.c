
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "libs/huffman.h"
#include "libs/linked_list.h"
#include "libs/utils.h"

#define MAX_FILENAME_SIZE 256

/**
 * @brief Reads the file and counts the frequency of each byte
 *
 * @param input File to be read
 * @param frequency Array of 256 elements to store the frequency of each byte
 */
void read_frequencies(FILE *input, uint64_t frequency[256])
{
    uint8_t byte;

    while (fread(&byte, sizeof(uint8_t), 1, input) > 0)
    {
        frequency[byte] += 1;
    }

    // return to beginning of the file
    fseek(input, 0, SEEK_SET);
}

/**
 * @brief Populates a huffman queue with the bytes and their frequencies
 *
 * @param hq Huffman queue to be populated
 * @param frequencies Array of 256 elements with the frequency of each byte
 */
void populate_huff_queue(huff_queue_t *hq, uint64_t frequencies[256])
{
    for (uint16_t i = 0; i < 256; i++)
    {
        if (frequencies[i] > 0)
        {
            uint8_t *byte = malloc(sizeof(uint8_t));
            *byte = i;
            huff_enqueue(hq, byte, frequencies[i], NULL, NULL);
        }
    }
}

/**
 * @brief recursive function to generate the paths of each byte walking through the huffman tree
 *
 * @param ht Huffman tree node (starts at root)
 * @param paths_of_bytes Array of lists to store the paths of each byte
 * @param path Auxiliary array to store the current byte path
 * @param index Auxiliary array index
 */
void generate_huff_paths(huff_node_t *ht, list_t *paths_of_bytes[256], uint8_t path[256], int8_t index)
{
    if (ht == NULL)
    {
        return;
    }

    if (ht->left == NULL && ht->right == NULL)
    {
        uint8_t leaf_byte = *(uint8_t *)ht->byte;
        paths_of_bytes[leaf_byte] = list_create();

        // copy auxiliary path array to current byte path list
        for (int i = index - 1; i >= 0; i--)
        {
            void *path_byte = malloc(sizeof(uint8_t));
            *(uint8_t *)path_byte = path[i];
            list_add_to_head(paths_of_bytes[leaf_byte], path_byte);
        }
    }
    else
    {
        path[index] = 0;
        generate_huff_paths(ht->left, paths_of_bytes, path, index + 1);

        path[index] = 1;
        generate_huff_paths(ht->right, paths_of_bytes, path, index + 1);
    }
}

/**
 * @brief Zips a file
 *
 * @param input pointer to the file to be zipped
 * @param output pointer to the output file
 * @param ht pointer to the root of the huffman tree
 * @param paths_of_bytes Array of lists to store the paths of each byte
 */
void zip_file(FILE *input, FILE *output, huff_node_t *ht, list_t *paths_of_bytes[256])
{
    uint16_t temp_header = 0;
    fwrite(&temp_header, sizeof(uint16_t), 1, output);

    huff_write_tree_to_file(ht, output);

    uint8_t byte;
    uint8_t byte_buffer = 0;
    int8_t byte_index = 7;

    while (fread(&byte, sizeof(uint8_t), 1, input) > 0)
    {
        list_t *path = paths_of_bytes[byte];
        list_node_t *current = path->head;
        while (current != NULL)
        {
            uint8_t bit = *(uint8_t *)current->item;

            if (bit == 1)
            {
                byte_buffer = set_bit(byte_buffer, byte_index);
            }

            byte_index--;

            if (byte_index == -1)
            {
                fwrite(&byte_buffer, sizeof(uint8_t), 1, output);
                byte_buffer = 0;
                byte_index = 7;
            }

            current = current->next;
        }
    }

    // Checks if there is any trash
    if (byte_index != 7)
    {
        fwrite(&byte_buffer, sizeof(uint8_t), 1, output);
    }

    // updates header with trash and tree size
    uint16_t tree_size = huff_get_tree_size(ht);
    uint16_t trash_size = (byte_index + 1) << 13;

    uint16_t header = trash_size | tree_size;
    uint8_t first_byte = header >> 8;
    uint8_t second_byte = header & 0b0000000011111111;

    fseek(output, 0, SEEK_SET);
    fwrite(&first_byte, sizeof(uint8_t), 1, output);
    fwrite(&second_byte, sizeof(uint8_t), 1, output);
}

int main(void)
{
    char file_path[MAX_FILENAME_SIZE];
    printf("Enter the name of the file to be compressed (relative to cwd): ");
    scanf("%s", file_path);

    FILE *input = fopen(file_path, "rb");

    uint64_t frequencies[256];
    read_frequencies(input, frequencies);

    huff_queue_t *hq = huff_create_queue();
    populate_huff_queue(hq, frequencies);

    huff_node_t *ht = huff_get_tree_from_queue(hq);

    list_t *paths_of_bytes[256];
    for (int i = 0; i < 256; i++)
    {
        paths_of_bytes[i] = NULL;
    }

    uint8_t path[256];
    generate_huff_paths(ht, paths_of_bytes, path, 0);

    char zipped_path[MAX_FILENAME_SIZE];
    strcpy(zipped_path, file_path);
    strcat(zipped_path, ".huff");

    FILE *output = fopen(zipped_path, "wb");
    zip_file(input, output, ht, paths_of_bytes);

    fclose(input);
    fclose(output);

    return 0;
}