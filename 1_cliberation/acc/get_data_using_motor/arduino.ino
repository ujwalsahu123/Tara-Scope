#include <Wire.h>
#include <AccelStepper.h>
#include "SparkFun_ISM330DHCX.h"

// ---------------- Motor ----------------
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

#define STEPS_PER_REV 4096
#define STEP_10_DEG   (STEPS_PER_REV / 36) // ≈114

AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

// ---------------- Sensor ----------------
SparkFun_ISM330DHCX myISM;
sfe_ism_raw_data_t rawAccel;

// ---------------- Settings ----------------
#define AVG_MEAS 25

// ----------------------------------------

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!myISM.begin()) {
    Serial.println("ISM init failed");
    while (1);
  }

  myISM.deviceReset();
  while (!myISM.getDeviceReset());

  myISM.setDeviceConfig();
  myISM.setBlockDataUpdate();
  myISM.setAccelDataRate(ISM_XL_ODR_104Hz);
  myISM.setAccelFullScale(ISM_2g);

  stepper.setMaxSpeed(800);
  stepper.setAcceleration(400);

  Serial.println("READY");
}

void takeAndSendAvg() {
  long ax = 0, ay = 0, az = 0;

  for (int i = 0; i < AVG_MEAS; i++) {
    myISM.getRawAccel(&rawAccel);
    ax += rawAccel.xData;
    ay += rawAccel.yData;
    az += rawAccel.zData;
    delay(5);
  }

  ax /= AVG_MEAS;
  ay /= AVG_MEAS;
  az /= AVG_MEAS;

  // Send RAW LSB ONLY
  Serial.print(ax);
  Serial.print(",");
  Serial.print(ay);
  Serial.print(",");
  Serial.println(az);
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd == 's') {
      Serial.println("START");

      delay(2000);
      takeAndSendAvg();

      for (int i = 1; i < 36; i++) {
        stepper.move(STEP_10_DEG);
        stepper.runToPosition();

        delay(1000);
        takeAndSendAvg();
        delay(1000);
      }

      Serial.println("DONE");
    }

    if (cmd == 'q') {
      Serial.println("QUIT");
      while (1);
    }
  }
}
