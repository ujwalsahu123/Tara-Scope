// simple code
#define LASER_PIN 7   // D41 PWM input pin

void setup() {
  pinMode(LASER_PIN, OUTPUT);
  digitalWrite(LASER_PIN, LOW);   // Start with laser off
}

void loop() {
  digitalWrite(LASER_PIN, HIGH);  // switch is turned on , thus Laser ON (full intensity)
  delay(2000);                    // Keep ON for 2 seconds
  digitalWrite(LASER_PIN, LOW);   // Laser OFF
  delay(2000);                    // Keep OFF for 2 seconds
}



  