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





// setup kar and test out the code. and update the fadeLaser code and controLaserSpacebar code (agar kuch changes hai too)


// the PWM pins quickly turns the current on and off
//PWM doesn’t actually change the current level — the current is always 0% or 100%. It simply turns the power ON and OFF very quickly (hundreds of times per second), and the ratio of ON vs OFF time makes our eyes perceive different brightness levels.
// 20% duty -> 20% time on , 80% time off 

// So also - need to check if it feels like the brightness is changing or does it feel like the light is flickering. 
// and if the light feels flickering at low duty , then in the fade code ->  instead of staring from 0 , we start from 10 , or 50 , etc. till 255. 





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

  