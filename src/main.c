#include <stdio.h>
#include <string.h>
#include "../include/node.h"
#include "../include/routing.h"
#include "../include/congestion.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./node <config_file> [mode]\n");
        return 1;
    }

    Node node;
    RoutingTable routing_table;

    if (!load_config(argv[1], &node)) {
        return 1;
    }

    print_node_info(&node);

    build_static_routing_table(&node, &routing_table);
    print_routing_table(&node, &routing_table);

    // MODE SEÇİMİ
    if (argc >= 3) {

        if (strcmp(argv[2], "tahoe") == 0) {
            run_tahoe();
        }

        else if (strcmp(argv[2], "reno") == 0) {
            run_reno();
        }

        else if (strcmp(argv[2], "send") == 0) {
            simulate_send(&node, &routing_table, "D", "hello_from_A_to_D");
        }

        else if (strcmp(argv[2], "all") == 0) {
            run_tahoe();
            run_reno();
            simulate_send(&node, &routing_table, "D", "hello_from_A_to_D");
        }

        else {
            printf("Unknown mode!\n");
        }
    }

    return 0;
}