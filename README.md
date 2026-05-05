# TCP Congestion Control Algorithms Comparison (CSE320)

This project simulates TCP congestion control behavior and a simple 6-node routing/forwarding topology.

Implemented congestion control algorithms:
- TCP Tahoe
- TCP Reno

## Build

```bash
make
```

Output binary name is `node` (on Windows you may see `node.exe`).

## Run

General form:

```bash
./node <config_file> <mode> [args...]
```

### 1) Print node info + routing table

```bash
./node configs/A.conf
```

### 2) Message forwarding (destination and message are NOT hardcoded)

```bash
./node configs/A.conf send D hello_from_A_to_D
./node configs/F.conf send E hello_from_F_to_E
```

### 3) Congestion control simulation (configurable packet loss events)

Packet loss events are configurable from the command line using pairs:

```bash
./node configs/A.conf tahoe <round> <timeout|triple> [<round> <timeout|triple>]...
./node configs/A.conf reno  <round> <timeout|triple> [<round> <timeout|triple>]...
```

Examples requested by the assignment feedback:

```bash
./node configs/A.conf tahoe 5 timeout
./node configs/A.conf reno 6 triple
```

You can also provide multiple loss events (useful for the “multiple test cases” requirement):

```bash
./node configs/A.conf reno 6 triple 9 timeout
```

If you run without specifying a loss event, the program uses a default demo loss event (Tahoe: round 5 timeout, Reno: round 6 triple-dup ACK) and prints an info line explaining that.

### 4) Run all demos

```bash
./node configs/A.conf all
```

## Topology / Routing requirement

The static routing tables follow the assignment requirement that for node A, destination D must be routed as `A -> B -> D` (not directly `A -> D`).

## Notes

- The congestion control output prints `cwnd` evolution step-by-step and indicates the triggered loss event (timeout or triple-duplicate ACK) and the action taken.
- See `configs/` for the six-node topology (A–F).
