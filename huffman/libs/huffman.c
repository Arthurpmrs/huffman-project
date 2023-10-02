#include "huffman.h"

bool is_special_node(huff_node_t *node)
{
    bool is_leaf = node->left == NULL &&
                   node->right == NULL;

    bool is_special_char = *(uint8_t *)node->byte == '\\' ||
                           *(uint8_t *)node->byte == '*';

    return is_leaf && is_special_char;
}

huff_queue_t *huff_create_queue()
{
    huff_queue_t *new_queue = malloc(sizeof(huff_queue_t));
    new_queue->head = NULL;
    new_queue->size = 0;
    return new_queue;
}

void huff_enqueue(huff_queue_t *hq, void *byte, uint64_t frequency,
                  huff_node_t *left, huff_node_t *right)
{
    huff_node_t *new_node = malloc(sizeof(huff_node_t));
    new_node->byte = byte;
    new_node->frequency = frequency;
    new_node->left = left;
    new_node->right = right;

    if (hq->head == NULL)
    {
        new_node->next = NULL;
        hq->head = new_node;
    }
    else
    {
        if (frequency <= hq->head->frequency)
        {
            new_node->next = hq->head;
            hq->head = new_node;
        }
        else
        {
            huff_node_t *current = hq->head;
            while (current->next != NULL &&
                   frequency > current->next->frequency)
            {
                current = current->next;
            }
            new_node->next = current->next;
            current->next = new_node;
        }
    }
    hq->size += 1;
}

huff_node_t *huff_dequeue(huff_queue_t *hq)
{
    if (hq->head == NULL)
    {
        printf("The queue is empty.\n");
        return NULL;
    }

    huff_node_t *dequeded = hq->head;

    hq->head = hq->head->next;

    hq->size -= 1;

    return dequeded;
}

huff_node_t *huff_get_tree_from_queue(huff_queue_t *hq)
{
    if(hq->size == 1){
        huff_node_t* node = huff_dequeue(hq);
        huff_node_t* root = malloc(sizeof(huff_node_t));
        root->byte = malloc(sizeof(uint8_t));
        *(uint8_t*)root->byte = '*';
        root->frequency = node->frequency;
        root->left = NULL;
        root->right = node;
        return root;
    }else{
        while(hq->size>1){
            huff_node_t* left = huff_dequeue(hq);
            huff_node_t* right = huff_dequeue(hq);
            huff_node_t* root = malloc(sizeof(huff_node_t));
            root->byte = malloc(sizeof(uint8_t));
            *(uint8_t*)root->byte = '*';
            root->frequency = left->frequency + right->frequency;
            root->left = left;
            root->right = right;
            huff_enqueue(hq, root->byte, root->frequency, root->left, root->right);
        }
        return huff_dequeue(hq);
    }
}

// TODO: REMOVE PRINT TREE
void huff_print_tree(huff_node_t *ht)
{
    if (ht != NULL)
    {
        if (is_special_node(ht))
        {
            printf("%c", '\\');
        }
 
        printf("%c", *(uint8_t *)ht->byte);
 
        huff_print_tree(ht->left);
        huff_print_tree(ht->right);
    }
}

uint16_t huff_get_tree_size(huff_node_t *ht)
{
    if (ht == NULL)
    {
        return 0;
    }
    else
    { 
        if (is_special_node(ht)) {
            return 2 + huff_get_tree_size(ht->left) + huff_get_tree_size(ht->right);
        } else {
            return 1 + huff_get_tree_size(ht->left) + huff_get_tree_size(ht->right);
        } 
    }
}