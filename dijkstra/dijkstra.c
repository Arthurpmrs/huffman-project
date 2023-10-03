#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INFINITY 2147483647
#define MAX_SIZE 16

typedef struct Node Node;
struct Node
{
    int item;
    int priority;
    Node *next;
};

typedef struct priority_queue
{
    Node *head;
    int size;
} priority_queue;

priority_queue *create_priority_queue()
{
    priority_queue *newPQueue = malloc(sizeof(priority_queue));
    newPQueue->head = NULL;
    newPQueue->size = 0;
    return newPQueue;
}

int is_empty(priority_queue *ql)
{
    return ql->head == NULL;
}

void enqueue(priority_queue *pql, int item, int priority)
{
    Node *newNode = malloc(sizeof(Node));
    newNode->item = item;
    newNode->priority = priority;

    if (is_empty(pql))
    {
        newNode->next = NULL;
        pql->head = newNode;
    }
    else
    {
        if (priority < pql->head->priority)
        {
            newNode->next = pql->head;
            pql->head = newNode;
        }
        else
        {
            Node *current = pql->head;
            while (current->next != NULL &&
                   priority >= current->next->priority)
            {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }
    pql->size += 1;
}

int dequeue(priority_queue *pql)
{
    if (is_empty(pql))
    {
        printf("The queue is empty.\n");
        return -1;
    }

    Node *DHead = pql->head;
    int item = pql->head->item;
    pql->head = pql->head->next;

    free(DHead);
    pql->size -= 1;

    return item;
}

typedef struct adj_list_node adj_list_node;
struct adj_list_node
{
    int value;
    int weight;
    adj_list_node *next;
};

typedef struct Graph
{
    adj_list_node *neighbors[MAX_SIZE];
    char visited[MAX_SIZE];
    char inserted[MAX_SIZE];
} Graph;

Graph *createGraph()
{
    Graph *graph = malloc(sizeof(Graph));
    for (int i = 0; i < MAX_SIZE; i++)
    {
        graph->neighbors[i] = NULL;
        graph->visited[i] = 0;
        graph->inserted[i] = 0;
    }
    return graph;
}

adj_list_node *createAdjListNode(int value, int weight)
{
    adj_list_node *node = malloc(sizeof(adj_list_node));
    node->next = NULL;
    node->weight = weight;
    node->value = value;
    return node;
}

/**
 * @brief Add vertex to undirected graph.
 *
 * @param graph
 * @param vertex1
 * @param vertex2
 */
void addEdge(Graph *graph, int vertex1, int vertex2, int weight)
{
    adj_list_node *vertex = createAdjListNode(vertex2, weight);
    vertex->next = graph->neighbors[vertex1];
    graph->neighbors[vertex1] = vertex;
    graph->inserted[vertex1] = 1;
    graph->inserted[vertex2] = 1;
}

void dijkstra(Graph *graph, int source, int distances[MAX_SIZE])
{
    priority_queue *q = create_priority_queue();

    for (int i = 0; i < MAX_SIZE; i++)
    {
        distances[i] = INFINITY;
    }

    distances[source] = 0;
    enqueue(q, source, distances[source]);

    while (!is_empty(q))
    {
        int current = dequeue(q);

        // Does not visit an already visited node
        if (graph->visited[current])
        {
            continue;
        }

        graph->visited[current] = true;

        adj_list_node *neighbor = graph->neighbors[current];
        while (neighbor != NULL)
        {
            int alternative_path = distances[current] + neighbor->weight;

            // Does not re-check neighbors that were already visited
            if (alternative_path < distances[neighbor->value] && !graph->visited[neighbor->value])
            {
                distances[neighbor->value] = alternative_path;
                enqueue(q, neighbor->value, distances[neighbor->value]);
            }

            neighbor = neighbor->next;
        }
    }
}

void example1(Graph *graph, int distances[])
{
    addEdge(graph, 1, 2, 2);
    addEdge(graph, 1, 3, 4);
    addEdge(graph, 2, 4, 7);
    addEdge(graph, 2, 3, 1);
    addEdge(graph, 3, 5, 3);
    addEdge(graph, 4, 6, 1);
    addEdge(graph, 5, 4, 2);
    addEdge(graph, 5, 6, 5);

    dijkstra(graph, 1, distances);
}

void example2(Graph *graph, int distances[])
{
    addEdge(graph, 1, 2, 50);
    addEdge(graph, 1, 4, 10);
    addEdge(graph, 1, 3, 45);
    addEdge(graph, 2, 3, 10);
    addEdge(graph, 2, 4, 15);
    addEdge(graph, 3, 5, 30);
    addEdge(graph, 4, 1, 10);
    addEdge(graph, 4, 5, 15);
    addEdge(graph, 5, 2, 20);
    addEdge(graph, 5, 3, 35);
    addEdge(graph, 6, 5, 3);

    dijkstra(graph, 1, distances);
}

void example3(Graph *graph, int distances[])
{
    addEdge(graph, 0, 1, 2);
    addEdge(graph, 0, 3, 1);
    addEdge(graph, 0, 2, 6);
    addEdge(graph, 1, 0, 2);
    addEdge(graph, 1, 3, 3);
    addEdge(graph, 1, 4, 6);
    addEdge(graph, 2, 4, 1);
    addEdge(graph, 3, 1, 3);
    addEdge(graph, 3, 0, 1);
    addEdge(graph, 3, 4, 5);
    addEdge(graph, 4, 1, 6);
    addEdge(graph, 4, 3, 5);

    dijkstra(graph, 0, distances);
}

// Failing example
void example4(Graph *graph, int distances[])
{
    addEdge(graph, 1, 2, 3);
    addEdge(graph, 1, 4, 5);
    addEdge(graph, 4, 3, 2);
    addEdge(graph, 3, 2, -10);

    dijkstra(graph, 1, distances);
}

int main(void)
{
    Graph *graph = createGraph();

    int distances[MAX_SIZE];

    example1(graph, distances);
    // example2(graph, distances);
    // example3(graph, distances);
    // example4(graph, distances);

    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (graph->inserted[i])
        {
            printf("%d: %d\n", i, distances[i]);
        }
    }
    return 0;
}