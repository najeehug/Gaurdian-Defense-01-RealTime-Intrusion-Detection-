#define BUZZ_PIN 5
void setup(){ pinMode(BUZZ_PIN, OUTPUT); }
void loop(){
  for (int i=0;i<3;i++){          // three long beeps
    digitalWrite(BUZZ_PIN,HIGH);
    delay(2000);                  // 2 s on
    digitalWrite(BUZZ_PIN,LOW);
    delay(500);                   // 0.5 s off
  }
  delay(3000);                    // pause before repeating
}
