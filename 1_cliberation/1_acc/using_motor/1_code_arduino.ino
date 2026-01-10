// here we use motor to print the data.
// it prints - average of 25 values (we are doing averaging here only in arduino, and not in py)
// and then in py - it just reads and scales the value and then store it.

////// ARD_code_1 -> using single motor (single axis sensor rotation) (you need to manually change the next orientation and again press "s")
/////  ARD_code_2 -> using 2 motors (3 axis sensor rotation) (you only need to press "s" once)







// ###################################################### ARD_CODE_1 ######################################################

#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"
#include <CheapStepper.h>

SparkFun_ISM330DHCX myISM;
sfe_ism_raw_data_t rawAccel;

CheapStepper motorAlt(4, 5, 6, 7);

// IMP - so that loop run as per the data rate of sensor and not try to run as fast as possible 
unsigned int DataRate_HZ;	
unsigned long PERIOD_US;          
// time tracker for loops -> mainLoop(), while() , for()     
unsigned long lastRead = 0; 
unsigned long now = 0;


// #######################   CONFIG   ####################### 
#define AVG_MEAS  25 // take Avg of N readings to get 1 stable reading /per orientation.
const int STEPS_PER_REV = 4096;
const int N_orientations = 32 ;
//SET THIS //(value must be -> 2^N). [VALUES -> (1 = 360Deg) (2 = 180Deg) (4 = 90Deg) (8 = 45Deg) (16 = 22.5Deg) (32 = 11.25Deg) (64 = 5.625Deg) (128 = 2.8125Deg) (256 = 1.4Deg)]
const int STEPS_to_move_per_orientation = (STEPS_PER_REV / N_orientations) ;
// So if 4096/1_orientation => so 4096_steps_per_orientation to complete 1 full rotation. if 4096/2_orientaitons => so 2048_step)per_orientation and it will take 2 orientatins to complete 1 full rotation.      
// #############################################################

int count = 0; // iterator for Average readings
long ax = 0, ay = 0, az = 0; // Make Avg calculation 0 - before storing new avg calculations.

// function to calculte the avg readings and print it.
void Read_Accel_data()
{
  // take 25 readings ka AVG - for single orientation.
    now = 0;
    lastRead = 0;
    count = 0; // make iterator 0 before starting.
    ax = 0; ay = 0; az = 0; // Make Avg calculation 0 - before storing new avg calculations.
    while (count < AVG_MEAS)  // dont worry it will take N readings as per AVG_MEAS at 104/particual hz.
    {
        now = micros();
        if (now - lastRead >= PERIOD_US)
        {
            lastRead = now;
            if(myISM.getRawAccel(&rawAccel))
            {
                ax += rawAccel.xData;
                ay += rawAccel.yData;
                az += rawAccel.zData;
                count++;
            }
        }
    }

    // find AVG
    ax /= AVG_MEAS;
    ay /= AVG_MEAS;
    az /= AVG_MEAS;

    // Send AVG RAW LSB ONLY , and in python it will scale
    Serial.print(ax);
    Serial.print(",");
    Serial.print(ay);
    Serial.print(",");
    Serial.println(az);
}


void setup()
{
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
	
	// Wire.setClock(400000); // uncomment it if using 416hz or higher data rate , other wise comment it.

  // Range -> always need to set, even if scaling maunally
	myISM.setAccelFullScale(ISM_2g); 
	// test: in Motor_code if motor moves faster than 2g or 125dps then the sensor values can break - so in that senario we can do 4g or 250dps, but that can lower the precision. other jugad we can do is -> move motor slowly or etc.
	// if you wanna update the range of acc or gyro then - not just you have to change the scaling factor , but again calib (as per that_range_lsb_data * that_range_scaling_factor) and get new offset.


	// DataRate -> 104Hz is good (fast & low noise) // can also try 208 or more in SF since it needs frequent values.
	myISM.setAccelDataRate(ISM_XL_ODR_104Hz); // data rate (best -> 104, 208, 416) (for faster use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12Hz5, 1Hz6 )
	DataRate_HZ = 104; // keep same as sensor_Hz
  PERIOD_US = (1000000UL / DataRate_HZ); // using this we Control the iteration time.

	// Filter ->
	myISM.setAccelFilterLP2(false); // can keep it ON also, // later, TRY : ON in SensorFusion and see the results. 
    // myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_20); // can keep 20, 45 (not 10) // (if you keep the filter then based on the filter -> update the hardcoded offset/bias as per that)
	
	// fifo config (not much to do here, can try stream mode in SF, see 1_full_info.ino ka final setting section for more info)
	myISM.setFifoMode(ISM_BYPASS_MODE); // fifo off (default)
	
    
  motorAlt.setTotalSteps(STEPS_PER_REV);
  motorAlt.setRpm(10); // 6 to 24


	delay(100);
  Serial.println("Settings applied.");

}



void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd == 's') {
      Serial.println("START");
      delay(1000);

      // Moves N times to complete 360 degree. 
      for (int i = 0; i < N_orientations ; i++)  // take reading for full circle - 
      { 
        if(i == 0)
        {
          delay(2000); // keep
          Read_Accel_data(); // initail read before moving the motor (since it moves the motor first and then reads)
        }

        delay(200); // keep
        motorAlt.move(true, STEPS_to_move_per_orientation); // move to next orientation of current movement.
        delay(2000); // 2 sec delay to stabalize the vibration, after motor has moved to the new position.
        Read_Accel_data();
      }

      delay(1000);
      motorAlt.move(false, STEPS_PER_REV ); // return to inital position of Movement 1


      Serial.println("DONE");
      Serial.println(STEPS_PER_REV);
      Serial.println(N_orientations);
      Serial.println(STEPS_to_move_per_orientation);
  }


    if (cmd == 'q') {
      Serial.println("QUIT");
      while (1);
    }
  }
}






// ###################################################### ARD_CODE_2 ######################################################




#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"
#include <CheapStepper.h>

SparkFun_ISM330DHCX myISM;
sfe_ism_raw_data_t rawAccel;

CheapStepper motorAlt(4, 5, 6, 7);
CheapStepper motorAz(8, 9, 10, 11);

// IMP - so that loop run as per the data rate of sensor and not try to run as fast as possible 
unsigned int DataRate_HZ;	
unsigned long PERIOD_US;          
// time tracker for loops -> mainLoop(), while() , for()     
unsigned long lastRead = 0; 
unsigned long now = 0;

// #######################   CONFIG   ####################### 
#define AVG_MEAS  25 // take Avg of N readings to get 1 stable reading /per orientation.
const int STEPS_PER_REV = 4096;
const int N_orientations = 64 ;
//SET THIS //(value must be -> 2^N). [VALUES -> (1 = 360Deg) (2 = 180Deg) (4 = 90Deg) (8 = 45Deg) (16 = 22.5Deg) (32 = 11.25Deg) (64 = 5.625Deg) (128 = 2.8125Deg) (256 = 1.4Deg)]
const int STEPS_to_move_per_orientation = (STEPS_PER_REV / N_orientations) ;
// So if 4096/1_orientation => so 4096_steps_per_orientation to complete 1 full rotation. if 4096/2_orientaitons => so 2048_step)per_orientation and it will take 2 orientatins to complete 1 full rotation.      
// #############################################################

int count = 0; // iterator for Average readings
long ax = 0, ay = 0, az = 0; // Make Avg calculation 0 - before storing new avg calculations.

// function to calculte the avg readings and print it.
void Read_Accel_data()
{
    // take 25 readings ka AVG - for single orientation.
    now = 0;
    lastRead = 0;
    count = 0; // make iterator 0 before starting
    ax = 0; ay = 0; az = 0; // Make Avg calculation 0 - before storing new avg calculations.
    while (count < AVG_MEAS)  // take Multiple readings ka AVG - for single orientation - at particular Hz.
    {
        now = micros();
        if (now - lastRead >= PERIOD_US)
        {
            lastRead = now;
            if(myISM.getRawAccel(&rawAccel))
            {
                ax += rawAccel.xData;
                ay += rawAccel.yData;
                az += rawAccel.zData;
                count++;
            }
        }
    }

    // find AVG
    ax /= AVG_MEAS;
    ay /= AVG_MEAS;
    az /= AVG_MEAS;

    // Send AVG RAW LSB ONLY , and in python it will scale
    Serial.print(ax);
    Serial.print(",");
    Serial.print(ay);
    Serial.print(",");
    Serial.println(az);
}


void setup()
{
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
	
	// Wire.setClock(400000); // uncomment it if using 416hz or higher data rate , other wise comment it.

	
	// DataRate -> 104Hz is good (fast & low noise) // can also try 208 or more in SF since it needs frequent values.
	myISM.setAccelDataRate(ISM_XL_ODR_104Hz); // data rate (best -> 104, 208, 416) (for faster use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12Hz5, 1Hz6 )
	DataRate_HZ = 104; // keep same as sensor_Hz
    PERIOD_US = (1000000UL / DataRate_HZ); // using this we Control the iteration time.

    // Range -> always need to set, even if scaling maunally
	myISM.setAccelFullScale(ISM_2g); 
	// test: in Motor_code if motor moves faster than 2g or 125dps then the sensor values can break - so in that senario we can do 4g or 250dps, but that can lower the precision. other jugad we can do is -> move motor slowly or etc.
	// if you wanna update the range of acc or gyro then - not just you have to change the scaling factor , but again calib (as per that_range_lsb_data * that_range_scaling_factor) and get new offset.
	
  
	// Filter ->
	myISM.setAccelFilterLP2(false); // can keep it ON also, // later, TRY : ON in SensorFusion and see the results. 
    // myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_20); // can keep 20, 45 (not 10) // (if you keep the filter then based on the filter -> update the hardcoded offset/bias as per that)
	myISM.setGyroFilterLP1(false); // strictly off
	
	// fifo config (not much to do here, can try stream mode in SF, see ISM/1_full_info.ino ka final setting section for more info)
	myISM.setFifoMode(ISM_BYPASS_MODE); // fifo off (default)

  // set motor RPM (10 is good)  
  motorAlt.setTotalSteps(STEPS_PER_REV);
  motorAlt.setRpm(10); // 6 to 24
  motorAz.setTotalSteps(STEPS_PER_REV);
  motorAz.setRpm(10); // 6 to 24

	delay(100);
  Serial.println("Settings applied.");

}



void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd == 's') 
    {
      Serial.println("START");
      delay(1000);

      //////////////////////////// MOVEMENT 1 -> Alt motor full rotation - from Initial orientation
      for (int i = 0; i < N_orientations ; i++)  // take reading for full circle - 
      { 
        if(i == 0)
        {
          delay(2000); // keep
          Read_Accel_data(); // initail read before moving the motor (since it moves the motor first and then reads)
        }

        // first Motor-move and then take readings.
        delay(200); // keep
        motorAlt.move(true, STEPS_to_move_per_orientation); // move to next orientation of current movement.
        delay(2000); // 2 sec delay to stabalize the vibration, after motor has moved to the new position.
        Read_Accel_data();
      }

      delay(1000);
      motorAlt.move(false, STEPS_PER_REV ); // return to inital position of Movement 1


      ////////////////////////// Movement 2 -> AZ motor full rotation - From initial orientation. 
      for (int i = 0; i < N_orientations ; i++)  
      { 
        if(i == 0)
        {
          delay(2000); // keep
          Read_Accel_data(); 
        }
        
        delay(200); // keep
        motorAz.move(false, STEPS_to_move_per_orientation); // move to next orientation of current movement. using AZ motor .
        delay(2000); // 2 sec delay to stabalize the vibration, after motor has moved to the new position.
        Read_Accel_data();
      }

      // no need to return to initial posi now, we first go to movement3 starting position.
      delay(1000);
      motorAlt.move(true, (STEPS_PER_REV/4)); // Rotate alt motor +90 deg.

      ////////////////////////////////// Movement 3 -> Rotate alt motor by 90 deg or 4096/4, then AZ motor full rotation in opposite direction this time.
      for (int i = 0; i < N_orientations ; i++) 
      { 
        if(i == 0)
        {
          delay(2000); // keep
          Read_Accel_data(); 
        }

        delay(200); // keep
        motorAz.move(true, STEPS_to_move_per_orientation); // move to next orientation of current movement. Using AZ motor (opposite this time)
        delay(2000); 
        Read_Accel_data();
      }
      delay(1000);
      motorAlt.move(false, (STEPS_PER_REV/4)); // move the alt motor back to -90 degree (back to inital orientation)
      // So now we are OG back to OG inital orientation.
      
      delay(2000);
      Read_Accel_data(); // final read - just to check if we are where we started from (green / red dot on plot) // no need to check in 8_Auto_calib.

      Serial.println("DONE");
      Serial.println(STEPS_PER_REV);
      Serial.println(N_orientations);
      Serial.println(STEPS_to_move_per_orientation);
    }

    if (cmd == 'q') {
      Serial.println("QUIT");
      while (1);
    }
  }
}