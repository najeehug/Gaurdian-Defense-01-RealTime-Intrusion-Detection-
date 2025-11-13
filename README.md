# CSE321-RealTime-Intrusion-Detection
Arduino-based real-time intrusion detection and response system using PIR motion sensors, reed switches, push-button arming, and buzzer/LED alerts. Developed for CSE 321 – Real-Time &amp; Embedded Systems.

# CSE321 Real-Time Intrusion Detection & Response System

### Overview
This project implements an **Arduino-based real-time intrusion detection and response system** that can sense motion, door/window openings, and user input events.  
It was developed as part of the **CSE 321 – Real-Time & Embedded Systems** course.

### 🎯 Features
- **PIR Motion Sensor (HC-SR501)** – detects movement in monitored area  
- **Reed Switch** – detects door/window open events  
- **Push Button** – arms/disarms the system  
- **LED Indicators** – red = armed/alarm, green = safe  
- **Buzzer** – audible alarm on intrusion  
- **Microphone Module (optional)** – future TinyML “disarm by voice” upgrade  

### 🧰 Hardware Components
| Component | Function | Arduino Pin |
|------------|-----------|-------------|
| PIR Motion Sensor | Motion input | D2 |
| Reed Switch | Door/window input | D3 |
| Buzzer | Audible alert | D5 |
| Red LED | Alarm indicator | D6 |
| Green LED | Safe indicator | D7 |
| Push Button | Arm/Disarm control | D8 |
| Microphone (A0) | Optional sound trigger | A0 |

### ⚙️ Software Overview
- Written in **C++ / Arduino IDE**
- Uses **non-blocking finite-state machine** (IDLE → ARMED → ALARM)
- Fully tested through stand-alone unit tests under `/code/tests`
