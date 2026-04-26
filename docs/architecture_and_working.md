# System Architecture and Working

## Overview

This system implements a **real-time cyber-physical pipeline** on ESP32 using FreeRTOS, where sensing, communication, and control are handled as independent concurrent tasks.

The design ensures:
- Deterministic execution
- Safe inter-task communication
- Immediate response to external commands

---

## High-Level Architecture

The system consists of three core components:

1. **Sensor Task**
2. **WiFi (Communication) Task**
3. **Control Task**

These components interact through **FreeRTOS queues**, ensuring safe and structured data flow.

---

## Task Design

### 1. Sensor Task (Periodic Execution)

- Runs every 1000 ms  
- Generates temperature data  
- Sends data to `xUploadQueue`  

**Behavior**
```
REAL:<value>
```

**Key Concept**
- Time-triggered execution using `vTaskDelay()`

---

### 2. WiFi Task (Communication Layer)

- Connects to TCP server  
- Sends sensor data  
- Receives commands  
- Routes commands to Control Task  

**Responsibilities**
- Acts as a gateway between device and server  
- Decouples networking from control logic  

---

### 3. Control Task (Real-Time Actuation)

- Highest priority task  
- Blocks on `xCommandQueue`  
- Executes GPIO actions immediately  

**Commands**
```
CMD:COOL:ON   → Turn ON cooling (LED)
CMD:HEAT:ON   → Turn ON heating (LED)
CMD:IDLE      → Turn OFF all outputs
```

---

## Inter-Task Communication

Two queues are used:

### xUploadQueue
- Producer: Sensor Task  
- Consumer: WiFi Task  
- Purpose: Transfer sensor data  

---

### xCommandQueue
- Producer: WiFi Task  
- Consumer: Control Task  
- Purpose: Transfer control commands  

---

## Why Queues Instead of Shared Variables?

Using shared variables can cause:
- Race conditions  
- Data inconsistency  
- Unpredictable timing  

Queues ensure:
- Thread-safe communication  
- Blocking behavior (no busy waiting)  
- Deterministic execution  

---

## Scheduling Behavior

FreeRTOS scheduling rule:
> The highest-priority READY task is always executed

### Normal Operation

- Control Task → BLOCKED (waiting for commands)  
- Sensor Task → runs periodically  
- WiFi Task → runs when data is available  

---

### Command Arrival Scenario

1. WiFi Task receives command from server  
2. Command pushed to `xCommandQueue`  
3. Control Task unblocks immediately  
4. Scheduler preempts other tasks  
5. Control Task executes GPIO action  
6. Control Task blocks again  

---

## Execution Flow

1. Sensor Task generates data  
2. Data sent to `xUploadQueue`  
3. WiFi Task sends data to server  
4. Server processes data  
5. Server sends command  
6. WiFi Task receives command  
7. Command pushed to `xCommandQueue`  
8. Control Task executes action  

---

## Real-Time Behavior

This system achieves real-time behavior through:

- Priority-based scheduling  
- Blocking IPC (queues)  
- Separation of concerns (tasks)  

Key outcome:
> Control actions are never delayed by non-critical operations

---

## Design Principles Used

- Modular task-based architecture  
- Event-driven execution  
- Producer-consumer model  
- Separation of communication and control logic  

---

## Key Takeaways

- FreeRTOS enables predictable execution in embedded systems  
- Queues are essential for safe inter-task communication  
- Priority scheduling ensures real-time responsiveness  
- Embedded systems can be designed using principles similar to distributed systems  

---

## Possible Extensions

- Add latency measurement (command → actuation)  
- Implement fault tolerance (WiFi reconnection)  
- Support multiple ESP32 nodes  
- Replace TCP with MQTT for scalability  

---
