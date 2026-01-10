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




// ************************* code_1_Calib_first ************************
// Collect_data_lsb => find Bias_lsb
// Raw_lsb - bias_lsb => calib_lsb
// calib_lsb * scaling_factor => calib_dps

#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM;
sfe_ism_raw_data_t rawGyro;


// IMP - so that loop run as per the data rate of sensor and not try to run as fast as possible 
unsigned int DataRate_HZ;	
unsigned long PERIOD_US;          
// time tracker for loops -> mainLoop(), while() , for()     
unsigned long lastRead = 0; 
unsigned long now = 0;


// #######################   CONFIG   ####################### 
#define GYRO_Scale_DPS   0.004375f      // scaling to 125 dps
#define GYRO_CALIB_SAMPLES  1000         // number of samples to collect for bias calculation
double gyro_bias[3] = { 44.295409, -169.449102, 72.213573}; // // Hardcode Gyro bias_lsb, not accurate
// ##########################################################

double calibGyro[3] = {0.0,0.0,0.0} ;
long gX = 0, gY = 0, gZ = 0;
long count = 0;

// =======================================================
// CALCULATE GYRO BIAS 
// =======================================================
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


  // calculated the bias(lsb)
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
  
    // DataRate -> 
    myISM.setGyroDataRate(ISM_GY_ODR_104Hz); // data rate (best -> 104, 208, 416) (for fast use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12 )
    DataRate_HZ = 104; // keep same as sensor_Hz
    PERIOD_US = (1000000UL / DataRate_HZ);
	
	// Range -> always need to set, even if scaling maunally
	myISM.setGyroFullScale(ISM_125dps); 

	// Filter ->
myISM.setGyroFilterLP1(false); // strictly off
	
	// fifo config (not much to do here, can try stream mode in SF, see 1_full_info.ino ka final setting section for more info)
	myISM.setFifoMode(ISM_BYPASS_MODE); // fifo off (default)
	
	delay(100);
  Serial.println("Settings applied.");


  ///// NOT HERE -> do in final_calib :
  //////////// Get latest offset/ bias from py from offset.txt
  // if not available then only we go with the hardcoded once and it will print - if we use from py wala or hardcoded wala.


  Serial.println("Using hardcoded gyro bias.");
  Serial.println("Send 'j' over Serial to recalibrate gyro.");
  delay(500);

}

// =======================================================
// LOOP
// =======================================================
void loop()
{
  // -------- SERIAL COMMAND CHECK for "j" --------
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

      default:
        break;
    }
  }

  now = micros();
  if (now - lastRead >= PERIOD_US)
  {
    lastRead = now;
    
    if (myISM.getRawGyro(&rawGyro))
    {
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


        ///// in sf - instead of prining we give it to sf. 
        Serial.print(calibGyro[0], 6); Serial.print(", ");
        Serial.print(calibGyro[1], 6); Serial.print(", ");
        Serial.println(calibGyro[2], 6);
    }
  }
}













































// ************************* code_2_Scale_first ************************
// collect_data_lsb => find bias_Lsb => scale_bias_dps
// Raw_lsb * Scaling_factor => Raw_dps
// Raw_dps - bias_dps => calib_dps


#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM;
sfe_ism_raw_data_t rawGyro;


// IMP - so that loop run as per the data rate of sensor and not try to run as fast as possible 
unsigned int DataRate_HZ;	
unsigned long PERIOD_US;          
// time tracker for loops -> mainLoop(), while() , for()     
unsigned long lastRead = 0; 
unsigned long now = 0;


// #######################   CONFIG   ####################### 
#define GYRO_Scale_DPS   0.004375f      // scaling to 125 dps
#define GYRO_CALIB_SAMPLES  1000         // number of samples to collect for bias calculation
double gyro_bias[3] = {0.193792 , -0.741340, 0.315934 };  // Hardcode Gyro bias_dps, not accurate
// ##########################################################

double calibGyro[3] = {0.0,0.0,0.0} ;
long gX = 0, gY = 0, gZ = 0;
long count = 0;

// =======================================================
// CALCULATE GYRO BIAS 
// =======================================================
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
        gX += rawGyro.xData ; 
        gY += rawGyro.yData ;
        gZ += rawGyro.zData ;
        count++;
      }
    }
  }

  // retset after use
  lastRead = 0;
  
  
  // calculated the bias_Lsb 
  // then scale it to get bias_dps.
  gyro_bias[0] = ((float)gX / count) * GYRO_Scale_DPS;
  gyro_bias[1] = ((float)gY / count) * GYRO_Scale_DPS;
  gyro_bias[2] = ((float)gZ / count) * GYRO_Scale_DPS;


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
  
    // DataRate -> 
    myISM.setGyroDataRate(ISM_GY_ODR_104Hz); // data rate (best -> 104, 208, 416) (for fast use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12 )
    DataRate_HZ = 104; // keep same as sensor_Hz
      PERIOD_US = (1000000UL / DataRate_HZ);
	
	// Range -> always need to set, even if scaling maunally
	myISM.setGyroFullScale(ISM_125dps); 

	// Filter ->
  myISM.setGyroFilterLP1(false); // strictly off
	
	// fifo config (not much to do here, can try stream mode in SF, see 1_full_info.ino ka final setting section for more info)
	myISM.setFifoMode(ISM_BYPASS_MODE); // fifo off (default)
	
	delay(100);
  Serial.println("Settings applied.");


  ///// NOT HERE -> do in final_calib :
  //////////// Get latest offset/ bias from py from offset.txt
  // if not available then only we go with the hardcoded once and it will print - if we use from py wala or hardcoded wala.


  Serial.println("Using hardcoded gyro bias.");
  Serial.println("Send 'j' over Serial to recalibrate gyro.");
  delay(500);

}

// =======================================================
// LOOP
// =======================================================
void loop()
{
  // -------- SERIAL COMMAND CHECK for "j" --------
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

      default:
        break;
    }
  }

  now = micros();
  if (now - lastRead >= PERIOD_US)
  {
    lastRead = now;
    
    if (myISM.getRawGyro(&rawGyro))
    {
      // first scale then calib with scaled_offset/bias.

        // -------- Raw_LSB -> SCALE TO -> Raw_dps  --------
        calibGyro[0] = (float)rawGyro.xData * GYRO_Scale_DPS;
        calibGyro[1] = (float)rawGyro.yData * GYRO_Scale_DPS;
        calibGyro[2] = (float)rawGyro.zData * GYRO_Scale_DPS;
        
        // -------- Raw_dps -> CALIB using bias_dps -> Calib_dps  --------
        // ( Calib_Dps = Raw_Dps - bias_Dps)
        calibGyro[0] -= gyro_bias[0];
        calibGyro[1] -= gyro_bias[1];
        calibGyro[2] -= gyro_bias[2];
      

        ///// in sf - instead of prining we give it to sf. 
        Serial.print(calibGyro[0], 6); Serial.print(", ");
        Serial.print(calibGyro[1], 6); Serial.print(", ");
        Serial.println(calibGyro[2], 6);
    }
  }
}

