
// cheapstepper INFO : -------------------------------------------------------------------------------------------



// setTotalSteps() - 4076 / 4096
// setRpm() - ?  test kar

// Blocking : 
// move(bool, steps)
// moveto(bool, step_position)
// moveDegrees(bool, degrees)
// moveToDegrees(bool, degree_position)

// Non-blocking :
// .step(bool)  - immediate single step ; dosent need .run()
// rest all non-blocking need .run() in loop
// .newMove(bool, steps)
// .newMoveTo(bool, step_position)
// .newMoveDegrees(bool, degrees)
// .newMoveToDegrees(bool, degree_position)






// simple arduino code of stepper motor
----------------------------------------------------------------------------------------------------------

#include <CheapStepper.h>
// Motor A connected to ULN2003 IN1–IN4
CheapStepper motorA(8, 9, 10, 11);

const long STEPS_PER_REV = 4076;  // instead of 4096

void setup() {
  Serial.begin(9600);
  Serial.println("Starting CheapStepper test...");

  motorA.setTotalSteps(STEPS_PER_REV);
  motorA.setRpm(12);

  Serial.print("RPM set to: ");
  Serial.println(motorA.getRpm());
  Serial.print("Steps per revolution: ");
  Serial.println(STEPS_PER_REV);
}

void loop() {
  // Move one full revolution clockwise, 1 step at a time
  for (int i = 0; i < STEPS_PER_REV; i++) {
    motorA.move(true, 1);  // move 1 step CW
    delay(3);              // small delay for smoothness
  }

  Serial.println("✅ One full CW rotation completed!");

  delay(1000);  // pause before reversing

  // Move one full revolution counterclockwise
  motorA.move(false, STEPS_PER_REV);  // blocking move for 4076 steps

  Serial.println("✅ One full CCW rotation completed!");

  while (1);  // stop here forever
}