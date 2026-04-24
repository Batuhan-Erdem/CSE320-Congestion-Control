#ifndef CONGESTION_H
#define CONGESTION_H

typedef enum {
    SLOW_START,
    CONGESTION_AVOIDANCE
} State;

typedef struct {
    int cwnd;
    int ssthresh;
    int dup_ack_count;
    State state;
} TCP;

void run_tahoe();
void run_reno();

#endif