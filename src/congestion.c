#include <stdio.h>
#include "../include/congestion.h"

static const char *state_label(State state) {
    switch (state) {
        case SLOW_START: return "Slow Start";
        case CONGESTION_AVOIDANCE: return "Congestion Avoidance";
        case FAST_RECOVERY: return "Fast Recovery";
        default: return "Unknown";
    }
}

static const char *event_label(LossEventType type) {
    switch (type) {
        case LOSS_EVENT_TIMEOUT: return "TIMEOUT";
        case LOSS_EVENT_TRIPLE_DUP_ACK: return "TRIPLE DUPLICATE ACK";
        default: return "NONE";
    }
}

static LossEventType event_at_round(const LossEvent *events, int event_count, int round) {
    if (events == NULL || event_count <= 0) {
        return LOSS_EVENT_NONE;
    }

    for (int i = 0; i < event_count; i++) {
        if (events[i].round == round) {
            return events[i].type;
        }
    }

    return LOSS_EVENT_NONE;
}

static int clamp_ssthresh(int ssthresh) {
    return (ssthresh < 1) ? 1 : ssthresh;
}

static void apply_timeout(TCP *tcp) {
    tcp->ssthresh = clamp_ssthresh(tcp->cwnd / 2);
    tcp->cwnd = 1;
    tcp->dup_ack_count = 0;
    tcp->state = SLOW_START;
}

static void apply_triple_dup_ack_tahoe(TCP *tcp) {
    tcp->ssthresh = clamp_ssthresh(tcp->cwnd / 2);
    tcp->cwnd = 1;
    tcp->dup_ack_count = 0;
    tcp->state = SLOW_START;
}

static void apply_triple_dup_ack_reno(TCP *tcp) {
    tcp->ssthresh = clamp_ssthresh(tcp->cwnd / 2);
    tcp->cwnd = tcp->ssthresh + 3;
    tcp->dup_ack_count = 3;
    tcp->state = FAST_RECOVERY;
}

void run_tahoe(const LossEvent *events, int event_count) {
    printf("\n--- TCP TAHOE ---\n");

    TCP tcp = {1, 8, 0, SLOW_START};

    for (int round = 1; round <= 10; round++) {
        printf("Round %d: cwnd=%d (ssthresh=%d) [%s]\n", round, tcp.cwnd, tcp.ssthresh, state_label(tcp.state));

        if (tcp.state == SLOW_START) {
            tcp.cwnd *= 2;
            if (tcp.cwnd >= tcp.ssthresh) {
                tcp.state = CONGESTION_AVOIDANCE;
            }
        } else if (tcp.state == CONGESTION_AVOIDANCE) {
            tcp.cwnd += 1;
        } else {
            // Tahoe has no fast recovery phase; treat as congestion avoidance.
            tcp.state = CONGESTION_AVOIDANCE;
        }

        LossEventType event = event_at_round(events, event_count, round);
        if (event != LOSS_EVENT_NONE) {
            printf("  Loss event at round %d: %s\n", round, event_label(event));

            if (event == LOSS_EVENT_TIMEOUT) {
                apply_timeout(&tcp);
                printf("  Action (Tahoe): ssthresh=cwnd/2, cwnd->1, state->Slow Start\n");
            } else if (event == LOSS_EVENT_TRIPLE_DUP_ACK) {
                printf("  Action (Tahoe): Fast Retransmit, then cwnd reset\n");
                apply_triple_dup_ack_tahoe(&tcp);
                printf("  After event: cwnd=%d (ssthresh=%d) [%s]\n", tcp.cwnd, tcp.ssthresh, state_label(tcp.state));
            }
        }
    }
}

void run_reno(const LossEvent *events, int event_count) {
    printf("\n--- TCP RENO ---\n");

    TCP tcp = {1, 8, 0, SLOW_START};

    for (int round = 1; round <= 10; round++) {
        printf("Round %d: cwnd=%d (ssthresh=%d) [%s]\n", round, tcp.cwnd, tcp.ssthresh, state_label(tcp.state));

        if (tcp.state == SLOW_START) {
            tcp.cwnd *= 2;
            if (tcp.cwnd >= tcp.ssthresh) {
                tcp.state = CONGESTION_AVOIDANCE;
            }
        } else if (tcp.state == CONGESTION_AVOIDANCE) {
            tcp.cwnd += 1;
        } else if (tcp.state == FAST_RECOVERY) {
            // Simplified Reno: assume a "new ACK" arrives in next RTT => exit fast recovery.
            printf("  (Fast Recovery) new ACK received => cwnd=ssthresh, state->Congestion Avoidance\n");
            tcp.cwnd = tcp.ssthresh;
            tcp.dup_ack_count = 0;
            tcp.state = CONGESTION_AVOIDANCE;
        }

        LossEventType event = event_at_round(events, event_count, round);
        if (event != LOSS_EVENT_NONE) {
            printf("  Loss event at round %d: %s\n", round, event_label(event));

            if (event == LOSS_EVENT_TIMEOUT) {
                apply_timeout(&tcp);
                printf("  Action (Reno): ssthresh=cwnd/2, cwnd->1, state->Slow Start\n");
            } else if (event == LOSS_EVENT_TRIPLE_DUP_ACK) {
                printf("  Action (Reno): Fast Retransmit / Fast Recovery\n");
                apply_triple_dup_ack_reno(&tcp);
                printf("  After event: cwnd=%d (ssthresh=%d) [%s]\n", tcp.cwnd, tcp.ssthresh, state_label(tcp.state));
            }
        }
    }
}