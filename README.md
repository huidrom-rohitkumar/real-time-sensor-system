# Real-Time CPS System using FreeRTOS on ESP32

## Overview
This project implements a **real-time cyber-physical system (CPS)** on ESP32 using FreeRTOS, where sensing, communication, and actuation are handled as concurrent tasks.

The system demonstrates how **real-time embedded software interacts with network services**, forming a complete pipeline from sensor data acquisition to remote command-based actuation.

It highlights **task scheduling, inter-task communication, and distributed system behavior**.

---

## Key Features
- Priority-based task scheduling using FreeRTOS  
- Concurrent execution of sensing, communication, and control tasks  
- Inter-task communication using queues (safe and deterministic IPC)  
- TCP-based communication between ESP32 and server  
- Event-driven actuator control with minimal latency  
- End-to-end real-time pipeline (sensor → network → control)  

---

## System Design

The system is divided into three core tasks:

### 1. Sensor Task (Periodic)
- Runs every 1 second  
- Generates temperature data  
- Sends data to upload queue  

### 2. WiFi Task (Communication Layer)
- Connects ESP32 to TCP server  
- Sends sensor data  
- Receives control commands  
- Routes commands to control task  

### 3. Control Task (Real-Time Actuation)
- Highest priority task  
- Blocks on command queue  
- Executes GPIO actions immediately  

---

## Inter-Task Communication

Two FreeRTOS queues are used:

- **xUploadQueue**  
  - Carries sensor data from Sensor Task to WiFi Task  

- **xCommandQueue**  
  - Carries validated commands from WiFi Task to Control Task  

This ensures:
- No race conditions  
- Deterministic task behavior  
- Decoupled system components  

---

## Task Scheduling

| Task          | Priority | Behavior            |
|--------------|---------|---------------------|
| Control Task | High    | Event-driven        |
| Sensor Task  | Medium  | Periodic (1s)       |
| WiFi Task    | Low     | Event-driven        |

FreeRTOS always executes the **highest-priority ready task**, ensuring immediate actuation on command arrival.

---

## Communication Protocol

### Sensor Data Format
```
REAL:<value>
```

### Command Format
```
CMD:COOL:ON
CMD:HEAT:ON
CMD:IDLE
```

---

## Execution Flow

1. Sensor Task generates data  
2. Data sent to WiFi Task via queue  
3. WiFi Task transmits data to server  
4. Server processes and sends command  
5. WiFi Task forwards command  
6. Control Task executes action instantly  

---

## Sample Output

```
[SensorTask] REAL:31
[WiFiTask] Sent: REAL:31
[WiFiTask] Received CMD:COOL:ON
[ControlTask] CMD:COOL:ON
```

---

## Hardware & Software

### Hardware
- ESP32 Development Board  

### Software
- Arduino IDE  
- FreeRTOS (ESP32 Arduino Core)  
- Python TCP Server (for backend communication)  

---

## Repository Structure

```
real-time-cps-freertos/
│
├── esp32/
│   └── main.ino
│
├── server/
│   ├── tcp_server.py
│   └── chatbot_client.py
│
├── README.md
└── requirements.txt
```

---

## How to Run

### 1. Start Server
```
python server/tcp_server.py
```

### 2. Configure ESP32
- Update WiFi credentials  
- Set server IP  

### 3. Upload Code
- Open `esp32/main.ino` in Arduino IDE  
- Upload to ESP32  

### 4. Monitor Output
- Use Serial Monitor  

---

## What This Project Demonstrates

- Real-time task scheduling in embedded systems  
- Safe inter-task communication using queues  
- Integration of embedded systems with network services  
- Event-driven system design  
- Practical application of OS concepts (scheduling, blocking, IPC)  

---

## Future Improvements

- Add latency measurement (command → actuation time)  
- Implement fault tolerance and reconnection logic  
- Support multiple ESP32 nodes  
- Replace TCP with MQTT for scalability  
- Deploy backend on cloud  

---

## Note
This project bridges **embedded systems and backend communication**, demonstrating how real-time devices integrate into distributed systems.

---

## Author
Rohit
