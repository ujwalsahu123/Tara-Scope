// here we use motor to print the data.
// it prints - average of 25 values (we are doing averaging here only in arduino, and not in py)
// and then in py - it just reads and scales the value and then store it.




/////////////////// update this code ka setup() as per 1_basic_ino

//////////////////// 2g , 104 hz , gyro-nofiler, ....   and acc-> NO,20,45 filter...






#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"
#include <CheapStepper.h>

CheapStepper motorA(4, 5, 6, 7);

const long STEPS_PER_REV = 4096;  

#define deg_to_move 5  ////////////////////////////////// just set this (10 , 5 , 2 ) so at each x degree it takes reading

const int Moves = 380 / deg_to_move ;   // keeping it 380 and not 360 since in 360 complete circle nahi hora tha.
// ex -  360 / 2 => 180 times it move to complete a circle, and each time it move 2 deg.

//const int steps_to_move = 4096 / Moves ;  // if you want to use motor.move() fucntion instead of moveDegrees().

// ---------------- Sensor ----------------
SparkFun_ISM330DHCX myISM;
sfe_ism_raw_data_t rawAccel;

// ---------------- Settings ----------------
#define AVG_MEAS 25

// ----------------------------------------

void setup() {
    Wire.begin();
    Serial.begin(115200);


    if (!myISM.begin()) {
        Serial.println("Could not initialize ISM330DHCX. Check connections.");
        while (1);
    }

    // Reset the device to default settings
    myISM.deviceReset();
    while (!myISM.getDeviceReset()) {
        delay(1);
    }
		Serial.println("Reset complete.");
		Serial.println("Applying settings.");
		delay(100);


    // Apply device settings

    myISM.setDeviceConfig();
    myISM.setBlockDataUpdate(); 

    Wire.setClock(400000); // Essential when running the accel and gyro at 416Hz or faster. // put this always.

    // Set accelerometer configuration
	myISM.setAccelDataRate(ISM_XL_ODR_208Hz); // data rate (best -> 104, 208, 416) (for faster use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12Hz5, 1Hz6 )
	myISM.setAccelFullScale(ISM_2g); // 2 , 4 , 8 ,16g  // no need , we do scaling manually 
	myISM.setAccelFilterLP2(true); // SF may filters -off try krna, since filtering does avg with old values. and sf also does the same. so try keeping it off. and see the result and decide - on / off. 
	myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_45); // 45 - balanced (low noise and fast responsiveness) // (fast & noisy) 10, 20, 45, 100 (slower response but less noisy)

    // Set gyroscope configuration
	myISM.setGyroDataRate(ISM_GY_ODR_208Hz); // data rate (best -> 104, 208, 416) (for fast use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12 )
	myISM.setGyroFullScale(ISM_125dps); // 125, 250, 500, 1000, 2000 4000dps // no need, we do scalin manually
	myISM.setGyroFilterLP1(true); // try off in sf - and see responsivess. and deicide - keeping it on or off.
	myISM.setGyroLP1Bandwidth(ISM_LIGHT); // ISM_LIGHT is balanced //  (fast & noisy) ISM_ULTRA_LIGHT, ISM_VERY_LIGHT,ISM_LIGHT, ISM_MEDIUM (Slower response, less noise)

	// fifo config (not much to do here, can try stream mode in SF)
	myISM.setFifoMode(ISM_BYPASS_MODE); // default is bypass - Sensor always exposes ONLY the latest sample and fifo is off.  (ISM_FIFO_MODE) FIFO buffer fills until full, then STOPS writing new samples. (ISM_STREAM_MODE) for contineous updating old data circular buffer (old data overwritten by new data).
	// below functions use only if fifo is enabled (fifo_mode, stream_mode)
	// myISM.setAccelFifoBatchSet(ISM_XL_BATCH_AT_104Hz); // keep same Hz as data rate or differnt as per how you want to store the readings
	// myISM.setGyroFifoBatchSet(ISM_GY_BATCH_AT_104Hz) ; // 
	// lets try FIFO_mode and Stream_mode in SF (maybe it can solve the laggy problem).....

  motorA.setTotalSteps(STEPS_PER_REV);
  motorA.setRpm(12);

  delay(100);
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
      delay(1000);
      // Moves X times to complete 360 degree. 
      for (int i = 0; i < Moves; i++) { 
        // take reading
        delay(2000); // wait
        takeAndSendAvg();
        delay(200); //   wait
        // move to next posi
        motorA.moveDegrees(true, deg_to_move);
      }
      Serial.println("DONE");
      motorA.moveDegrees(false,380);  // back to the starting orientation
    }

    

    if (cmd == 'q') {
      Serial.println("QUIT");
      while (1);
    }
  }
}
