# CSE320 — TCP Congestion Control Algorithms Comparison (C)

This repository contains a C-based simulation for comparing TCP congestion control behaviors and a simple 6-node routing/forwarding topology (A–F).

Implemented algorithms:
- TCP Tahoe
- TCP Reno

## Students / Algorithm Selection

Student IDs:
- 20210808042
- 20230808611

PDF rule: `(student1_id + student2_id) % 3`

Calculation:

```
(20210808042 + 20230808611) % 3 = 1
```

Selected main algorithm: **TCP Reno**

Secondary algorithm (comparison): **TCP Tahoe**

## Assignment Requirements Coverage (PDF Checklist)

- Single executable simulates TCP packet transmission (sender-side behavior): builds `node`
- Models sender-side congestion window (`cwnd`) and prints evolution step-by-step
- Packet loss events are configurable (command-line loss event pairs)
- Demonstrates both loss scenarios:
  - Timeout-based loss
  - Triple duplicate ACK loss
- Demonstrates the phases/behaviors:
  - Slow Start
  - Congestion Avoidance
  - Fast Retransmit / Fast Recovery (Reno)
- Uses the required six-node topology configuration files (A.conf–F.conf)
- Routing requirement: Node A routes to D via `A -> B -> D` (not direct `A -> D`)

## Project Structure

- configs/ — node topology config files (A.conf–F.conf)
- include/ — headers
- src/ — sources
- Makefile — build script

## Build

On Linux / WSL (Ubuntu):

```bash
make clean
make
```

Binary name: `node`

## Usage

General form:

```bash
./node <config_file> <mode> [args...]
```

### 1) Print node info + routing table

```bash
./node configs/A.conf
```

Expected: routing table for node A shows destination D with next hop B and path `A -> B -> D`.

### 2) Message forwarding (destination + message are arguments)

```bash
./node configs/A.conf send D hello_from_A_to_D
./node configs/F.conf send E hello_from_F_to_E
```

### 3) Congestion control simulation (configurable loss events)

Loss events are provided as pairs:

```bash
./node configs/A.conf tahoe <round> <timeout|triple> [<round> <timeout|triple>]...
./node configs/A.conf reno  <round> <timeout|triple> [<round> <timeout|triple>]...
```

Examples (explicitly requested by the assignment feedback):

```bash
./node configs/A.conf tahoe 5 timeout
./node configs/A.conf reno 6 triple
```

Multiple loss events in one run (useful for “multiple test cases / different loss patterns”):

```bash
./node configs/A.conf reno 6 triple 9 timeout
```

If you run without specifying any loss event, the program uses a default demo loss event and prints an info line.

### 4) Run default demos

```bash
./node configs/A.conf all
```

## Experimental Analysis (Multiple Test Cases)

The PDF asks for multiple test cases with different packet loss patterns and comparison using cwnd growth, throughput, and recovery speed.

In this simulation, the primary observable output is the round-by-round `cwnd` and the actions taken at loss events. With a fixed RTT assumption, throughput is proportional to `cwnd` per round (larger cwnd → more data in flight).

Recommended test cases (run and compare Tahoe vs Reno output):

1) Timeout-only
```bash
./node configs/A.conf tahoe 5 timeout
./node configs/A.conf reno  5 timeout
```

2) Triple-dup-ACK only
```bash
./node configs/A.conf tahoe 6 triple
./node configs/A.conf reno  6 triple
```

3) Mixed loss pattern (triple then timeout)
```bash
./node configs/A.conf tahoe 6 triple 9 timeout
./node configs/A.conf reno  6 triple 9 timeout
```

What to compare:
- cwnd growth shape (Slow Start vs Congestion Avoidance)
- Recovery speed after loss (how many rounds to return near pre-loss cwnd)
- Relative throughput proxy (cwnd level over time)