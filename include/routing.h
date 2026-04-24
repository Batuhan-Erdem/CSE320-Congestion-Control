#ifndef ROUTING_H
#define ROUTING_H

#include "node.h"

#define MAX_ROUTES 10

typedef struct {
    char destination[MAX_NAME_LEN];
    char next_hop[MAX_NAME_LEN];
    int cost;
    char path[100];
} Route;

typedef struct {
    Route routes[MAX_ROUTES];
    int route_count;
} RoutingTable;

void build_static_routing_table(const Node *node, RoutingTable *table);
void print_routing_table(const Node *node, const RoutingTable *table);
Route* find_route(RoutingTable *table, const char *destination);
void simulate_send(const Node *node, RoutingTable *table, const char *destination, const char *message);

#endif