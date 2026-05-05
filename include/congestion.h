#ifndef CONGESTION_H
#define CONGESTION_H

#define MAX_LOSS_EVENTS 16

typedef enum {
    SLOW_START,
    CONGESTION_AVOIDANCE,
    FAST_RECOVERY
} State;

typedef enum {
    LOSS_EVENT_NONE = 0,
    LOSS_EVENT_TIMEOUT,
    LOSS_EVENT_TRIPLE_DUP_ACK
} LossEventType;

typedef struct {
    int round;
    LossEventType type;
} LossEvent;

typedef struct {
    int cwnd;
    int ssthresh;
    int dup_ack_count;
    State state;
} TCP;

void run_tahoe(const LossEvent *events, int event_count);
void run_reno(const LossEvent *events, int event_count);

#endif