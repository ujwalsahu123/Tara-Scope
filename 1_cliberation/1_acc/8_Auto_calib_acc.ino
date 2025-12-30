// we can do auto_calc the offset / bias using data points that we have -> as per the currect location ka gravity. 
// same as magneto , but for more precision we use current location ka g and not 1g . 
// so bar bar location ka g nikalna and manually give to magnetor and offset copy paste say acccha
// we can use the location api and noaa api for height . and use the g_calculator api and give it locaiton and height to get our G . 
// and then in the calc_acc function we can use our stored_data (hardcode - our collected datapoints)

//  i think i need to collect new data also yaa puranay wale say kam chal jaygaa ? (since old data was collected where g was differnt...)
// also check Magnetor offset/ bias on my manually collected data as per -> 1g vs 0.97...(current location ka g)  both kay offset compare - kitna fark hai.
// there is minute fark but local g is better... for sure. i saw the internet.
// and we can collect the data also (not a big deal) - sleeping position may rakhna hoga the device (since we get orientations) 
// and then when standed up we do gyro calib and mag calib. and then no touching the device.

// see -> acc_collect_data, get local g, acc_calculate_offset <- is better to do (Not at every startup but if the device is in at new location than previously acc_calib location ) 
// too karna too accha hai , but need to think ki improvement hoga issay ya nahi (need to check this) since noise is way bigger than these small accuracy jugad we do. ********************


// Think karna hai yaa nahi ???? (does this increase nay accuracy ?  if we calc offset/bias as per current locaiton ka G and not 1g.)

// so think ki karha hai ya nahi, 

// if karna hai => then write 7_calc_acc.ino , then think ki -> old data with new g value or get new data with new g ?? (better dosent matter - how much accurary imporves that matters  - if very little bit then no need.)
// so based on thinking write collect_data or not (same code as motor_code_acc - but sleeping may hoga and fully automated moving and reading) - 
// get the g from api - using python code (the ard will request the python and python will fetch api Not NOAA wala -its bad, and get the local g) - and if not able to get the api - then it asks in terminal to us . and after given it passes it to arduino. 
// and ard uses the data and local in ard_calc() function to calculate the offset (not bias since factor bias is fine and its not dependent on local g) . then store the offset in a var  (in real device store the readins and offset, etc constanst in the Ram data - since if device on-off hua then we dont want all the data to be deleted. - can think more on what gets stored and what not and architecture, etc..)
// then run the Calib_acc fun which does raw->scale->calib

// after making this function update the final_Auto_calib.ino and 3_motor_code and py which sends cmd to 3_motor_code


// write function -> calc_acc() which collectes data pionts at sleep position - full automatically. 
// # and then scales those values and get the local_g from user input (later python api)
// #  and then does using the scaled_data_points collected and local_g -> finds the offset and bias and stores it in a variable.



// use api for getting local_g . and for that we need height which we can get from api or (barometer later) and we also want latitude logitude - which we will get from api

// So in this code only put calc_acc() wala code . and using python we will get local_g and then give it to the calc_acc() function which will - 1st give instructions that sleeping may rakho at rest.... and then it will collect data and use that data with local_g to calculte the offset/bias using the Algo.... and then store it and then use that when calib ......  
////// process ->  here write the calc_acc() function which takes local_g from python - collects data & scales (motor-automated) - runs algo to find the offset/bias and stores it - in main fucntion raw->scale->calib->print
// and then use this code and update the final_calib/Auto_calib.ino   , and then that Auto_calib will be used in 3_full_motor_code -> and that 3_full_motor_code will be used by a py file from which we will send command -> "calib"  and also send local_g , magnetig_field (using api) to arudino -> so that it can caclulate and store it and use it for raw->scale->calib->sf->RPY

// after writing this code file -> update the final_calib/ACC_GYRO_calib.ino file and all other files of that folder.



// cmd "l" press hoga then only it calibs accel. and updates the hardcoded values and then prints the new values
// hardcoded may kya rakhna hai ??? konsa offset ??








// process : 
// apply caliberation on live data - and see the xyz and Magnitude . 
// by seeing the magnitude you can judge if the offset are good or bad as per (magnitude close to 1).
// so there is hardcoded offset prewritten and it also ask for the latest offsets and bias from python form offset.txt
// also auto calib is there when presses "l"
// here we use motor to print the data.
// it prints - average of 25 values (we are doing averaging here only in arduino, and not in py)
// and then in py - it just reads and scales the value and then calculates the offset and bias over there and then store it in offset.txt and also send it to arduino 
// and arduino updates the offset and bias and give calib_values as per that new only.





// # dont do Raw->calib->Scale , do Raw->scale-> calib. (since when you do Raw-> calib-> scale then precision is lost since the lsb values are super big, so offset are like 0.000061)



#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"
#include <CheapStepper.h>

SparkFun_ISM330DHCX myISM;
CheapStepper motorA(4, 5, 6, 7);
CheapStepper motorB(8, 9, 10, 11);

sfe_ism_raw_data_t rawAccel;

unsigned int DataRate_HZ;
unsigned long PERIOD_US;               
unsigned long lastRead = 0;
unsigned long now = 0;

// #######################   CONFIG   ####################### 
#define ACC_LSB_TO_G 0.00006103515625f   // 2g range
#define AVG_MEAS    25
#define DEG_to_move    5 ////////////// Each 5-deg par readings laygaa (update if you want)
const int Moves = 380 / deg_to_move ;   
#define STEPS_PER_REV 4096
// Hardcoded offset and bias
double acc_bias[3] = {  0.005380, -0.014230, 0.009798 }; 
double A[3][3] = 
{   {0.996986, 0.000024, -0.000549},
    {0.000024, 1.001778, 0.000595},
    {-0.000549, 0.000595, 1.003680}     };
// ##########################################################



// --------------------------------------------------
// CALCULATE ACCEL BIAS USING MOTOR
// --------------------------------------------------
void calc_acc_bias()
{
  long sumX = 0, sumY = 0, sumZ = 0;
  int samples = 0;

  Serial.println("\n--- ACC CALIBRATION START ---");
  delay(1000);
  
  // MOVEMENT 1
  for (int i = 0; i < Moves; i++)  // take reading for full circle - as per Moves and Degrees_to_move
  { 
    delay(2000);
     
    long ax = 0, ay = 0, az = 0;
    
    // take 25 readings ka AVG - for single orientation.
    int count = 0;
    lastRead = 0;
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
    lastRead = 0;


    // find AVG
    ax /= AVG_MEAS;
    ay /= AVG_MEAS;
    az /= AVG_MEAS;

    // Send RAW LSB ONLY
    Serial.print(ax);
    Serial.print(",");
    Serial.print(ay);
    Serial.print(",");
    Serial.println(az);

    // cannot do calculation of ACC offset and bias over here .
    // so we simply print the raw values and python collects these datapoint and scales it then calculate the offset and bias and stores it in offset.txt
    // and then the arduino asks for the updated offset and bais and python gives it . and then arduino updates the offset and bias values here and gives calib_data as per the updated once.


    samples++; // so that later we can print the number of samples we collected. or other use... // if not in use then remove it.
    
    delay(100); 
    motorA.moveDegrees(true, DEG_to_move); // move to next orientation or current movement.
    delay(100);
  }
  motorA.moveDegrees(false, 380); // return to inital position



///////////  NOW
/// Movement 2 , 3 later implement.
////// implement 2nd roataion -> az motor (and dont return to initial posi )
////// then 3rd rotation -> move alt motor by 90 deg (or 4096/4) and then az motor moves movement but this in opposite direction



///// NOT HERE -> do in final_calib
/////////// ask python for the updated bias and offset values. 

///// NOT HERE -> do in final_calib
///////// update the hardcoded offset and bias. and print
//   acc_bias[0] = ;
//   acc_bias[1] = ;
//   acc_bias[2] = ;

//   Serial.println("Updated offset and bias ");
//   Serial.print("bx = "); Serial.print(acc_bias[0], 6) .....;
// //   .........
// Serial.print(samples);


  Serial.println("--- CALIBRATION DONE ---\n");
  delay(500);
}

// --------------------------------------------------
// SETUP
// --------------------------------------------------
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
	myISM.setGyroFullScale(ISM_125dps); 

	// DataRate -> 
	myISM.setAccelDataRate(ISM_XL_ODR_104Hz); // data rate (best -> 104, 208, 416) (for faster use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12Hz5, 1Hz6 )
	myISM.setGyroDataRate(ISM_GY_ODR_104Hz); // data rate (best -> 104, 208, 416) (for fast use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12 )
	DataRate_HZ = 104; // keep same as sensor_Hz
    PERIOD_US = (1000000UL / DataRate_HZ);

	// Filter ->
	myISM.setAccelFilterLP2(false); 
  // myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_45); // can keep 20, 45. 
	myISM.setGyroFilterLP1(false); // strictly off
	
	// fifo config (not much to do here, can try stream mode in SF, see 1_full_info.ino ka final setting section for more info)
	myISM.setFifoMode(ISM_BYPASS_MODE); // fifo off (default)
	
    
  motorA.setTotalSteps(STEPS_PER_REV);
  motorA.setRpm(12);

	delay(100);
  Serial.println("Settings applied.");



  ///// NOT HERE -> do in final_calib
  //////////// Get latest offset/ bias from py from offset.txt
  // if not available then only we go with the hardcoded once and it will print - if we use from py wala or hardcoded wala.



  Serial.println("Using hardcoded gyro bias.");
  Serial.println("Send 'l' over Serial to recalibrate Accel.");
  delay(500);}

// --------------------------------------------------
// LOOP
// --------------------------------------------------
void loop()
{
  if (Serial.available())
  {
    char cmd = Serial.read();

    switch (cmd)
    {
      case 'j':
        Serial.println("\n--- Gyro Calibration Triggered ---");
        calc_gyro_bias();
        break;

      default:
        Serial.print("Unknown command: ");
        Serial.println(cmd);
        break;
    }
  }

  now = micros();
  if (now - lastRead >= PERIOD_US)
  {
    lastRead = now;

    if (myISM.getRawAccel(&rawAccel))
    {
        // -------- LSB -> SCALE TO g  --------
        // (we are not doing 0.061 since that's mg not g. and the sf wants g . dont do 0.000061, since that has binary representation noise)
        double rawX = (rawAccel.xData) * ACC_LSB_TO_G ;
        double rawY = (rawAccel.yData) * ACC_LSB_TO_G;
        double rawZ = (rawAccel.zData) * ACC_LSB_TO_G;

        // -------- SCALED -> CALIB --------
        double meas[3] = {rawX, rawY, rawZ};
        double calib[3] = {0.0, 0.0, 0.0};

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                calib[i] += A[i][j] * (meas[j] - b[j]);
            }
        }

        // ------- Calculate magnitude of the calibrated vector
        // at static position the magnitude should be same as g value
        double magnitude = sqrt(calib[0] * calib[0] + calib[1] * calib[1] + calib[2] * calib[2]);  // no need to do in sf.

        // Print calibrated data (X, Y, Z) and the magnitude // in sf we will not print it , but give it to the sf. 
        Serial.print("Calibrated X: ");
        Serial.print(calib[0], 5);
        Serial.print(", Y: ");
        Serial.print(calib[1], 5);
        Serial.print(", Z: ");
        Serial.print(calib[2], 5);
        Serial.print(" | Magnitude: ");
        Serial.println(magnitude, 5);
    } else {
        Serial.println("Failed to read raw accelerometer data.");
    }
  }
}

