#include <stdio.h>
#include "../include/congestion.h"

void run_tahoe() {
    printf("\n--- TCP TAHOE ---\n");

    TCP tcp = {1, 8, 0, SLOW_START};

    for (int round = 1; round <= 10; round++) {

        printf("Round %d: cwnd=%d ", round, tcp.cwnd);

        if (tcp.state == SLOW_START) {
            printf("(Slow Start)\n");
            tcp.cwnd *= 2;

            if (tcp.cwnd >= tcp.ssthresh) {
                tcp.state = CONGESTION_AVOIDANCE;
            }
        } else {
            printf("(Congestion Avoidance)\n");
            tcp.cwnd += 1;
        }

        // simulate timeout at round 5
        if (round == 5) {
            printf("!!! TIMEOUT OCCURRED !!!\n");

            tcp.ssthresh = tcp.cwnd / 2;
            tcp.cwnd = 1;
            tcp.state = SLOW_START;
        }
    }
}

void run_reno() {
    printf("\n--- TCP RENO ---\n");

    TCP tcp = {1, 8, 0, SLOW_START};

    for (int round = 1; round <= 10; round++) {

        printf("Round %d: cwnd=%d ", round, tcp.cwnd);

        if (tcp.state == SLOW_START) {
            printf("(Slow Start)\n");
            tcp.cwnd *= 2;

            if (tcp.cwnd >= tcp.ssthresh) {
                tcp.state = CONGESTION_AVOIDANCE;
            }
        } else {
            printf("(Congestion Avoidance)\n");
            tcp.cwnd += 1;
        }

        // simulate triple duplicate ACK at round 6
        if (round == 6) {
            printf("!!! TRIPLE DUPLICATE ACK !!!\n");

            tcp.ssthresh = tcp.cwnd / 2;
            tcp.cwnd = tcp.ssthresh;
            tcp.state = CONGESTION_AVOIDANCE;
        }
    }
}