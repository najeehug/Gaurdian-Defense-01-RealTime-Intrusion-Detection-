  // PIR Motion Sensor Test
  // HC-SR501 → Arduino UNO
  // VCC → 5V
  // OUT → D2
  // GND → GND

#define PIR_PIN 2

void setup() {
  pinMode(PIR_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("PIR Test: warming up...");
  delay(30000);  // 30-sec warm-up
  Serial.println("Ready! Move in front of sensor...");
}

void loop() {
  int motion = digitalRead(PIR_PIN);

  if (motion == HIGH) {
    Serial.println("Motion detected!");
    delay(1000);   // prevents flooding serial output
  } else {
    Serial.println("No motion");
    delay(1000);
  }
}
