// INFO:
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
CheapStepper motorAlt(4, 5, 6, 7);
CheapStepper motorAz(8, 9, 10, 11);

sfe_ism_raw_data_t rawAccel;

unsigned int DataRate_HZ;
unsigned long PERIOD_US;               
unsigned long lastRead = 0;
unsigned long now = 0;

// #######################   CONFIG   ####################### 
#define ACC_LSB_TO_G 0.00006103515625f   // 2g range
#define AVG_MEAS  25 // take Avg of N readings to get 1 stable reading /per orientation.

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
// ##########################################################

double rawX ;
double rawY ;
double rawZ ;
double calibAccel[3] = {0.0, 0.0, 0.0};

// --------------------------------------------------
// CALCULATE ACCEL BIAS USING MOTOR
// --------------------------------------------------

int count = 0;
long ax = 0, ay = 0, az = 0; // Make Avg calculation 0 - before storing new avg calculations.

// function to calculte the avg readings and print it.
void Read_Accel_data()
{
    ax = 0; ay = 0; az = 0; // Make Avg calculation 0 - before storing new avg calculations.
    now = 0;
    lastRead = 0;
    count = 0;
    // take 25 readings ka AVG - for single orientation.
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

    // cannot do calculation of ACC offset and bias over here .
    // so we simply print the raw values and python collects these datapoint and scales it then calculate the offset and bias and stores it in offset.txt
    // and then the arduino asks for the updated offset and bais and python gives it . and then arduino updates the offset and bias values here and gives calib_data as per the updated once.
    // imp -> problem -> the offset that we calculate in python has 10 decimals but arudino cannot take 10 decimals i think -- check how much decimals can the aruino float store .... and as per that we send round off till 6 or 7 decimal place ka offset and bias we send here.
}




//--------------------------------------
// Function which uses 2_motors to collecte the Acc data at differnt orientations
void calc_acc_offset()
{
 
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
/////////// ask python for the updated bias and offset values. 
///////// and update the hardcoded offset and bias. and print also

//   acc_bias[0] = ;
//   acc_bias[1] = ;
//   acc_bias[2] = ;
//   Serial.println("Updated offset and bias ");
//   Serial.print("bx = "); Serial.print(acc_bias[0], 6) .....;
// //   .........



  Serial.println("--- Accel CALIBRATION DONE ---\n");
  delay(1000);
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
	
    
  motorAlt.setTotalSteps(STEPS_PER_REV);
  motorAlt.setRpm(10); // 6 to 24
  motorAz.setTotalSteps(STEPS_PER_REV);
  motorAz.setRpm(10); // 6 to 24

	delay(100);
  Serial.println("Settings applied.");



  ///// NOT HERE -> do in final_calib
  //////////// Get latest offset/ bias from py from offset.txt
  // if not available then only we go with the hardcoded once and it will print - if we use from py wala or hardcoded wala.



  Serial.println("Using hardcoded gyro bias.");
  Serial.println("Send 'l' over Serial to recalibrate Accel.");
  delay(500);
}




// --------------------------------------------------
// LOOP
// --------------------------------------------------
void loop()
{
  if (Serial.available())
  {
    char cmd = Serial.read();

    // Clear the buffer - (newline, extra keys, etc) 
    // so that during the calibration is being done - No key press are recoded. 
    // (other wise while calib you pressed l l l again then after calib it will again do calib 3 more times, since its in the buffer)
    while (Serial.available()) Serial.read();

    switch (cmd)
    {
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

    if (myISM.getRawAccel(&rawAccel))
    {   
      // first scale then calib with scaled_offset/bias.

        // -------- LSB -> SCALE TO g  --------
        // (we are not doing 0.061 since that's mg not g. and the sf wants g . dont do 0.000061, since that has binary representation noise)
        rawX = (rawAccel.xData) * ACC_LSB_TO_G ;
        rawY = (rawAccel.yData) * ACC_LSB_TO_G;
        rawZ = (rawAccel.zData) * ACC_LSB_TO_G;

        // -------- SCALED -> CALIB --------
        // -------- Raw_g -> CALIB using offset/bias_g -> Calib_g  --------
        calibAccel[0] = A_acc[0][0] * (rawX - b_acc[0]) + A_acc[0][1] * (rawY - b_acc[1]) + A_acc[0][2] * (rawZ - b_acc[2]);
        calibAccel[1] = A_acc[1][0] * (rawX - b_acc[0]) + A_acc[1][1] * (rawY - b_acc[1]) + A_acc[1][2] * (rawZ - b_acc[2]);
        calibAccel[2] = A_acc[2][0] * (rawX - b_acc[0]) + A_acc[2][1] * (rawY - b_acc[1]) + A_acc[2][2] * (rawZ - b_acc[2]);

        // ------- Calculate magnitude of the calibrated vector
        // at static position the magnitude should be same as g value
        double magnitude = sqrt(calibAccel[0] * calibAccel[0] + calibAccel[1] * calibAccel[1] + calibAccel[2] * calibAccel[2]);  // no need to do in sf.

        // Print calibrated data (X, Y, Z) and the magnitude // in sf we will not print it , but give it to the sf. 
        Serial.print("Calibrated X: ");
        Serial.print(calibAccel[0], 5);
        Serial.print(", Y: ");
        Serial.print(calibAccel[1], 5);
        Serial.print(", Z: ");
        Serial.print(calibAccel[2], 5);
        Serial.print(" | Magnitude: ");
        Serial.println(magnitude, 5);
    } else {
        Serial.println("Failed to read raw accelerometer data.");
    }
  }
}

