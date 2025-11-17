#define MIC_PIN A0

void setup() {
  Serial.begin(9600);
  Serial.println("Mic range test: quiet vs loud.");
}

void loop() {
  int minVal = 1023;
  int maxVal = 0;

  for (int i = 0; i < 300; i++) {
    int v = analogRead(MIC_PIN);
    if (v < minVal) minVal = v;
    if (v > maxVal) maxVal = v;
    delayMicroseconds(200);
  }

  Serial.print("Min: ");
  Serial.print(minVal);
  Serial.print("  Max: ");
  Serial.println(maxVal);
}
