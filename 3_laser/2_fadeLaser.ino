// fade in and out code 


// test out if works, if changes hai here , then also change in the 1.2_Maual_laser code.

// test which faderate and laserOntime is good . 
// and update it in 1.2_Maual_laser code.



#define LASER_PIN 9   // Use a PWM-capable pin (3, 5, 6, 9, 10, or 11 on UNO)

void setup() {
  pinMode(LASER_PIN, OUTPUT);
  digitalWrite(LASER_PIN, LOW);
}


// fadeLaser function - does fade in, hold, fade out
// fadeRate  = delay between brightness steps (controls fade speed)
// laserOnTime = how long the laser stays ON at full brightness
void fadeLaser(int fadeRate, int laserOnTime) {
  
    // 1️) Fade In
  for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(LASER_PIN, brightness);
    delay(fadeRate);  // itna time hold kro har brightness step pe
  } 
  // in the end its at full brightness (255)

  // 2️) Hold at full brightness
  delay(laserOnTime);

  // 3️) Fade Out
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(LASER_PIN, brightness);
    delay(fadeRate);
  }

  // Ensure it's fully off
  analogWrite(LASER_PIN, 0);
}

void loop() {
  fadeLaser(10, 2000);  // (faderate , laserOnTime) 
  
  delay(2000); 
  //no need to write this delay in the main motor pointing code. just write all of the above code & fucntion & function call
  //so after the motors have reached the target position, then simply call the fadeLaser() function.
}