#include <CheapStepper.h>

// Motor A (Azimuth) → ULN2003 IN1–IN4
CheapStepper motorA(8, 9, 10, 11);
// Motor B (Altitude) → ULN2003 IN1–IN4
CheapStepper motorB(4, 5, 6, 7);

const long STEPS_PER_REV = 4076;
const int RPM = 12;

void setup() {
  Serial.begin(9600);

  motorA.setTotalSteps(STEPS_PER_REV);
  motorB.setTotalSteps(STEPS_PER_REV);

  motorA.setRpm(RPM);
  motorB.setRpm(RPM);

  Serial.println("System ready:");
  Serial.println("↑ ↓ ← →  = Fast movement");
  Serial.println("1 2 3 4  = Precise 1-step move");
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();

    switch (c) {

      // ===== FAST MOTION (Arrow Keys) =====
      case 'U':   // Up arrow → Altitude CW
        motorB.move(true, 1);
        Serial.println("Motor B: UP");
        break;

      case 'D':   // Down arrow → Altitude CCW
        motorB.move(false, 1);
        Serial.println("Motor B: DOWN");
        break;

      case 'R':   // Right arrow → Azimuth CW
        motorA.move(true, 1);
        Serial.println("Motor A: RIGHT");
        break;

      case 'L':   // Left arrow → Azimuth CCW
        motorA.move(false, 1);
        Serial.println("Motor A: LEFT");
        break;


      // ===== PRECISE MOTION (Keys 1–4) =====
      // Each press triggers: wait 1s → 1 step → wait 1s

      case '1':   // Azimuth CW
        delay(1000);
        motorA.move(true, 1);
        delay(1000);
        Serial.println("Motor A: Precise CW");
        break;

      case '2':   // Azimuth CCW
        delay(1000);
        motorA.move(false, 1);
        delay(1000);
        Serial.println("Motor A: Precise CCW");
        break;

      case '3':   // Altitude CW
        delay(1000);
        motorB.move(true, 1);
        delay(1000);
        Serial.println("Motor B: Precise CW");
        break;

      case '4':   // Altitude CCW
        delay(1000);
        motorB.move(false, 1);
        delay(1000);
        Serial.println("Motor B: Precise CCW");
        break;


      default:
        break;
    }
  }
}
