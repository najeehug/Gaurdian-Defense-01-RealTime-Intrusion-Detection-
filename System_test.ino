#define REED_PIN     3
#define PIR_PIN      2
#define MIC_PIN      A0
#define DISARM_BUTTON   8
#define ARM_BUTTON      9
#define LED_RED      6
#define LED_GREEN    7
#define BUZZ_PIN     5

enum State { IDLE, ARMED, ALARM };
State systemState = IDLE;

String alarmCause = "NONE";

//Microphone Calibration 
int MIC_BASELINE = 530;
int MIC_THRESHOLD = 130;

// Button Debounce
unsigned long lastDebounceArm = 0;
unsigned long lastDebounceDisarm = 0;
const unsigned long debounceDelay = 200;

unsigned long armedTimestamp = 0;

bool armPressed() {
  if (digitalRead(ARM_BUTTON) == LOW) {
    if (millis() - lastDebounceArm > debounceDelay) {
      lastDebounceArm = millis();
      return true;
    }
  }
  return false;
}

bool disarmPressed() {
  if (digitalRead(DISARM_BUTTON) == LOW) {
    if (millis() - lastDebounceDisarm > debounceDelay) {
      lastDebounceDisarm = millis();
      return true;
    }
  }
  return false;
}

// Alarm Pattern
void alarmPattern() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(BUZZ_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZ_PIN, LOW);
  digitalWrite(LED_RED, LOW);
  delay(200);
}

void deactivateAlarm() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(BUZZ_PIN, LOW);
}

void setup() {
  Serial.begin(9600);

  pinMode(REED_PIN, INPUT_PULLUP);
  pinMode(PIR_PIN, INPUT);
  pinMode(MIC_PIN, INPUT);

  pinMode(ARM_BUTTON, INPUT_PULLUP);
  pinMode(DISARM_BUTTON, INPUT_PULLUP);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUZZ_PIN, OUTPUT);

  Serial.println("SECURITY SYSTEM READY");
  Serial.println("ARM: RED  |  DISARM: GREEN");
}

void loop() {

  bool armButton = armPressed();
  bool disarmButton = disarmPressed();

  int reedRaw = digitalRead(REED_PIN);
  int pirMotionRaw  = digitalRead(PIR_PIN);
  int micValue = analogRead(MIC_PIN);

  bool doorOpen = (reedRaw == 0);
  bool motionDetected = (pirMotionRaw == HIGH);

  // Microphone Trigger
  bool loudSound = false;

  if (millis() - armedTimestamp > 1500) {
    if (abs(micValue - MIC_BASELINE) > MIC_THRESHOLD) {
      loudSound = true;
    }
  }

  switch (systemState) {

    // IDLE (Disarmed)
    case IDLE:
      deactivateAlarm();
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
      alarmCause = "NONE";

      if (armButton) {
        Serial.println("SYSTEM ARMED");
        systemState = ARMED;
        armedTimestamp = millis();
      }
      break;

    // ARMED 
    case ARMED:
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, HIGH);

      if (doorOpen) {
        alarmCause = "INTRUSION DETECTED: DOOR OPENED!";
        Serial.println(alarmCause);
        systemState = ALARM;
      }

      if (motionDetected) {
        alarmCause = "INTRUSION DETECTED: MOTION DETECTED!";
        Serial.println(alarmCause);
        systemState = ALARM;
      }

      if (loudSound) {
        alarmCause = "INTRUSION SUSPECTED: LOUD NOISE DETECTED!";
        Serial.println(alarmCause);
        systemState = ALARM;
      }

      // NORMAL disarming (single press)
      if (disarmButton) {
        Serial.println("SYSTEM DISARMED");
        systemState = IDLE;
      }
      break;

    // ALARM ACTIVE — must HOLD DISARM for 5 SECONDS
    case ALARM:
      digitalWrite(LED_GREEN, LOW);
      Serial.println(alarmCause);
      alarmPattern();

      // HOLD TO DISARM 5 seconds
      if (digitalRead(DISARM_BUTTON) == LOW) {

        unsigned long holdStart = millis();
        Serial.println("Hold disarm button for 5 seconds...");

        // Keep checking while the button is held
        while (digitalRead(DISARM_BUTTON) == LOW) {

          // If held for 5000ms → disarm
          if (millis() - holdStart >= 5000) {
            Serial.println("ALARM DISARMED (5-second hold)");
            deactivateAlarm();
            systemState = IDLE;
            delay(500);
            return;
          }

          alarmPattern(); // continue alarm while holding
        }

        // Button was released too early
        Serial.println("Disarm hold too short — ALARM CONTINUES");
      }
      break;
  }

  delay(20);
}
