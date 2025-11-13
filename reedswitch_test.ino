  // Reed Switch Test (Door sensor)
  // Fix for NC wiring 


#define REED_PIN 3
#define BUZZ_PIN 5

void setup() {
  pinMode(REED_PIN, INPUT_PULLUP);
  pinMode(BUZZ_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Reed switch test ready (NC logic).");
}

void loop() {
  int state = digitalRead(REED_PIN);

  // Flip logic: HIGH = CLOSED, LOW = OPEN
  if (state == HIGH) {
    Serial.println("Door CLOSED");
    digitalWrite(BUZZ_PIN, LOW);
  } else {
    Serial.println("Door OPEN - Triggering alarm!");
    digitalWrite(BUZZ_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZ_PIN, LOW);
  }

  delay(500);
}
