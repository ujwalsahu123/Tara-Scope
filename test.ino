


#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"
#include <CheapStepper.h>

SparkFun_ISM330DHCX myISM;
sfe_ism_raw_data_t rawAccel;
sfe_ism_raw_data_t rawGyro;

CheapStepper motorAlt(4, 5, 6, 7);
CheapStepper motorAz(8, 9, 10, 11);

// IMP - so that loop run as per the data rate of sensor and not try to run as fast as possible 
unsigned int DataRate_HZ;	
unsigned long PERIOD_US;          
// time tracker for loops -> mainLoop(), while() , for()     
unsigned long lastRead = 0; 
unsigned long now = 0;

// sending n decimal data to python 
int n = 6;


// #######################   CONFIG GYRO   ####################### 
#define GYRO_Scale_DPS   0.004375f      // scaling to 125 dps
#define GYRO_CALIB_SAMPLES  1000         // number of samples to collect for bias calculation
double gyro_bias[3] = { 44.295409, -169.449102, 72.213573}; // // Hardcode Gyro bias_lsb, not accurate
// ##########################################################



// #######################   CONFIG ACCEL  ####################### 
#define ACC_LSB_TO_G 0.00006103515625f   // 2g range
#define AVG_MEAS  25 // take Avg of N readings to get 1 stable reading /per orientation.

const int STEPS_PER_REV = 4096;
const int N_orientations = 64 ;
//SET THIS //(value must be -> 2^N). [VALUES -> (1 = 360Deg) (2 = 180Deg) (4 = 90Deg) (8 = 45Deg) (16 = 22.5Deg) (32 = 11.25Deg) (64 = 5.625Deg) (128 = 2.8125Deg) (256 = 1.4Deg)]
const int STEPS_to_move_per_orientation = (STEPS_PER_REV / N_orientations) ;
// So if 4096/1_orientation => so 4096_steps_per_orientation to complete 1 full rotation. if 4096/2_orientaitons => so 2048_step)per_orientation and it will take 2 orientatins to complete 1 full rotation.      

// Hardcoded offset and bias - (g_2g_Nofilter_0.97859) 
// change it as per filter you use.
// 1 time Calib when in new Location.
double A_acc[3][3] = 
{{0.97522389, 0.00009342, -0.00058922},
 {0.00009342, 0.98066595, 0.00053047},
 {-0.00058922, 0.00053047, 0.98232234}};
double b_acc[3] = { 0.00500114, -0.01523379, 0.01161619}; 
// ################################################################



double rawAcc[3] =  {0.0, 0.0, 0.0};
double calibAccel[3] = {0.0, 0.0, 0.0};
double calibGyro[3] = {0.0,0.0,0.0} ;
long count = 0; // iterator for Average readings



// =======================================================
// CALCULATE GYRO BIAS 
// =======================================================
long gX = 0, gY = 0, gZ = 0; // for storing Average readings

void calc_gyro_bias()
{
  // calculate the bias - by readings 1000 values and doing avg of that.
  gX = 0; gY = 0; gZ = 0;
  count = 0;
  lastRead = 0;

  Serial.println("Keep gyro STILL for calibration ......");
  delay(1000); // wait 

  // Run loop for to collect 1000 samples
  while (count < GYRO_CALIB_SAMPLES) 
  {
    // Run loop as per Sensor Hz and not Run as fast as possible.
    // Dont worry it will Collect all 1000 samples, since the condition is as per count variable, and count variable only increments at each reading
    now = micros();
    if (now - lastRead >= PERIOD_US)
    {
      lastRead = now;

      if (myISM.getRawGyro(&rawGyro))
      {
        gX += rawGyro.xData;
        gY += rawGyro.yData;
        gZ += rawGyro.zData;
        count++;
      }
    }
  }

  // retset after use.
  lastRead = 0;


  // calculated the bias(lsb) and Update the Hardcoded bias
  gyro_bias[0] = (float)gX / count;
  gyro_bias[1] = (float)gY / count;
  gyro_bias[2] = (float)gZ / count;


    // ---- SEND GYRO BIAS TO PYTHON ----
    Serial.print("GYRO_BIAS,");                             
    Serial.print(gyro_bias[0], n); Serial.print(",");     // sending n decimals to python
    Serial.print(gyro_bias[1], n); Serial.print(",");
    Serial.println(gyro_bias[2], n);


  delay(1000);

}


// =======================================================
// CALCULATE ACCEL OFFSET/BIAS USING MOTOR
// =======================================================
double ax = 0, ay = 0, az = 0; // for storing Average readings

// function to calculte the avg readings and print it.
void Read_Accel_data()
{
    // take 25 readings ka AVG - for single orientation.
    lastRead = 0;
    count = 0; // make iterator 0 before staring.
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
    
    lastRead = 0;


    // find AVG
    ax /= AVG_MEAS;
    ay /= AVG_MEAS;
    az /= AVG_MEAS;

    // Send AVG RAW LSB ONLY , and in python it will scale
    Serial.print("Accel Raw data");
    Serial.print(ax, n); Serial.print(",");   // sending n decimals 
    Serial.print(ay, n); Serial.print(",");
    Serial.println(az, n);

    }

// =======================================================
// Function which uses 2_motors to collecte the Acc data at differnt orientations
// =======================================================

void calc_acc_offset()
{ 
  // cannot do calculation of ACC offset and bias over here .
  // so we simply print the raw values and python collects these datapoint and scales it then calculate the offset and bias and stores it in offset.txt and also sends it here to update the harcoded values.
 
  Serial.println("\n--- ACC CALIBRATION START ---");
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
      
      delay(1000);



///// NOT HERE -> do in final_calib
///////////  so python collecte the raw data and scaled it , and stored it in offset.txt 
// now ask python for the updated bias and offset values. 
///////// and update the hardcoded offset and bias. and print also
// imp -> problem -> the offset that we calculate in python has 10 decimals but arudino cannot take 10 decimals i think -- check how much decimals can the aruino float store .... and as per that we send round off till 6 or 7 decimal place ka offset and bias we send here.

//   acc_bias[0] = ;
//   acc_bias[1] = ;
//   acc_bias[2] = ;
//   Serial.println("Updated offset and bias ");
//   Serial.print("bx = "); Serial.print(acc_bias[0], 6) .....;
// //   .........



  Serial.println("--- Accel CALIBRATION DONE ---\n");
  delay(1000);
}


// =======================================================
// GET INITIAL OFFSET/BIAS FROM PYTHON
// =======================================================

bool use_file_offsets = false; // for debugging

void requestOffsetsFromPython()
{
  Serial.println("READY_FOR_OFFSETS");

  unsigned long t0 = millis();
  while (!Serial.available())
  {
    if (millis() - t0 > 5000)
    {
      Serial.println("Using hardcoded values (timeout)");
      return;
    }
  }

  // ---- GYRO ----
  String line = Serial.readStringUntil('\n');
  sscanf(line.c_str(), "GYRO,%f,%f,%f",
         &gyro_bias[0], &gyro_bias[1], &gyro_bias[2]);

  // ---- ACC BIAS ----
  line = Serial.readStringUntil('\n');
  sscanf(line.c_str(), "ACC_B,%f,%f,%f",
         &b_acc[0], &b_acc[1], &b_acc[2]);

  // ---- ACC MATRIX ----
  Serial.readStringUntil('\n'); // ACC_A,
  for (int i = 0; i < 3; i++)
  {
    line = Serial.readStringUntil('\n');
    sscanf(line.c_str(), "%f,%f,%f",
           &A_acc[i][0], &A_acc[i][1], &A_acc[i][2]);
  }

  String endLine = Serial.readStringUntil('\n'); // reads "END" // for safety
  if(endLine == "END")
  {
    Serial.println("Updated all offset/bias values");
  }
  else{
    Serial.println("Error, Python dosent print END after sending values... any Error may occured....");
  }

  Serial.println("Using offset.txt values");
  use_file_offsets = true; // for degugging 
  delay(1000);
}



// =======================================================
// SETUP
// =======================================================

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
	
	// Wire.setClock(400000); // uncomment it if using 416hz or higher data rate , other wise comment it.
	

	// DataRate -> 104Hz is good (fast & low noise) // can also try 208 or more in SF since it needs frequent values.
	myISM.setAccelDataRate(ISM_XL_ODR_104Hz); // data rate (best -> 104, 208, 416) (for faster use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12Hz5, 1Hz6 )
	myISM.setGyroDataRate(ISM_GY_ODR_104Hz); // data rate (best -> 104, 208, 416) (for fast use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12 )
	DataRate_HZ = 104; // keep same as sensor_Hz
    PERIOD_US = (1000000UL / DataRate_HZ); // using this we Control the iteration time.


	// Range -> always need to set, even if scaling maunally
	myISM.setAccelFullScale(ISM_2g); 
	myISM.setGyroFullScale(ISM_125dps); 
	// test: in Motor_code if motor moves faster than 2g or 125dps then the sensor values can break - so in that senario we can do 4g or 250dps, but that can lower the precision. other jugad we can do is -> move motor slowly or etc.
	// if you wanna update the range of acc or gyro then - not just you have to change the scaling factor , but again calib (as per that_range_lsb_data * that_range_scaling_factor) and get new offset.
	

	// Filter ->
	myISM.setAccelFilterLP2(false); // can keep it ON also, // later, TRY : ON in SensorFusion and see the results. 
    // myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_20); // can keep 20, 45 (not 10) // (if you keep the filter then based on the filter -> update the hardcoded offset/bias as per that)
	myISM.setGyroFilterLP1(false); // strictly off
	

	// fifo config (not much to do here, can try stream mode in SF, see ISM/1_full_info.ino ka final setting section for more info)
	myISM.setFifoMode(ISM_BYPASS_MODE); // fifo off (default)
	
     // Set motor RPM (10 is good)
  motorAlt.setTotalSteps(STEPS_PER_REV);
  motorAlt.setRpm(10); // 6 to 24
  motorAz.setTotalSteps(STEPS_PER_REV);
  motorAz.setRpm(10); // 6 to 24
    

	delay(100);
  Serial.println("Settings applied.");

//   Serial.println(" press 'l' for Accel recalibrate // press 'j' for Gyro recalibrate.");
  delay(500);

}

// =======================================================
// LOOP
// =======================================================
void loop()
{
  if (Serial.available())
  {
    char cmd = Serial.read();
     
    // Clear the buffer - (newline, extra keys, etc) 
    // so that during the calibration is being done - No key press are recoded. 
    // (other wise while calib you pressed j j j again then after calib it will again do calib 3 more times, since its in the buffer)
    while (Serial.available()) Serial.read();

    switch (cmd)
    {
      case 'g':
      Serial.println("\n--- Offset Load Triggered ---");
      requestOffsetsFromPython(); // get latest offset/bias from the offset.txt using python.
      break;

      case 'j':
        Serial.println("\n--- Gyro Calibration Triggered ---");
        calc_gyro_bias();
        break;

      case 'l':
        Serial.println("\n--- Accel Calibration Triggered ---");
        calc_acc_offset();
        break;

      default:
        break;
    }
  }

  now = micros();
  if (now - lastRead >= PERIOD_US)
  {
    lastRead = now;
    
    if (myISM.getRawGyro(&rawGyro) && myISM.getRawAccel(&rawAccel))
    {
        // ----------------------------GYRO---------------------------------------------
        // first Calib with offset/bias_lsb then Scale it
        
        // -------- Raw_LSB -> CALIB using bias_Lsb -> Calib_Lsb --------
        // (Calib_Lsb = Raw_LSB - Bias_Lsb)
        calibGyro[0] = rawGyro.xData - gyro_bias[0];
        calibGyro[1] = rawGyro.yData - gyro_bias[1];
        calibGyro[2] = rawGyro.zData - gyro_bias[2];

        // -------- Calib_Lsb -> Scale to -> Calib_Dps --------
        calibGyro[0] *= GYRO_Scale_DPS;
        calibGyro[1] *= GYRO_Scale_DPS;
        calibGyro[2] *= GYRO_Scale_DPS;


        // -------------------------------------Accel----------------------------------------
        // first scale then calib with scaled_offset/bias.
        
        // -------- LSB -> SCALE TO g  --------
        // (we are not doing 0.061 since that's mg not g. and the sf wants g . dont do 0.000061, since that has binary representation noise)
        rawAcc[0] = (rawAccel.xData) * ACC_LSB_TO_G ;
        rawAcc[1] = (rawAccel.yData) * ACC_LSB_TO_G;
        rawAcc[2] = (rawAccel.zData) * ACC_LSB_TO_G;

        // -------- SCALED -> CALIB --------
        // -------- Raw_g -> CALIB using offset/bias_g -> Calib_g  --------
        calibAccel[0] = A_acc[0][0] * (rawAcc[0] - b_acc[0]) + A_acc[0][1] * (rawAcc[1] - b_acc[1]) + A_acc[0][2] * (rawAcc[2] - b_acc[2]);
        calibAccel[1] = A_acc[1][0] * (rawAcc[0] - b_acc[0]) + A_acc[1][1] * (rawAcc[1] - b_acc[1]) + A_acc[1][2] * (rawAcc[2] - b_acc[2]);
        calibAccel[2] = A_acc[2][0] * (rawAcc[0] - b_acc[0]) + A_acc[2][1] * (rawAcc[1] - b_acc[1]) + A_acc[2][2] * (rawAcc[2] - b_acc[2]);




        // ------------------------------------------PRINT---------------------------------------------------------
        ///// in sf - instead of prining we give it to sf. 
        Serial.print("Gyro calib :  ");
        Serial.print(calibGyro[0], 6); Serial.print(", ");
        Serial.print(calibGyro[1], 6); Serial.print(", ");
        Serial.print(calibGyro[2], 6);
        
        
        // ------- Calculate magnitude of the calibrated vector
        // at static position the magnitude should be same as g value
        double magnitude = sqrt(calibAccel[0] * calibAccel[0] + calibAccel[1] * calibAccel[1] + calibAccel[2] * calibAccel[2]);  // no need to do in sf.
        
        // Print calibrated data (X, Y, Z) and the magnitude // in sf we will not print it , but give it to the sf. 
        Serial.print("Accel calib :  ");
        Serial.print(calibAccel[0], 5); Serial.print(", ");
        Serial.print(calibAccel[1], 5); Serial.print(", ");
        Serial.print(calibAccel[2], 5);
        Serial.print(" | Magnitude: ");
        Serial.println(magnitude, 5);
    } 
    else {
        Serial.println("Failed to read Sensor data.");
    }

  }

}


