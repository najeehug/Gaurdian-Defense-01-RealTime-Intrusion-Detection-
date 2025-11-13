// LED Test – verifies that both LEDs light correctly.
// Connect:
//   Red LED  → D6 (through 220 Ω resistor)
//   Green LED → D7 (through 220 Ω resistor)
//   Both short legs → GND


#define LED_RED   6
#define LED_GREEN 7

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  Serial.begin(9600);
  Serial.println("LED Test: starting...");
}

void loop() {
  Serial.println("Red ON, Green OFF");
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  delay(1000);

  Serial.println("Red OFF, Green ON");
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  delay(1000);
}
