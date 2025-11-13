  // Button Test
  // Verifies push button input.
  //   One leg → D8
  //   Other leg → GND

#define BTN_PIN 8

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);   
  Serial.begin(9600);
  Serial.println("Button Test Ready. Press the button!");
}

void loop() {
  int state = digitalRead(BTN_PIN);

  if (state == LOW) {
    Serial.println("Button pressed!");
    delay(200);  
  }
}
