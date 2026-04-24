#include <stdio.h>
#include <string.h>
#include "../include/node.h"

int load_config(const char *filename, Node *node) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error: Could not open config file: %s\n", filename);
        return 0;
    }

    char keyword[50];
    node->neighbor_count = 0;

    while (fscanf(file, "%s", keyword) != EOF) {
        if (strcmp(keyword, "NODE") == 0) {
            fscanf(file, "%s", node->name);
        } 
        else if (strcmp(keyword, "PORT") == 0) {
            fscanf(file, "%d", &node->port);
        } 
        else if (strcmp(keyword, "NEIGHBOR") == 0) {
            Neighbor *n = &node->neighbors[node->neighbor_count];

            fscanf(file, "%s %d %d", n->name, &n->port, &n->cost);

            node->neighbor_count++;
        }
    }

    fclose(file);
    return 1;
}

void print_node_info(const Node *node) {
    printf("Node: %s\n", node->name);
    printf("Port: %d\n", node->port);
    printf("Neighbors:\n");

    for (int i = 0; i < node->neighbor_count; i++) {
        printf("- %s port=%d cost=%d\n",
               node->neighbors[i].name,
               node->neighbors[i].port,
               node->neighbors[i].cost);
    }
}