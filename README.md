CSE321 Real-Time Intrusion Detection System – User Guide

📘 Overview

This Arduino-based real-time intrusion detection system monitors motion, door openings, and loud noise, and responds with LED indicators, OLED alerts, and a buzzer alarm.
The system is implemented using a cyclic executive and a finite state machine with three modes:

DISARMED

ARMED

ALARM

This guide explains how to operate the system once built.

🛠️ Hardware Input/Output Summary
| Component                  | Purpose                                    | Interaction               |
| -------------------------- | ------------------------------------------ | ------------------------- |
| **Arm Button (D9)**        | Arms the system                            | Press once                |
| **Disarm Button (D8)**     | Disarms the system / hold to silence alarm | Press or hold 3 seconds   |
| **PIR Motion Sensor (D2)** | Detects movement                           | Triggers alarm when armed |
| **Reed Switch (D3)**       | Detects door/window opening                | Triggers alarm when armed |
| **Microphone (A0)**        | Detects loud noise                         | Triggers alarm when armed |
| **Red LED (D6)**           | Shows ARMED/ALARM                          | ON when armed or alarming |
| **Green LED (D7)**         | Shows DISARMED                             | ON when system is safe    |
| **Buzzer (D5)**            | Audible alarm                              | Rapid beeping in ALARM    |
| **OLED Display**           | Shows system state & alarm cause           | Auto-updates              |


🚀 System Startup

When powered on:

The OLED shows “SYSTEM STARTING”

The microphone automatically performs calibration

System enters DISARMED mode

Display shows:

DISARMED
IDLE


You are now ready to use the system.

🔒 How to Arm the System

To arm the system:

➤ Press the ARM button (Pin 9)

The system will:

Turn GREEN LED OFF

Turn RED LED ON

Display:

ARMED
ACTIVE


While armed, the system monitors:

Door opening

Motion

Loud noise

🚨 What Triggers the Alarm?

When armed, the system enters ALARM mode if any of the following occurs:

Trigger	Cause on OLED
Door opens (reed sensor LOW)	“DOOR”
Motion detected (PIR HIGH)	“MOTION”
Loud noise detected	“NOISE”

The alarm response includes:

Flashing red LED

Buzzer siren pattern

OLED displays:

ALARM!
<CAUSE>

🧯 How to Disarm the System

You can disarm in two ways, depending on the current state:

1. Disarm from ARMED mode

Press the Disarm button (Pin 8) once.

System returns to:

DISARMED
IDLE


Green LED turns back on.

2. Disarm from ALARM mode

To prevent accidental shutdown, the user must:

➡ Hold the DISARM button for 3 seconds

The OLED shows:

HOLD 3S
TO DISARM


After 3 seconds:

Alarm stops

LEDs reset

State becomes DISARMED

Cause resets to “None”

🎛️ State Machine Summary
DISARMED

Green LED ON

Sensors active but alarms disabled

Press ARM → enters ARMED

ARMED

Red LED ON

Any sensor activity → ALARM

Press DISARM → returns to DISARMED

ALARM

Siren + red LED flashing

OLED shows alarm cause

Hold DISARM 3 seconds → resets system

📟 OLED Display Behavior

The display updates only when:

State changes

Alarm cause changes

This reduces flicker and follows the real-time cyclic executive design.

📡 Microphone Calibration

At startup:

500 readings are averaged

Baseline noise level is stored

Any sound +250 above baseline OR >900 absolute triggers noise alarm

This prevents false alarms by adapting to the room environment.

⏱️ Real-Time Cyclic Executive

The system runs on a 4-frame cyclic executive (10ms/frame):

| Frame | Tasks                                    |
| ----- | ---------------------------------------- |
| 0     | Read sensors + mic + alarm pattern       |
| 1     | Read sensors + mic + alarm pattern       |
| 2     | Read sensors + mic + alarm pattern       |
| 3     | Read sensors + mic + alarm + OLED update |

This ensures predictable, periodic sensor sampling and display updates.

✔️ How to Use the System (Quick Guide)

Power on → System calibrates & enters DISARMED

Press ARM → System is now active

Any intrusion → Alarm triggers

Hold DISARM for 3s → Alarm stops

Press DISARM (when armed) → Return to safe state
