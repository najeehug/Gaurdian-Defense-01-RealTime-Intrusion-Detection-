# **Real-Time Intrusion Detection System**

A real-time Arduino-based intrusion detection and response system using PIR motion sensing, reed-switch door monitoring, microphone noise detection, OLED feedback, LED indicators, and buzzer alarm.
Developed as a Real-Time & Embedded System.


## **📘 Overview**

This system monitors a protected area for **motion**, **door opening**, and **loud noise**, then triggers an alarm with visual and audible feedback.
A **finite-state machine (DISARMED → ARMED → ALARM)** and a **cyclic executive** are used to guarantee predictable real-time behavior.

The system is displayed on an **SSD1306 OLED** which shows the current mode and the cause of any alarm trigger.


## **🛠 Hardware Components**

| Component                  | Purpose                          | Arduino Pin |
| -------------------------- | -------------------------------- | ----------- |
| PIR Motion Sensor          | Detects human movement           | D2          |
| Reed Switch                | Detects door/window opening      | D3          |
| Microphone (MAX4466)       | Detects loud noises              | A0          |
| Buzzer                     | Audible alarm output             | D5          |
| Red LED                    | ARMED/ALARM indicator            | D6          |
| Green LED                  | DISARMED indicator               | D7          |
| Arm Button                 | Arms the system                  | D9          |
| Disarm Button              | Disarms the system               | D8          |
| OLED Display (SSD1306 I2C) | Shows system state & alarm cause | SDA/SCL     |


## **🔧 System States**

### **1. DISARMED**

* System is idle
* Green LED ON
* No alarms will trigger
* Press **ARM** to activate system

### **2. ARMED**

* Red LED ON
* System actively monitors all sensors
* Alarm triggers if any of the following occur:

  * **Door opens**
  * **Motion detected**
  * **Loud noise**

### **3. ALARM**

* Red LED flashes
* Buzzer beeps in rapid intervals
* OLED displays the cause:

  * **DOOR**
  * **MOTION**
  * **NOISE**

To stop the alarm, **hold the DISARM button for 3 seconds**.


## **🚀 How to Use the System**

### **➡️ Power On**

* OLED displays **SYSTEM STARTING**
* Microphone calibrates automatically
* System enters **DISARMED** mode


### **➡️ Arming the System**

Press the **ARM button (D9)**.

OLED shows:

```
ARMED
ACTIVE
```

Red LED turns ON.


### **➡️ What Triggers the Alarm?**

| Event               | Trigger Mechanism | Displayed Cause |
| ------------------- | ----------------- | --------------- |
| Door opens          | Reed switch LOW   | DOOR            |
| Motion detected     | PIR HIGH          | MOTION          |
| Loud noise detected | Microphone spike  | NOISE           |


### **➡️ Disarming the System (from ARMED)**

Press **DISARM (D8)** once.

OLED returns to:

```
DISARMED
IDLE
```

Green LED turns ON.


### **➡️ Stopping an Alarm**

To avoid accidental alarm cancellation:

**Hold the DISARM button for 3 seconds.**

OLED shows:

```
HOLD 3S
TO DISARM
```

After 3 seconds:

* Alarm stops
* LEDs reset
* System returns to DISARMED


## **📟 OLED Display Behavior**

The display updates only when:

* The system state changes
* The alarm cause changes

This prevents flicker and reduces I2C overhead during real-time execution.


## **🎤 Microphone Calibration**

On startup, the system:

* Reads 500 microphone samples
* Computes the quiet baseline
* Uses thresholds to detect loud events

This ensures **false alarms are minimized**.


## **⏱ Real-Time Cyclic Executive**

The system runs on a **4-frame, 10 ms per frame** cyclic executive:

| Frame # | Tasks Executed                                            |
| ------- | --------------------------------------------------------- |
| 0       | Read sensors, read mic, update alarm pattern              |
| 1       | Read sensors, read mic, update alarm pattern              |
| 2       | Read sensors, read mic, update alarm pattern              |
| 3       | Read sensors, read mic, update alarm pattern, update OLED |

This guarantees predictable periodic behavior and avoids blocking delays.
