# CSE321-RealTime-Intrusion-Detection
Arduino-based real-time intrusion detection and response system using PIR motion sensors, reed switches, push-button arming, and buzzer/LED alerts. Developed for CSE 321 – Real-Time &amp; Embedded Systems.

# CSE321 Real-Time Intrusion Detection & Response System

### Overview
This project implements an **Arduino-based real-time intrusion detection and response system** that can sense motion, door/window openings, and user input events.  
It was developed as part of the **CSE 321 – Real-Time & Embedded Systems** course.

### 🎯 Features
PIR Motion Sensor (HC-SR501) – Detects movement

Reed Switch – Detects door/window openings

Push Button – Arms/disarms the system

LED Indicators – Red = armed/alarm, Green = safe

Buzzer – Audible alarm activation

OLED Display – Shows system state and alarm cause

Microphone Module – Noise-based intrusion detection

### 🧰 Hardware Components
| Component | Function | Arduino Pin |
| Component              | Function              | Arduino Pin   |
| ---------------------- | --------------------- | ------------- |
| PIR Motion Sensor      | Motion detection      | D2            |
| Reed Switch            | Door/window detection | D3            |
| Buzzer                 | Audible alarm         | D5            |
| Red LED                | Armed/Alarm indicator | D6            |
| Green LED              | Safe indicator        | D7            |
| Disarm Button          | Disarm control        | D8            |
| Arm Button             | Arm control           | D9            |
| OLED Display (SSD1306) | Status output         | I2C (SDA/SCL) |
| Microphone (MAX4466)   | Noise detection       | A0            |


### ⚙️ Software Overview
Uses a non-blocking Finite State Machine (FSM)
(DISARMED → ARMED → ALARM)
Includes debouncing, sensor calibration, and interrupt-safe logic
Designed using principles from real-time scheduling, synchronization, and embedded system timing constraints
- Written in **C / Arduino IDE**
- Uses **non-blocking finite-state machine** (IDLE → ARMED → ALARM)
- Fully tested through stand-alone unit tests under `/code/tests`
