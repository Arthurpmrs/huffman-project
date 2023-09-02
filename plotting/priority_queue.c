#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;
struct Node
{
    void *item;
    int priority;
    Node *next;
};

typedef struct PriorityQueueList
{
    Node *head;
    Node *tail;
    int size;
} PriorityQueueList;

PriorityQueueList *createPriorityQueueList()
{
    PriorityQueueList *newPQueue = malloc(sizeof(PriorityQueueList));
    newPQueue->head = NULL;
    newPQueue->tail = NULL;
    newPQueue->size = 0;
    return newPQueue;
}

int isEmpty(PriorityQueueList *ql)
{
    return ql->head == NULL;
}

void enqueue(PriorityQueueList *pql, void *item, int priority)
{
    Node *newNode = malloc(sizeof(Node));
    newNode->item = item;
    newNode->priority = priority;

    if (isEmpty(pql))
    {
        newNode->next = NULL;
        pql->head = newNode;
        pql->tail = newNode;
    }
    else
    {
        if (priority > pql->head->priority)
        {
            newNode->next = pql->head;
            pql->head = newNode;
        }
        else
        {
            Node *current = pql->head;
            while (current->next != NULL &&
                   priority <= current->next->priority)
            {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }
    pql->size += 1;
}

void *dequeue(PriorityQueueList *pql)
{
    if (isEmpty(pql))
    {
        printf("The queue is empty.\n");
        return NULL;
    }

    Node *DHead = pql->head;
    void *item = pql->head->item;

    if (pql->head == pql->tail)
    {
        pql->head = NULL;
        pql->tail = NULL;
    }
    else
    {
        pql->head = pql->head->next;
    }

    free(DHead);
    pql->size -= 1;

    return item;
}

void printQueue(PriorityQueueList *ql, void (*printFun)(void *))
{
    if (isEmpty(ql))
    {
        printf("The queue is empty.\n");
        return;
    }

    printf("HEAD ");
    Node *current = ql->head;
    while (current != NULL)
    {

        printFun(current->item);
        printf("(%d)", current->priority);
        if (current->next != NULL)
        {
            printf("->");
        }

        current = current->next;
    }
    printf(" LAST\n");
}

// Integer specific functions
void printInt(void *item)
{
    printf("%d", *(int *)item);
}

// int main(void)
// {
//     int arr[10] = {45, 87, 96, 78, 12, 1, 0, 47, 3, 5};
//     PriorityQueueList *pql = createPriorityQueueList();
//     int priorities[10] = {5, 98, 15, 98, 45, 12, 13, 77, 42, 7};
//     for (int i = 0; i < 10; i++)
//     {
//         enqueue(pql, (void *)&arr[i], priorities[i]);
//     }

//     int newValue = 9999;
//     int newValue2 = 7777;
//     enqueue(pql, (void *)&newValue, 1);
//     enqueue(pql, (void *)&newValue2, 100);
//     printQueue(pql, printInt);
//     printf("size: %d\n", pql->size);

//     dequeue(pql);
//     dequeue(pql);
//     printQueue(pql, printInt);
//     printf("size: %d\n", pql->size);
//     return 0;
// }