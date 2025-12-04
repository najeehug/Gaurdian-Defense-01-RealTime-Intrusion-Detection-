#define MIC_PIN A0  

// Baseline noise level of the room 
int quietLevel = 0;   

// Timestamp of the last detected loud noise event   
unsigned long lastTrigger = 0;  

void setup() {
  Serial.begin(9600);
  delay(500);  // Small delay for serial monitor stability

  // Microphone Calibration Routine
  // Determine the average ambient noise level ("quiet level")
  long sum = 0;
  for (int i = 0; i < 500; i++) {
    // Read ambient microphone voltage
    sum += analogRead(MIC_PIN); 
    // Short delay for stable sampling 
    delay(2);                    
  }

  // Compute the average quiet value from 500 samples
  quietLevel = sum / 500;

  Serial.print("Quiet Level: ");
  Serial.println(quietLevel);
}

bool loudNoiseDetected() {

  // Read current microphone value
  int v = analogRead(MIC_PIN);  

  // Peak detection (absolute threshold)
  if (v > 900) {
    // Enforces a cooldown period of 300 ms to prevent duplicate triggers.
    if (millis() - lastTrigger > 300) {
      lastTrigger = millis();
      return true;
    }
  }

  // Relative jump detection (quiet baseline threshold)
  // Detects sudden changes relative to the calibrated quiet level.
  if (v - quietLevel > 250) {
    // Enforce same 300ms cooldown
    if (millis() - lastTrigger > 300) {
      lastTrigger = millis();
      return true;
    }
  }
// No loud event detected
  return false;  
}

void loop() {
  // Check microphone for loud noise events
  if (loudNoiseDetected()) {
    Serial.println("LOUD NOISE DETECTED!");
  }
}
