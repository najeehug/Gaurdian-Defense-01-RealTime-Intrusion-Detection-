// Include I2C communication library
#include <Wire.h>
// Include graphics library for text and shapes
#include <Adafruit_GFX.h>
// Include SSD1306 OLED library
#include <Adafruit_SSD1306.h>

// Create OLED display object (128x64 resolution, using I2C, no reset pin)
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// Pins
#define ARM_BUTTON      9
#define DISARM_BUTTON   8
#define LED_RED         6
#define LED_GREEN       7
#define BUZZER_PIN      5
#define REED_PIN        3
#define PIR_PIN         2
#define MIC_PIN         A0

// Three States
enum State { DISARMED, ARMED, ALARM };
// Current state and previously displayed state
State systemState = DISARMED, lastState = DISARMED;

// Cause of last alarm and previous cause (for OLED update)
String alarmCause = "None", lastCause = "None";

// Microphone
// Quiet baseline level after calibration
int quietLevel = 0;

// Timestamp of last loud-noise detection
unsigned long lastTrigger = 0;

// Cyclic Executive timing
// 10 ms per frame
const unsigned long FRAME_SIZE = 10; 
unsigned long lastFrameTime = 0;
// Current frame number (0–3)
int frameNumber = 0;

// Sensor status flags
// True if door open detected
bool doorOpen = false;
// True if motion detected
bool motionDetected = false;
// True if loud noise detected
bool noiseDetected = false;

// OLED function
//Display two lines of text
void showOLED(const String &line1, const String &line2) {
  //clear screen
  display.clearDisplay();
  //white text black background 
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  //Big text
  display.setTextSize(2);
  // Top line
  display.setCursor(0, 0);
  // prints out the firstline
  display.println(line1);
  //Bottom line
  display.setCursor(0, 32);
  // prints out the second line
  display.println(line2);
  //update display
  display.display();
}

void initOLED() {
  // Start I2C
  Wire.begin();
  // A fast I2C speed
  Wire.setClock(400000);
  // Start OLED at address 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // Change brightness & set max contrast
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(0xFF);

  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 16);
  display.println("SYSTEM");
  display.setCursor(0, 40);
  display.println("STARTING");
  display.display();
  delay(500);
}

// MIC CALIBRATION
void calibrateMic() {
  long sum = 0;
  // Average 500 microphone readings to determine quiet baseline
  for (int i = 0; i < 500; i++) {
    // Read microphone
    sum += analogRead(MIC_PIN);
    delay(2);
  }
  // Compute average
  quietLevel = sum / 500;
}

// TASKS
void task_readSensors() {
  doorOpen       = (digitalRead(REED_PIN) == LOW);
  motionDetected = (digitalRead(PIR_PIN) == HIGH);
}

bool loudNoiseDetected() {
  // Read mic value
  int v = analogRead(MIC_PIN);
  // Prevent rapid triggers
  if (v > 900 && millis() - lastTrigger > 300) {
    lastTrigger = millis();
    return true;
  }
  // Sudden jump relative to quiet baseline
  if (v - quietLevel > 250 && millis() - lastTrigger > 300) {
    lastTrigger = millis();
    return true;
  }

  return false;
}
// Task to update noiseDetected flag
void task_mic() {
  noiseDetected = loudNoiseDetected();
}
// Update OLED only when state or cause changes
void task_oled() {
  if (systemState == lastState && alarmCause == lastCause) return;

  if (systemState == DISARMED)
    showOLED("DISARMED", "IDLE");
  else if (systemState == ARMED)
    showOLED("ARMED", "ACTIVE");
  else
    showOLED("ALARM!", alarmCause);

  lastState = systemState;
  lastCause = alarmCause;
}

// Alarm pattern
bool alarmOutputOn = false;
unsigned long alarmToggle = 0;
// Alarm siren pattern: 200ms ON, 120ms OFF
void task_alarmPattern() {
  // Only run during ALARM
  if (systemState != ALARM) return;

  unsigned long now = millis();

  if (alarmOutputOn) {
    if (now - alarmToggle >= 200) {
      noTone(BUZZER_PIN);
      digitalWrite(LED_RED, LOW);
      alarmOutputOn = false;
      alarmToggle = now;
    }
  } else {
    if (now - alarmToggle >= 120) {
      tone(BUZZER_PIN, 2500);
      digitalWrite(LED_RED, HIGH);
      alarmOutputOn = true;
      alarmToggle = now;
    }
  }
}

// BUTTONS
unsigned long lastArmPress = 0;
unsigned long lastDisarmPress = 0;

// Detect ARM button press with debounce
bool armPressed() {
  if (digitalRead(ARM_BUTTON) == LOW && millis() - lastArmPress > 250) {
    lastArmPress = millis();
    return true;
  }
  return false;
}
// Detect DISARM button press with debounce
bool disarmPressed() {
  if (digitalRead(DISARM_BUTTON) == LOW && millis() - lastDisarmPress > 250) {
    lastDisarmPress = millis();
    return true;
  }
  return false;
}

// STATE MACHINE + DISARM HOLD
unsigned long disarmHoldStart = 0;
bool holdingDisarm = false;

void updateSystemState() {

  // DISARMED STATE
  if (systemState == DISARMED) {
    digitalWrite(LED_GREEN, HIGH);

    if (armPressed()) {
      systemState = ARMED;
      alarmCause = "None";
    }
    return;
  }

  // ARMED STATE
  if (systemState == ARMED) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);

    if (doorOpen) { systemState = ALARM; alarmCause = "DOOR"; }
    else if (motionDetected) { systemState = ALARM; alarmCause = "MOTION"; }
    else if (noiseDetected) { systemState = ALARM; alarmCause = "NOISE"; }

    if (disarmPressed()) {
      systemState = DISARMED;
      alarmCause = "None";
    }

    return;
  }

  // Must HOLD disarm button 3 seconds
  if (systemState == ALARM) {

    if (digitalRead(DISARM_BUTTON) == LOW) {

      if (!holdingDisarm) {
        holdingDisarm = true;
        disarmHoldStart = millis();
        showOLED("HOLD 3S", "TO DISARM");
      }

      if (millis() - disarmHoldStart >= 3000) {
        // Disarm after 3 seconds
        systemState = DISARMED;
        alarmCause = "None";
        noTone(BUZZER_PIN);
        digitalWrite(LED_RED, LOW);
        holdingDisarm = false;
      }

    } else {
      holdingDisarm = false;
    }
  }
}

// SETUP
void setup() {
  Serial.begin(9600);

  initOLED();

  pinMode(ARM_BUTTON, INPUT_PULLUP);
  pinMode(DISARM_BUTTON, INPUT_PULLUP);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(REED_PIN, INPUT_PULLUP);
  pinMode(PIR_PIN, INPUT);
  pinMode(MIC_PIN, INPUT);

  digitalWrite(LED_GREEN, HIGH);

  calibrateMic();
  showOLED("DISARMED", "IDLE");
}

// CYCLIC EXECUTIVE
void loop() {
  // Check if frame duration has passed
  if (millis() - lastFrameTime < FRAME_SIZE) return;
  // Update frame timestamp
  lastFrameTime = millis();
  // Select which task set to run based on frame number
  switch (frameNumber) {

    case 0:
      task_readSensors();
      task_mic();
      task_alarmPattern();
      break;

    case 1:
      task_readSensors();
      task_mic();
      task_alarmPattern();
      break;

    case 2:
      task_readSensors();
      task_mic();
      task_alarmPattern();
      break;

    case 3:
      task_readSensors();
      task_mic();
      task_alarmPattern();
      // Update OLED only once per cycle
      task_oled();
      break;
  }
  // Update system mode
  updateSystemState();
  // Next frame (0–3)
  frameNumber = (frameNumber + 1) % 4;
}
