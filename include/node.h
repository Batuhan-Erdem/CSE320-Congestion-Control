#ifndef NODE_H
#define NODE_H

#define MAX_NEIGHBORS 10
#define MAX_NAME_LEN 10

typedef struct {
    char name[MAX_NAME_LEN];
    int port;
    int cost;
} Neighbor;

typedef struct {
    char name[MAX_NAME_LEN];
    int port;
    Neighbor neighbors[MAX_NEIGHBORS];
    int neighbor_count;
} Node;

int load_config(const char *filename, Node *node);
void print_node_info(const Node *node);

#endif