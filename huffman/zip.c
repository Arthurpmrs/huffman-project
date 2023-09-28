
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "libs/huffman.h"
#include "libs/linked_list.h"
#include "libs/stack.h"
#include "libs/utils.h"

#define DEBUG 1
#define MAX_FILENAME_SIZE 256

/**
 * @brief Reads the file and counts the frequency of each byte
 * 
 * @param input File to be read
 * @param frequency Array of 256 elements to store the frequency of each byte
 */
void read_frequencies(FILE *input, uint64_t frequency[256]){
    uint8_t byte;

    while(fread(&byte, sizeof(uint8_t), 1, input) > 0){
        frequency[byte] += 1;
    }

    // return to begging file
    fseek(input, 0, SEEK_SET);
}

/**
 * @brief Populates a huffman queue with the bytes and their frequencies
 * 
 * @param frequencies Array of 256 elements with the frequency of each byte
 * @param hq Huffman queue to be populated
 */
void populate_huff_queue(uint64_t frequencies[256], huff_queue_t *hq){
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


// TODO: review
/**
 * @brief recursive function to generate the paths of each byte walking through the huffman tree
 * 
 * @param root Root of the huffman tree
 * @param paths_of_bytes Array of lists to store the paths of each byte
 * @param path List to store the path of the current byte
 * 
 * @return
 */
void generate_huff_paths(huff_node_t *node, list_t* paths_of_bytes[256], uint8_t path[256], int index){
    if(node->left==NULL && node->right == NULL){
        uint8_t leaf_byte = *(uint8_t*)node->byte;
        paths_of_bytes[leaf_byte] = list_create();
        for(int i = index - 1; i >= 0; i--){
            // TODO: review void pointer
            void* path_byte = malloc(sizeof(uint8_t));
            *(uint8_t*)path_byte = path[i];
            list_add_to_head(paths_of_bytes[leaf_byte], path_byte);
        }
    }

    if(node->left != NULL){
        path[index] = 0;
        generate_huff_paths(node->left, paths_of_bytes, path, index + 1);
    }

    if(node->right != NULL){
        path[index] = 1;
        generate_huff_paths(node->right, paths_of_bytes, path, index + 1);
    }
}

// TODO: add docs
void write_tree(huff_node_t* root, FILE *output){
    if (root != NULL)
    {
        if (is_special_node(root))
        {
            char scape_char = '\\';
            fwrite(&scape_char, sizeof(uint8_t), 1, output);
        }
 
        fwrite((uint8_t *)root->byte, sizeof(uint8_t), 1, output);
 
        write_tree(root->left, output);
        write_tree(root->right, output);
    }
}


/**
 * @brief Writes the huffman to the output file
 * 
 * @param input File to be read
 * @param output Root of the huffman tree
 * @param paths_of_bytes Array of lists with the paths of each byte
 */
void zip_file(FILE *input, FILE *output, huff_node_t *ht, list_t* paths_of_bytes[256], uint16_t tree_size){
    uint16_t temp_header = 0;
    fwrite(&temp_header, sizeof(uint16_t), 1, output);

    write_tree(ht, output);

    uint8_t byte;
    uint8_t byte_buffer=0;
    int8_t byte_index = 7;

    while(fread(&byte, sizeof(uint8_t), 1, input) > 0){
        list_t* path = paths_of_bytes[byte];
        list_node_t* current = path->head;
        while(current != NULL){
            if(byte_index == -1){
                printf("byte_buffer: %d\n", byte_buffer);
                fwrite(&byte_buffer, sizeof(uint8_t), 1, output);
                byte_buffer = 0;
                byte_index = 7;
            }

            uint8_t bit = *(uint8_t*)current->item;

            if(bit == 1){
                byte_buffer = set_bit(byte_buffer, byte_index);
            }

            byte_index--;
            current = current->next;
        }
    }

    uint16_t trash_size = (byte_index + 1) << 13;
    uint16_t header = trash_size | tree_size;
    uint8_t first_byte = header >> 8;
    uint8_t second_byte = header & 0b0000000011111111;

    fseek(output, 0, SEEK_SET);
    fwrite(&first_byte, sizeof(uint8_t), 1, output);
    fwrite(&second_byte, sizeof(uint8_t), 1, output);
    
    fclose(output);
}


int main(void)
{
    char file_path[MAX_FILENAME_SIZE];
    printf("Enter the name of the file to be compressed (relative to cwd): ");
    scanf("%s", file_path);

    FILE *input = fopen(file_path, "rb");

    uint64_t frequencies[256] = {};
    read_frequencies(input, frequencies);

    huff_queue_t *hq = huff_create_queue();
    populate_huff_queue(frequencies, hq);
    
    huff_node_t *root = huff_get_tree_from_queue(hq);
    uint16_t tree_size = huff_get_tree_size(root);

    list_t* paths_of_bytes[256];
    for(int i = 0; i < 256; i++){
        paths_of_bytes[i] = NULL;
    }

    uint8_t path[256] = {};
    generate_huff_paths(root, paths_of_bytes, path, 0);

    char zipped_path[MAX_FILENAME_SIZE];
    strcpy(zipped_path, file_path);
    strcat(zipped_path, ".huff");

    FILE *output = fopen(zipped_path, "wb");
    zip_file(input, output, root, paths_of_bytes, tree_size);

    return 0;
}