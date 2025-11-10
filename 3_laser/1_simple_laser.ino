// 1) define the pin number 
// #define LASER_PIN 7

// 2) in setup - set the pin mode to output . and set it low to start with laser off
// pinMode(LASER_PIN, OUTPUT);
// digitalWrite(LASER_PIN, LOW);

// 3) to turn on the laser at full intensity - set the pin high

//  digitalWrite(LASER_PIN, HIGH);  // (ON) Switch is ON thus the laser is ON (it will be ON until set off)
//  analogWrite(LASER_PIN, 0-255);  // (ON) controlled brightness (use a PWM digital pin) (it will be ON until set off)
//  digitalWrite(LASER_PIN, LOW);   // (OFF) so the Switch is now OFF and thus laser is OFF



//---------------------------------------------------------------------------------------




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


//---------------------------------------------------------------------------------------



// Brightness control code
#define LASER_PIN 9   // Use a PWM-capable pin (e.g., 3, 5, 6, 9, 10, or 11 on UNO)

void setup() {
  pinMode(LASER_PIN, OUTPUT);
}

void loop() {
  analogWrite(LASER_PIN, 255);  // 100% brightness (full ON)
  delay(2000);

  analogWrite(LASER_PIN, 128);  // 50% brightness
  delay(2000);

  analogWrite(LASER_PIN, 64);   // 25% brightness
  delay(2000);

  analogWrite(LASER_PIN, 0);    // 0% brightness (OFF)
  delay(2000);
}


//---------------------------------------------------------------------------------------


