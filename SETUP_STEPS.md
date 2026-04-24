# TCP Congestion Control Algorithms Comparison

This project implements and compares TCP congestion control algorithms using C.

The goal is to simulate how TCP behaves under different network conditions, including packet loss and congestion scenarios.

Implemented algorithms:
- TCP Tahoe
- TCP Reno

The project also includes a simple network simulation with routing and message forwarding between nodes.


## CSE320 - TCP Congestion Control Assignment

## Step 1 — Algorithm Selection

Student IDs:
- 20230808611
- 20210808042

Calculation:
(20230808611 + 20210808042) % 3 = 1

Selected Algorithm:
- Main Algorithm: TCP Reno
- Secondary Algorithm (for comparison): TCP Tahoe

---

## Step 2 — Project Setup

Project directory created:

CSE320_Congestion_Assignment

### Folder Structure

- src/ → Source files
- include/ → Header files
- configs/ → Configuration files for nodes

### Files Created

#### Source Files (src/)
- main.c
- node.c
- routing.c
- congestion.c

#### Header Files (include/)
- node.h
- routing.h
- congestion.h

#### Config Files (configs/)
- A.conf
- B.conf
- C.conf
- D.conf
- E.conf
- F.conf

#### Other Files
- Makefile
- README.md

---

## Notes

This structure will support:
- Network simulation (nodes and routing)
- TCP congestion control algorithms (Reno & Tahoe)
- Testing and demonstration scenarios

## How to Compile and Run

### Compile

make

### Run Examples

Run TCP Tahoe:
./node configs/A.conf tahoe

Run TCP Reno:
./node configs/A.conf reno

Run message forwarding:
./node configs/A.conf send

Run all:
./node configs/A.conf all



## Step 3 — Configuration Files

Each node is defined with:
- Node name
- Port number
- Neighbor nodes with cost

### Example (Node A)

NODE A  
PORT 5001  

NEIGHBOR B 5002 4  
NEIGHBOR C 5003 7  
NEIGHBOR D 5004 13  
NEIGHBOR F 5006 5  

### Purpose

These configuration files define the network topology.
Routing decisions will be based on link costs.

Example:
- A → D direct cost = 13
- A → B → D cost = 12

Therefore:
Next hop from A to D = B

## Step 4 — Config Parser and Node Initialization

In this step, we implemented the first working part of the project.

### Files Updated

- include/node.h
- src/node.c
- src/main.c
- Makefile

### What Was Implemented

The program can now:

- Read a node configuration file
- Detect the node name
- Detect the node port
- Read all neighbors
- Read neighbor port numbers
- Read link costs
- Print the loaded node information

### Example Command

./node configs/A.conf

### Example Output

Node: A  
Port: 5001  
Neighbors:  
- B port=5002 cost=4  
- C port=5003 cost=7  
- D port=5004 cost=13  
- F port=5006 cost=5  

### Purpose

This step prepares the base node structure for routing and network simulation.
Each node can now understand its own configuration file.


## Step 5 — Static Routing Table and Next Hop Selection

In this step, we implemented static routing tables for all six nodes.

### Files Updated

- include/routing.h
- src/routing.c
- src/main.c

### What Was Implemented

The program can now:

- Build a routing table for each node
- Store destination, next hop, cost, and path information
- Print the routing table
- Find the correct next hop for a destination

### Important Requirement

For node A:

- A → D direct cost = 13
- A → B → D cost = 12

Therefore, the correct next hop from A to D is B.

### Example Output

Routing table for node A:

Destination | Next Hop | Cost | Path  
A | - | 0 | A  
B | B | 4 | A -> B  
C | C | 7 | A -> C  
D | B | 12 | A -> B -> D  
E | B | 7 | A -> B -> E  
F | F | 5 | A -> F  

### Purpose

This step prepares the forwarding logic.
When a message is sent to a destination, the node can now determine the correct next hop.

## Step 6 — Message Forwarding Simulation

In this step, we implemented message forwarding using routing tables.

### What Was Implemented

- Nodes can now send messages to a destination
- The correct next hop is selected using routing table
- The full path is followed step by step

### Example

Command:
send D hello_from_A_to_D

Output:
[A] Destination D, next hop B  
[B] Forwarding message from A to D, next hop D  
[D] Received message from A: hello_from_A_to_D    

### Purpose

This step simulates real packet forwarding in a network.
It ensures that messages follow the correct path based on routing decisions.

## Step 7 — TCP Congestion Control Simulation

In this step, we implemented TCP congestion control algorithms.

### Algorithms Implemented

- TCP Tahoe
- TCP Reno

### Features

- Slow Start phase
- Congestion Avoidance phase
- Timeout simulation (Tahoe)
- Triple duplicate ACK simulation (Reno)

### Behavior

Tahoe:
- Resets cwnd to 1 after timeout

Reno:
- Uses Fast Recovery after triple duplicate ACK
- Does not fully reset cwnd

### Output

The program prints cwnd evolution step-by-step for each round.

### Algorithm Comparison

TCP Tahoe:
- Resets cwnd to 1 after packet loss
- Always returns to Slow Start
- More conservative approach

TCP Reno:
- Uses Fast Recovery on triple duplicate ACK
- Does not reset cwnd to 1
- More efficient in handling packet loss

## Step 8 — Test Scenarios and Execution Modes

In this step, we added execution modes to control which part of the program runs.

### Modes

- tahoe → runs TCP Tahoe simulation
- reno → runs TCP Reno simulation
- send → runs message forwarding simulation
- all → runs everything

### Example Commands

./node configs/A.conf tahoe  
./node configs/A.conf reno  
./node configs/A.conf send  
./node configs/A.conf all  

### Purpose

This allows testing each algorithm separately and improves demonstration quality.

## Conclusion

In this project, we implemented and compared TCP Tahoe and TCP Reno algorithms.

We observed that:
- TCP Tahoe reacts more aggressively to packet loss
- TCP Reno improves performance by avoiding full restart

This simulation helped us understand how TCP adapts to network congestion and maintains reliable data transmission.