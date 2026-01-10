


ok now what i wanna do is -> i wanna write a py code file -> from which we send commands 
"j" gyro calib , "l" accel calib , "h" for calculating local_g , magnetic filed. 
ok so in the start setup() function the arduino asks python for gyro_bias value , accel_offset and bias value , and then python gives it those values from the offset.txt where it is stored. 
and if python is able to send those values then arudino and arduino catches it and updates the hardcoded values - and also prints "using offset.txt values" . and if python is not able to get the data from offset.txt then it sends - couldnt found - and then arduino checks this and then prints "using hardcoded values" and dosent updates the hardcoded values.
if stores all the offset/bias in a single txt file is hard then we can make seperate txt files for each accel , gyro..


ok so when i press j then its does gyro calib and then form the gyro_calib function it sends the bias vector to the python and then python updates it in a offsets.txt file .

when i press h then it ask the user for lati , longi , height . then uses the formula and api to get the local_g and magnetic field and then stores that calculated values in offsets.txt  ### give it code file

when i press l then the accel calib happens - and then arduino prints values , and python reads those values and scales it and uses the local_g from the offsets.txt and then uses the function to calc the bias and offset using the collected value and local_g . and then updates the offset/bias in offset.txt and also sends it to arudino and then arudino catches it updates the hardcoded values..












// GYRO INFO ----------------------------------------------------------------------------------------------

// Gyro calib is required after every startup. since the bias is dependent on temp, etc 

// so we write a calc_gyro_bias() function which collects data points and find bias - and then we can use that bias to get calib_output. 
// so we can either directly call this calc_gyro_bias() function in the setup() so as it startups - it calculates. 
// or we can call it when the user gives cmd. 
// both are fine - but using user cmd is better - so that user when ever wants can calib .... 

// process: 
// so the code prints the calib values contineously . 
// & since calib is as per user cmd - so jabtak user calib naa kar tab tak - bekar unclib values print nahi hone chaheay ->
// so initally we use the hardcoded bias values (not accurate but okayish) for getting the calib_value. 
// and when we press "j" then it calls the calc_gyro_bias() function - and updates that hardcoded with the new one ...and print the new one...
// in calc_gyro_bias() function it first Reads raw values for 1000 readings 
// and then Calculates the bias(lsb) that biass and updates that hardcodecd bias array
// then in main loop we do raw_lsb => calib using bias_lsb (hardcoded/updated) => scale to dps
 
// Code Process : 
// call the calc_gyro() function when cmd "j" is pressed
// in that function read data - calculate the bias - and update the hardcoded bias.
// then in main loop simply do calib and print.  raw->calib->scale.

// We can do any  // raw->scale->calib // or // raw->calib->scale // both are same but caliberating first (-bias_lsb) is better since less binary_representation noise.
// if you want scale first -> the code is also there down below. *******code_2_scale_first******
// but use code_1. its Better.


// --------------
// at rest the values should be 0 for all.
// 1000 samples read (dosent matter how many hz you keep)
// keep the sensor STILL at calib time.

// ------Result--------
// after code run ->  calib is done fine- so the vales are closer to 0. 
// but since the gyro is very noisy thats why values seems to see shaky - but they are very small noise 
// ex:- 
// x,y,z
// 0.006952, 0.054626, 0.047337
// -0.023673, 0.032751, 0.012337
// so values are closer to 0, but super noisy, but the noise is very small +-0.0x , so max 0.1 to 0.02 degree error at Rest position.


// i tested this works and it uses hardcoded bias intially and updated bias later after calibration.


// As we know that bias drift as per time, so if we calculate the bias at differnt time -> startup time , 20 seconds after the startup then we will get differnt bias. 
// so if we calc the bias after some time of startup -> thus we get drifted bias ...
// but sensor fusion takes care of the bias drift (using mag, acc) , and we do gyro bias calculation at the startup or later is fine .
// and a stable calibration at whatever time is fine, (but you will think that latest calib will give latest bias and it will be more accurate ... 
// so yes, ✅ Calibrating slightly later (after warm-up) usually gives slightly better / more stable bias (after heat up, it stays at stable heat)
// and we know that Bias drift never fully stops -> so we do NOT need to recalibrate gyro bias frequently., since sf takes cares of drift.
// so Thus -> 1-stable calib of bias after 1 miniute of statup is enough.. and after that sf will take care of drift. 
// & for more precise you can do after every 15 min... but not sure if it imporves.. or its unecessay (very small improvement)
// ex:-
// Startup bias:
// bx = 58.77
// by = -184.35
// bz = 62.86
// After 1 min:
// bx = 56.72   (-2.05)
// by = -183.29 (+1.06)
// bz = 63.93   (+1.07)


// so do calib 30 sec after the startup , just calib for 1000 values not more. , do calib every 10 min...(later figure out time interval)
// use code_1 its better.

// Collect_data_lsb => find Bias_lsb
// Raw_lsb - bias_lsb => calib_lsb
// calib_lsb * scaling_factor => calib_dps





// Accel INFO ----------------------------------------------------------------------------------------------

// we can do Accel Auto Calculate the offset/bias using data points & local_g (instead of using magento) 
// so that no need to Run 3 differnt files and use magneto then store offset/bias here and there. 
// We just simply press a Cmd "L" and then it uses the 2 motors to move the sensor and collect the data at different orientaitons
// and then send that data to python where it uses the collected data and local_g to calculate the offset/bias and it also stores it in offset.txt file
// and then send that offset/bias to arduino , so that the arduino gives calibrated values as per the new offset/bias we calculated.
// put the device in sleeping position before calib (so that sensors can move 3 axis rotation)
// then stand up the device and do gyro calib , mag calib - and then dont touch the device.

// NOTES:
// -> at differnt gravity /place its better to calcualte new data and then use that data with the local_g of that place.
// (No need to do Accel Calib at every startup but if the device is in at new location than previously acc_calib location ) 
// Also later think on this -> Calib karna too accha hai , but need to think ki improvement hoga issay ya nahi (need to check this) since noise is way bigger than these small accuracy jugad we do -(calib as per local g.)
// Also later think -> if the main thing we are doing is calib as per the local_g then can we use the previous data collected and just calcuate the offset/bias as per that previously_collected data and the current location ka local_g.
// we are not doing calc offset/bias here since its very heavy computation and it needs numpy library.


// PROCESS : 
// apply caliberation on live data - and see the xyz and Magnitude . 
// by seeing the magnitude you can judge if the offset are good or bad as per (magnitude close to 1).
// so there is hardcoded offset prewritten and it also ask for the latest offsets and bias from python form offset.txt
// also auto calib is there when presses "l"
// here we use motor to print the data.
// it prints - average of 25 values (we are doing averaging here only in arduino, and not in py)
// and then in py - it just reads and scales the value and then calculates the offset and bias over there and then store it in offset.txt and also send it to arduino 
// and arduino updates the offset and bias and give calib_values as per that new only.


// # For Accel dont do Raw->calib->Scale , do Raw->scale-> calib. (since when you do Raw-> calib-> scale then precision is lost since the lsb values are super big, so offset are like 0.000061)












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


  ///// NOT HERE -> do in final_calib
  ///////////////// send data to python for storing the new one in offset.txt


  Serial.println("Updated gyro bias (LSB):");
  Serial.print("bx = "); Serial.println(gyro_bias[0]);
  Serial.print("by = "); Serial.println(gyro_bias[1]);
  Serial.print("bz = "); Serial.println(gyro_bias[2]);
  Serial.println(count);
  Serial.println("Gyro Calibration complete\n");
  delay(1000);

}


// --------------------------------------------------
// CALCULATE ACCEL OFFSET/BIAS USING MOTOR
// --------------------------------------------------
long ax = 0, ay = 0, az = 0; // for storing Average readings

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
    Serial.print(ax);
    Serial.print(",");
    Serial.print(ay);
    Serial.print(",");
    Serial.println(az);

    }

// --------------------------------------------
// Function which uses 2_motors to collecte the Acc data at differnt orientations
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


  ///// NOT HERE -> do in final_calib :
  //////////// Get latest gyro bias using py from -> offset.txt
  // if not available then only we go with the hardcoded once (and it will print - if we use from py wala or hardcoded wala.)
  
  

  ///// NOT HERE -> do in final_calib
  //////////// Get latest Accel offset/ biasusing py from -> offset.txt
  // if not available then only we go with the hardcoded once (and it will print - if we use from py wala or hardcoded wala.)

  Serial.println("Using hardcoded Accel, Gyro offset/bias.");
  Serial.println(" press 'l' for Accel recalibrate // press 'j' for Gyro recalibrate.");
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


