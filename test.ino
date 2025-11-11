

#include <CheapStepper.h>

// -------- Motors --------
CheapStepper motorA(8, 9, 10, 11);   // Azimuth (ULN2003 IN1–IN4)
CheapStepper motorB(4, 5, 6, 7);     // Altitude (ULN2003 IN1–IN4)

const long STEPS_PER_REV = 4076;
const int RPM = 10;  // base RPM for normal moves

// ===== PRECISE CONTROL SETTINGS (w a s d) =====
const int PRECISE_STEP  = 1;     // steps per precise press  // keep it 1

// ===== Diamond settings =====
// test out
const int RADIUS_IN_STEPS = 50;      // <- radius (steps)
const int DIAMOND_DELAY_MS = 50;       // <- speed (ms per iteration)


// -------- Laser - spacebar - on/off --------
#define LASER_PIN 3   // PWM pin we can use this pin for both - digitalWrite and analogWrite
bool laserOn = false; 

void setLaser(bool on) {
  laserOn = on;
  digitalWrite(LASER_PIN, on ? HIGH : LOW);
}

// -------- Laser fade - --------
const int FADE_RATE_MS = 10;      // delay between fade steps
const int LASER_ON_TIME_MS = 1000; // how long laser stays ON at full brightness

void fadeLaser(int fadeRate, int laserOnTime) {
  Serial.println("Laser fade: starting");

  // 1) Fade In
  for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(LASER_PIN, brightness);
    delay(fadeRate);
  }

  // 2) Hold full brightness
  delay(laserOnTime);

  // 3) Fade Out
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(LASER_PIN, brightness);
    delay(fadeRate);
  }

  analogWrite(LASER_PIN, 0);
  Serial.println("Laser fade: done");
}

void setup() {
  pinMode(LASER_PIN, OUTPUT);
  setLaser(false); // start OFF

  Serial.begin(9600);

  motorA.setTotalSteps(STEPS_PER_REV);
  motorB.setTotalSteps(STEPS_PER_REV);

  motorA.setRpm(RPM);
  motorB.setRpm(RPM);

}

// ------------------------

inline void Diamond_step(bool azDir, bool altDir){
  motorA.move(azDir, 1);
  // delay(DIAMOND_DELAY_MS);  // test out 
  motorB.move(altDir, 1);
  delay(DIAMOND_DELAY_MS);
}

// ------------------------

void drawDiamond() {

  Serial.println("Drawing Diamond..."); 

  // 1) remember current center
  // no need to this - if we use move() instead of moveTo() when coming to the og position.
  long alt0 = motorB.getStep(); 
  long az0  = motorA.getStep();  

  // 2) go to bottom (ALT down by R)
  motorB.move(false,RADIUS_IN_STEPS);            // false = “down”
  delay(500);

  // 3) laser ON
  setLaser(true);

  // 4) four loops 

  // Loop1: AZ left R, ALT up R
  for(int i=0;i<RADIUS_IN_STEPS;i++) Diamond_step(false, true);

  // Loop2: AZ right R, ALT up R
  for(int i=0;i<RADIUS_IN_STEPS;i++) Diamond_step(true,  true);

  // Loop3: AZ right R, ALT down R
  for(int i=0;i<RADIUS_IN_STEPS;i++) Diamond_step(true,  false);

  // Loop4: AZ left R, ALT down R
  for(int i=0;i<RADIUS_IN_STEPS;i++) Diamond_step(false, false);

  // 5) laser OFF
  setLaser(false);

  delay(500);

  // 6) return to original pointing

  motorB.moveTo(true,  alt0);       // up 
  
  // we cannot simply do motorA.moveTo(az0) because it may take the long way around
  // ---- Smart Azimuth return ----
  long currentAz = motorA.getStep();
  long diff = az0 - currentAz;
  if (diff > 0)      motorA.move(true,  diff);
  else if (diff < 0) motorA.move(false, -diff);

    // or alternatively:
    // motorB.move(true, RADIUS_IN_STEPS);  // test out

    Serial.println("done");
}


void loop() {
  if (Serial.available()) {
    char c = Serial.read();

    switch (c) {
      // ===== FAST (arrows) =====
      case 'U': motorB.move(false, 1); break;  // Up  -> ALT down (your mapping)
      case 'D': motorB.move(true,  1); break;  // Down-> ALT up
      case 'R': motorA.move(true,  1); break;  // Right-> AZ CW
      case 'L': motorA.move(false, 1); break;  // Left -> AZ CCW

      // ===== precise (w a s d) =====
      case 'a': motorA.move(false, PRECISE_STEP); break;
      case 'd': motorA.move(true,  PRECISE_STEP); break;
      case 'w': motorB.move(false, PRECISE_STEP); break;
      case 's': motorB.move(true,  PRECISE_STEP); break;

      // ===== laser toggle (space) =====
      case 'T': setLaser(!laserOn);Serial.println(laserOn ? "LASER ON" : "LASER OFF"); break;

      // ===== diamond (press 'v') =====
      case 'v': drawDiamond(); break;

      // ===== fade laser (f) =====
      case 'f': fadeLaser(FADE_RATE_MS, LASER_ON_TIME_MS); break;

      default: break;
    }
  }
}



