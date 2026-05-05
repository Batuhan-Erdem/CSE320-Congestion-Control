#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../include/node.h"
#include "../include/routing.h"
#include "../include/congestion.h"

static void print_usage(void) {
    printf("Usage:\n");
    printf("  ./node <config_file>\n");
    printf("  ./node <config_file> tahoe [<round> <timeout|triple>]...\n");
    printf("  ./node <config_file> reno  [<round> <timeout|triple>]...\n");
    printf("  ./node <config_file> send <DEST> <MESSAGE>\n");
    printf("  ./node <config_file> all\n\n");
    printf("Examples:\n");
    printf("  ./node configs/A.conf send D hello_from_A_to_D\n");
    printf("  ./node configs/F.conf send E hello_from_F_to_E\n");
    printf("  ./node configs/A.conf tahoe 5 timeout\n");
    printf("  ./node configs/A.conf reno 6 triple\n");
}

static int is_number(const char *s) {
    if (s == NULL || *s == '\0') {
        return 0;
    }
    for (const char *p = s; *p; p++) {
        if (!isdigit((unsigned char)*p)) {
            return 0;
        }
    }
    return 1;
}

static void to_lower_copy(char *dst, size_t dst_size, const char *src) {
    if (dst_size == 0) {
        return;
    }
    size_t i = 0;
    for (; src != NULL && src[i] != '\0' && i + 1 < dst_size; i++) {
        dst[i] = (char)tolower((unsigned char)src[i]);
    }
    dst[i] = '\0';
}

static LossEventType parse_event_type(const char *s) {
    char tmp[32];
    to_lower_copy(tmp, sizeof(tmp), s);
    if (strcmp(tmp, "timeout") == 0) {
        return LOSS_EVENT_TIMEOUT;
    }
    if (strcmp(tmp, "triple") == 0 || strcmp(tmp, "tripledup") == 0 || strcmp(tmp, "triple_dup") == 0) {
        return LOSS_EVENT_TRIPLE_DUP_ACK;
    }
    return LOSS_EVENT_NONE;
}

static int parse_loss_events_pairs(int argc, char *argv[], int start_index, LossEvent *events, int max_events) {
    int count = 0;
    for (int i = start_index; i + 1 < argc && count < max_events; i += 2) {
        if (!is_number(argv[i])) {
            break;
        }
        int round = atoi(argv[i]);
        LossEventType type = parse_event_type(argv[i + 1]);
        if (round <= 0 || type == LOSS_EVENT_NONE) {
            break;
        }
        events[count].round = round;
        events[count].type = type;
        count++;
    }
    return count;
}

static int default_loss_events_for_mode(const char *mode, LossEvent *events, int max_events) {
    if (events == NULL || max_events <= 0) {
        return 0;
    }

    if (strcmp(mode, "tahoe") == 0) {
        // Keep previous behavior as default demo.
        events[0] = (LossEvent){.round = 5, .type = LOSS_EVENT_TIMEOUT};
        return 1;
    }
    if (strcmp(mode, "reno") == 0) {
        // Keep previous behavior as default demo.
        events[0] = (LossEvent){.round = 6, .type = LOSS_EVENT_TRIPLE_DUP_ACK};
        return 1;
    }
    return 0;
}

static void join_message(char *out, size_t out_size, int argc, char *argv[], int start_index) {
    if (out_size == 0) {
        return;
    }
    out[0] = '\0';
    for (int i = start_index; i < argc; i++) {
        if (i != start_index) {
            strncat(out, " ", out_size - strlen(out) - 1);
        }
        strncat(out, argv[i], out_size - strlen(out) - 1);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
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
            LossEvent events[MAX_LOSS_EVENTS];
            int event_count = parse_loss_events_pairs(argc, argv, 3, events, MAX_LOSS_EVENTS);
            if (event_count == 0) {
                event_count = default_loss_events_for_mode("tahoe", events, MAX_LOSS_EVENTS);
                printf("\n[info] No loss event provided; using default: round=%d type=timeout\n", events[0].round);
            }
            run_tahoe(events, event_count);
        }

        else if (strcmp(argv[2], "reno") == 0) {
            LossEvent events[MAX_LOSS_EVENTS];
            int event_count = parse_loss_events_pairs(argc, argv, 3, events, MAX_LOSS_EVENTS);
            if (event_count == 0) {
                event_count = default_loss_events_for_mode("reno", events, MAX_LOSS_EVENTS);
                printf("\n[info] No loss event provided; using default: round=%d type=triple\n", events[0].round);
            }
            run_reno(events, event_count);
        }

        else if (strcmp(argv[2], "send") == 0) {
            if (argc < 5) {
                printf("Usage: ./node <config_file> send <DEST> <MESSAGE>\n");
                return 1;
            }
            const char *dest = argv[3];
            char message[256];
            join_message(message, sizeof(message), argc, argv, 4);
            simulate_send(&node, &routing_table, dest, message);
        }

        else if (strcmp(argv[2], "all") == 0) {
            LossEvent tahoe_events[MAX_LOSS_EVENTS];
            LossEvent reno_events[MAX_LOSS_EVENTS];
            int tahoe_count = default_loss_events_for_mode("tahoe", tahoe_events, MAX_LOSS_EVENTS);
            int reno_count = default_loss_events_for_mode("reno", reno_events, MAX_LOSS_EVENTS);
            run_tahoe(tahoe_events, tahoe_count);
            run_reno(reno_events, reno_count);
            simulate_send(&node, &routing_table, "D", "hello_from_A_to_D");
        }

        else {
            printf("Unknown mode!\n\n");
            print_usage();
        }
    }

    return 0;
}