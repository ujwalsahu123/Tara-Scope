// apply caliberation on live data - and see the xyz and Magnitude . 
// by seeing the magnitude you can judge if the offset are good or bad as per (magnitude close to 1).





#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM;

// Define the structure for raw accelerometer data
sfe_ism_raw_data_t rawAccelData;

// Hardcoded offset and bias - (g_2g_Nofilter) // change it as per filter you use
double A_acc[3][3] = {
{0.97522389, 0.00009342, -0.00058922},
{0.00009342, 0.98066595, 0.00053047},
{-0.00058922, 0.00053047, 0.98232234}
};

double b_acc[3] = { 0.00500114, -0.01523379, 0.01161619}; 

// IMP - so that loop run as per the data rate of sensor and not try to run as fast as possible 
unsigned int DataRate_HZ;	
unsigned long PERIOD_US;          
// time tracker for loops -> mainLoop(), while() , for()     
unsigned long lastRead = 0; 
unsigned long now = 0;


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
	delay(100);

    // Apply device settings
    myISM.setDeviceConfig();
    myISM.setBlockDataUpdate(); // Prevent overwriting of unread data
	
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
	

	// fifo config (not much to do here, can try stream mode in SF, see ISM/1_full_info.ino ka final setting section for more info)
	myISM.setFifoMode(ISM_BYPASS_MODE); // fifo off (default)
	

	delay(100);
    Serial.println("Settings applied.");
}

double raw[3] = {0.0, 0.0, 0.0};
double calib[3] = {0.0, 0.0, 0.0};
double magnitude;


void loop() {
    

  now = micros();
  if (now - lastRead >= PERIOD_US)
  {
    lastRead = now;

    // Get raw accelerometer data
    if (myISM.getRawAccel(&rawAccelData)) {

        // -------- LSB -> SCALE TO g  --------
        // (we are not doing 0.061 since that's mg not g. and the sf wants g . dont do 0.000061, since that has binary representation noise)
        raw[0] = (rawAccelData.xData) * 0.00006103515625;
        raw[1] = (rawAccelData.yData) * 0.00006103515625;
        raw[2] = (rawAccelData.zData) * 0.00006103515625;

        // Apply calibration: h_calib = A * (h_meas - b)

        // -------- SCALED -> CALIB --------
        // -------- Raw_g -> CALIB using offset/bias_g -> Calib_g  --------
        calib[0] = A_acc[0][0] * (raw[0] - b_acc[0]) + A_acc[0][1] * (raw[1] - b_acc[1]) + A_acc[0][2] * (raw[2] - b_acc[2]);
        calib[1] = A_acc[1][0] * (raw[0] - b_acc[0]) + A_acc[1][1] * (raw[1] - b_acc[1]) + A_acc[1][2] * (raw[2] - b_acc[2]);
        calib[2] = A_acc[2][0] * (raw[0] - b_acc[0]) + A_acc[2][1] * (raw[1] - b_acc[1]) + A_acc[2][2] * (raw[2] - b_acc[2]);


        // Calculate magnitude of the calibrated vector
        // at static position the magnitude should be same as g value

        magnitude = sqrt(calib[0] * calib[0] + calib[1] * calib[1] + calib[2] * calib[2]);  // no need to do in sf.

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









