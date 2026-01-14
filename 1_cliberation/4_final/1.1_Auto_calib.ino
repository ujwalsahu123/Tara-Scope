
// Read 1.2_Auto_calib.py file for more info... 
// to know more Info about the Accel_Auto_calib , Gyro_Auto_calib , Mag_Auto_calib code -> refer there each code files ... 

// ###############################################################################################



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

// sending n decimal data to python   // UNO float can only handle till 6 decimals (more than this has representation error) 
int n_gyro_bias = 6; // do 6 in uno, 8 in esp
int n_acc_raw = 4; // do 2-4 in uno , 4-6 in esp  (32343.123...)
int n_mag_raw ; // later set...
// for accel , mag ka offset/bias ka decimal -> go in the python and update from there -> because python hee calc offset/bias and then it stores in offset.txt


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
// 1 time Calib when in new Location (and then use that latest values using "g")
double A_acc[3][3] = 
{{0.97522389, 0.00009342, -0.00058922},
 {0.00009342, 0.98066595, 0.00053047},
 {-0.00058922, 0.00053047, 0.98232234}};
double b_acc[3] = { 0.00500114, -0.01523379, 0.01161619}; 
// ################################################################



double rawAcc[3] =  {0.0, 0.0, 0.0};
double calibAccel[3] = {0.0, 0.0,0.0};
double calibGyro[3] = {0.0,0.0,0.0} ;
long count = 0; // iterator for Average readings



// =======================================================
// CALCULATE GYRO BIAS 
// =======================================================
long gX = 0, gY = 0, gZ = 0; // for storing Average readings

void calc_gyro_bias()
{

  delay(100); // # 100 milliseconds dealy IMP, keep 100 only.
  // Nothing printing here for handshake,since no need - we gonna directly send the data to python.

  // calculate the bias - by readings 1000 values and doing avg of that.
  gX = 0; gY = 0; gZ = 0;
  count = 0;
  lastRead = 0;

  Serial.println("Keep Sensor STILL for Gyro calibration ......");
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


  // calculated the bias(lsb) and Update the Hardcoded gyro_bias[]
  gyro_bias[0] = (float)gX / count;
  gyro_bias[1] = (float)gY / count;
  gyro_bias[2] = (float)gZ / count;


    // ---- SEND GYRO BIAS TO PYTHON ----
    Serial.print("GYRO_BIAS,");                             
    Serial.print(gyro_bias[0], n_gyro_bias); Serial.print(",");     // sending n_gyro_bias decimals to python
    Serial.print(gyro_bias[1], n_gyro_bias); Serial.print(",");
    Serial.println(gyro_bias[2], n_gyro_bias);


  delay(500); // not necessary but good.

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
    Serial.print(ax, n_acc_raw); Serial.print(",");   // sending n decimals 
    Serial.print(ay, n_acc_raw); Serial.print(",");
    Serial.println(az, n_acc_raw);

}

// =======================================================
// Function which uses 2_motors to collecte the Acc data at differnt orientations
// =======================================================
void calc_acc_offset()
{ 
  // cannot do calculation of ACC offset and bias over here .
  // so we simply print the raw values and python collects these datapoint and scales it then calculate the offset and bias and stores it in offset.txt and also sends it here to update the harcoded values.
 
  delay(100); // # 100 milliseconds dealy IMP, keep 100 only.
  Serial.println("ACC_RAW_DATA_SHARING_START"); // for handshake - to tell python that collect the data
  
  delay(1000); // for stabilization // keep

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
      delay(1000); // keep
      motorAlt.move(false, (STEPS_PER_REV/4)); // move the alt motor back to -90 degree (back to inital orientation)
      // So now we are OG back to OG inital orientation.
      
      delay(100); // keep
      Serial.println("ACC_RAW_DATA_SHARING_DONE"); // finished sending raw accel data to Python
  


      // --------------- get new Accel offset/bias from python and update the  hardcoded once --------------------------

      // wait for python to process the data and send the new offset/bias values. if not received in 5 sec then timeout and exit the function.
      unsigned long t0 = millis();
      while (true)
      {
        if (Serial.available())
          break;

        if (millis() - t0 > 5000)   // 5 sec timeout
        {
          Serial.println("TIMEOUT");
          return;
        }
      }
      
      // ---- HANDSHAKE ----
      while(true)         // wait for python to send the msg that it is sending updated offset/bias values.
      { 
        if (Serial.available() > 0)
        {

        // read single line from python
        String msg = Serial.readStringUntil('\n');
        msg.trim();

        if (msg.length() == 0) continue;   // 👈 ignore blank lines
        
        if(msg == "REQUEST_FOR_SENDING_UPDATED_VALUES")
        { 
          Serial.println("READY_FOR_UPDATED_VALUES"); 
          break; // exit the while loop
        }
        else if(msg == "ACCEL_CALIB_FAILED")
        {
          Serial.println("ERROR_GETTING_UPDATED_VALUES"); // something went wrong
          return; // exit the function - without updating the hardcoded values.
        }
        else{ // when unexpected msg received
          Serial.println(msg); // print the msg for debugging
          return; // exit the function - without updating the hardcoded values.
        }

        }
      }


      bool gotAccA = false;
      bool gotAccB = false;
      int accARow = 0;

      while (true)
      {
        if (!Serial.available())
          continue;   // wait for data to come

        String line = Serial.readStringUntil('\n');
        line.trim();

        // ---- IGNORE BLANK / GARBAGE ----
        if (line.length() == 0)
          continue;

        // -------- ACC_B --------
        if (line.startsWith("ACC_B,"))
        {
          int p1 = line.indexOf(',');
          int p2 = line.indexOf(',', p1 + 1);
          int p3 = line.indexOf(',', p2 + 1);

          if (p1 < 0 || p2 < 0 || p3 < 0)
          {
            Serial.println("Error");
            return;
          }

          b_acc[0] = line.substring(p1 + 1, p2).toFloat();
          b_acc[1] = line.substring(p2 + 1, p3).toFloat();
          b_acc[2] = line.substring(p3 + 1).toFloat();

          gotAccB = true;
        }

        // -------- ACC_A header --------
        else if (line == "ACC_A")
        {
          accARow = 0;
        }

        // -------- ACC_A rows --------
        else if (accARow < 3 && gotAccB)
        {
          int p1 = line.indexOf(',');
          int p2 = line.indexOf(',', p1 + 1);

          if (p1 < 0 || p2 < 0)
          {
            Serial.println("Error");
            return;
          }      

          A_acc[accARow][0] = line.substring(0, p1).toFloat();
          A_acc[accARow][1] = line.substring(p1 + 1, p2).toFloat();
          A_acc[accARow][2] = line.substring(p2 + 1).toFloat();

          accARow++;
          if (accARow == 3)
            gotAccA = true;
        }

        // -------- END --------
        else if (line == "END")
        {
          if (gotAccA && gotAccB)
          {            
            Serial.println("ACC_UPDATED");  // for handshake -> that we got the offset/bias from python and updated the hardcoded values.
          }
          else
          {
            Serial.println("Error"); // END printed but the ACC_A and ACC_B not received properly
          }
          return; // exit after read End 
        }

        // -------- UNKNOWN --------
        else
        {
          Serial.println(line); // print the UNKNOWN LINE for debugging
        }

      }// end while true

    delay(500); // keep
}


// =======================================================
// GET INITIAL OFFSET/BIAS FROM PYTHON
// ======================================================= 
void requestOffsetsFromPython()
{
  delay(100); // # 100 milliseconds dealy IMP, keep 100 only.
  
      // wait for python to process the data and send the new offset/bias values. if not received in 5 sec then timeout and exit the function.
      unsigned long t0 = millis();
      while (true)
      {
        if (Serial.available())
        break;

        if (millis() - t0 > 5000)   // 5 sec timeout
        {
          Serial.println("TIMEOUT");
          return;
        }
      }


    // ---- HANDSHAKE ----
    while(true)         // wait for python to send the msg that it is sending updated offset/bias values.
    { 
      if (Serial.available() > 0)
      {
        
      // read single line from python
      String msg = Serial.readStringUntil('\n');
      msg.trim();

      if (msg.length() == 0) continue;   // 👈 ignore blank lines
        
      if(msg == "REQUEST_FOR_SENDING_UPDATED_VALUES")
      { 
        Serial.println("READY_FOR_UPDATED_VALUES"); 
        break; // exit the while loop
      }
      else if(msg == "FAILED_GETTING_OFFSETS")
      {
        Serial.println("ERROR_GETTING_UPDATED_VALUES"); // something went wrong
        return; 
      }
      else{ // when unexpected msg received
        Serial.println(msg); // print the msg for debugging
        return; 
      }

      }
    }

    // --------------- get new offset/bias from python and update the hardcoded once --------------------------
    bool gotGyro = false;
    bool gotAccB = false;
    bool gotAccA = false;
    int accARow = 0;
  
    while (true)
    {
      if (!Serial.available())
        continue; // wait for data to come

      String line = Serial.readStringUntil('\n');
      line.trim();

      if (line.length() == 0)
        continue; // ignore blank lines

      // -------- GYRO --------
      if (line.startsWith("GYRO,"))
      {
        int p1 = line.indexOf(',');
        int p2 = line.indexOf(',', p1 + 1);
        int p3 = line.indexOf(',', p2 + 1);

        if (p1 < 0 || p2 < 0 || p3 < 0)
        {
          Serial.println("Error");
          return;
        }

        gyro_bias[0] = line.substring(p1 + 1, p2).toFloat();
        gyro_bias[1] = line.substring(p2 + 1, p3).toFloat();
        gyro_bias[2] = line.substring(p3 + 1).toFloat();

        gotGyro = true;
      }

      // -------- ACC_B --------
      else if (line.startsWith("ACC_B,"))
      {
        int p1 = line.indexOf(',');
        int p2 = line.indexOf(',', p1 + 1);
        int p3 = line.indexOf(',', p2 + 1);

        if (p1 < 0 || p2 < 0 || p3 < 0)
        {
          Serial.println("Error");
          return;
        }

        b_acc[0] = line.substring(p1 + 1, p2).toFloat();
        b_acc[1] = line.substring(p2 + 1, p3).toFloat();
        b_acc[2] = line.substring(p3 + 1).toFloat();

        gotAccB = true;
      }

      // -------- ACC_A HEADER --------
      else if (line == "ACC_A")
      {
        accARow = 0;
      }

      // -------- ACC_A ROWS --------
      else if (accARow < 3 && gotAccB)
      {
        int p1 = line.indexOf(',');
        int p2 = line.indexOf(',', p1 + 1);

        if (p1 < 0 || p2 < 0)
        {
          Serial.println("Error");
          return;
        }

        A_acc[accARow][0] = line.substring(0, p1).toFloat();
        A_acc[accARow][1] = line.substring(p1 + 1, p2).toFloat();
        A_acc[accARow][2] = line.substring(p2 + 1).toFloat();

        accARow++;
        if (accARow == 3)
          gotAccA = true;
      }

      // -------- END -------
      else if (line == "END")
      {
        if (gotGyro && gotAccB && gotAccA)
          Serial.println("OFFSETS_UPDATED"); // for handshake -> that we got the offset/bias from python and updated the hardcoded values.
        else
          Serial.println("Error"); // if END printed but all values not received properly

        return;
      }

      // -------- UNKNOWN --------
      else
      { // when unexpected msg received
        Serial.println(line); // print the UNKNOWN LINE for debugging
        return;
      }

    } // end while true

    delay(500); // keep
}



// =======================================================
// PRINT CALIBRATED DATA
// =======================================================
void Calib_Output_Print()
{
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
        Serial.print("     Accel calib :  ");
        Serial.print(calibAccel[0], 5); Serial.print(", ");
        Serial.print(calibAccel[1], 5); Serial.print(", ");
        Serial.print(calibAccel[2], 5);
        Serial.print(" | Magnitude: ");
        Serial.println(magnitude, 5);
       } 
      else 
       {
        Serial.println("Failed to read Sensor data.");
       }
}

void Offset_bias_values_Print()
{
  delay(100);// imp
  Serial.print("GYRO: ");
  Serial.print(gyro_bias[0],6);Serial.print(", ");
  Serial.print(gyro_bias[1],6);Serial.print(", ");
  Serial.println(gyro_bias[2],6);
  Serial.print("B_ACC: ");
  Serial.print(b_acc[0],6 );Serial.print(", ");
  Serial.print(b_acc[1],6 );Serial.print(", ");
  Serial.println(b_acc[2],6 );  
  Serial.print("A_ACC: ");
  Serial.print(A_acc[0][0],6 );Serial.print(", ");
  Serial.print(A_acc[0][1],6 );Serial.print(", ");
  Serial.print(A_acc[0][2],6 );Serial.print(", ");
  Serial.print(A_acc[1][0],6 );Serial.print(", ");
  Serial.print(A_acc[1][1],6 );Serial.print(", ");
  Serial.print(A_acc[1][2],6 );Serial.print(", ");
  Serial.print(A_acc[2][0],6 );Serial.print(", ");
  Serial.print(A_acc[2][1],6 );Serial.print(", ");
  Serial.println(A_acc[2][2],6 );
  Serial.println("DONE");
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
bool streamEnabled = true;

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
      // Serial.println("\n--- Offset Load Triggered ---");
      streamEnabled = false;
      requestOffsetsFromPython(); // get latest offset/bias from the offset.txt using python.
      streamEnabled = true;
      break;

      case 'j':
        // Serial.println("\n--- Gyro Calibration Triggered ---");
        streamEnabled = false;
        calc_gyro_bias();
        streamEnabled = true;
        break;

      case 'l':
        // Serial.println("\n--- Accel Calibration Triggered ---");
        streamEnabled = false;
        calc_acc_offset();
        streamEnabled = true;
        break;

      case 'p':
        // print all offset/bais values , good for debugging and see if the offset.txt values or calib values actually updated or not.
        streamEnabled = false;
        Offset_bias_values_Print();
        streamEnabled = true;
        break;
      
      case 'y':
        // to display 10 calib_output values in terminal. (no need , just for degubbing)
        streamEnabled = false;
        delay(100);
        for(int i = 0 ; i < 10 ; i++) { Calib_Output_Print(); }
        Serial.println("DONE");
        streamEnabled = true;
        break;
         
      default:
        break;
    }
  }

  // IN the LOOP - Always Calc the Calib output and send to SF (when no cmd is being processed / streamEnabled = true)
  now = micros();
  if (now - lastRead >= PERIOD_US)
  {
    lastRead = now;

    if(streamEnabled)
    {
      // Calib_Output_Print();           // in sf -> we will not print the calib values , but do SF ... 
    }

  }

}


