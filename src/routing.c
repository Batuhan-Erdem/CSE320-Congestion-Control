#include <stdio.h>
#include <string.h>
#include "../include/routing.h"

static void add_route(RoutingTable *table, const char *destination, const char *next_hop, int cost, const char *path) {
    Route *route = &table->routes[table->route_count];

    strcpy(route->destination, destination);
    strcpy(route->next_hop, next_hop);
    route->cost = cost;
    strcpy(route->path, path);

    table->route_count++;
}

void build_static_routing_table(const Node *node, RoutingTable *table) {
    table->route_count = 0;

    if (strcmp(node->name, "A") == 0) {
        add_route(table, "A", "-", 0, "A");
        add_route(table, "B", "B", 4, "A -> B");
        add_route(table, "C", "C", 7, "A -> C");
        add_route(table, "D", "B", 12, "A -> B -> D");
        add_route(table, "E", "B", 7, "A -> B -> E");
        add_route(table, "F", "F", 5, "A -> F");
    }
    else if (strcmp(node->name, "B") == 0) {
        add_route(table, "A", "A", 4, "B -> A");
        add_route(table, "B", "-", 0, "B");
        add_route(table, "C", "A", 11, "B -> A -> C");
        add_route(table, "D", "D", 8, "B -> D");
        add_route(table, "E", "E", 3, "B -> E");
        add_route(table, "F", "A", 9, "B -> A -> F");
    }
    else if (strcmp(node->name, "C") == 0) {
        add_route(table, "A", "A", 7, "C -> A");
        add_route(table, "B", "A", 11, "C -> A -> B");
        add_route(table, "C", "-", 0, "C");
        add_route(table, "D", "A", 19, "C -> A -> B -> D");
        add_route(table, "E", "A", 14, "C -> A -> B -> E");
        add_route(table, "F", "A", 12, "C -> A -> F");
    }
    else if (strcmp(node->name, "D") == 0) {
        add_route(table, "A", "B", 12, "D -> B -> A");
        add_route(table, "B", "B", 8, "D -> B");
        add_route(table, "C", "B", 19, "D -> B -> A -> C");
        add_route(table, "D", "-", 0, "D");
        add_route(table, "E", "B", 11, "D -> B -> E");
        add_route(table, "F", "B", 17, "D -> B -> A -> F");
    }
    else if (strcmp(node->name, "E") == 0) {
        add_route(table, "A", "B", 7, "E -> B -> A");
        add_route(table, "B", "B", 3, "E -> B");
        add_route(table, "C", "B", 14, "E -> B -> A -> C");
        add_route(table, "D", "B", 11, "E -> B -> D");
        add_route(table, "E", "-", 0, "E");
        add_route(table, "F", "B", 12, "E -> B -> A -> F");
    }
    else if (strcmp(node->name, "F") == 0) {
        add_route(table, "A", "A", 5, "F -> A");
        add_route(table, "B", "A", 9, "F -> A -> B");
        add_route(table, "C", "A", 12, "F -> A -> C");
        add_route(table, "D", "A", 17, "F -> A -> B -> D");
        add_route(table, "E", "A", 12, "F -> A -> B -> E");
        add_route(table, "F", "-", 0, "F");
    }
}

void print_routing_table(const Node *node, const RoutingTable *table) {
    printf("\nRouting table for node %s\n", node->name);
    printf("Destination\tNext Hop\tCost\tPath\n");

    for (int i = 0; i < table->route_count; i++) {
        printf("%s\t\t%s\t\t%d\t%s\n",
               table->routes[i].destination,
               table->routes[i].next_hop,
               table->routes[i].cost,
               table->routes[i].path);
    }
}

Route* find_route(RoutingTable *table, const char *destination) {
    for (int i = 0; i < table->route_count; i++) {
        if (strcmp(table->routes[i].destination, destination) == 0) {
            return &table->routes[i];
        }
    }

    return NULL;
}

void simulate_send(const Node *node, RoutingTable *table, const char *destination, const char *message) {
    Route *route = find_route(table, destination);

    if (route == NULL) {
        printf("No route to destination %s\n", destination);
        return;
    }

    printf("\n[%s] Destination %s, next hop %s\n",
           node->name,
           destination,
           route->next_hop);

    char path_copy[100];
    strcpy(path_copy, route->path);

    char nodes[10][MAX_NAME_LEN];
    int count = 0;

    char *token = strtok(path_copy, " ");
    while (token != NULL) {
        if (strcmp(token, "->") != 0) {
            strcpy(nodes[count], token);
            count++;
        }

        token = strtok(NULL, " ");
    }

    if (count == 1) {
        printf("[%s] Received message: %s\n", node->name, message);
        return;
    }

    for (int i = 1; i < count - 1; i++) {
        printf("[%s] Forwarding message from %s to %s, next hop %s\n",
               nodes[i],
               nodes[0],
               destination,
               nodes[i + 1]);
    }

    printf("[%s] Received message from %s: %s\n",
           destination,
           nodes[0],
           message);
}