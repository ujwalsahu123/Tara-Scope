// prints raw data (2g_lsb) 
// Run this Aduino code and then Run the 2_get-data.py to read this output data.



#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM;

// Define the structure for raw accelerometer data
sfe_ism_raw_data_t rawAccelData;


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

  // Apply device settings
  myISM.setDeviceConfig();
  myISM.setBlockDataUpdate(); // Prevent overwriting of unread data
	
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
	
	// fifo config (not much to do here, can try stream mode in SF, see ISM/1_full_info.ino ka final setting section for more info)
	myISM.setFifoMode(ISM_BYPASS_MODE); // fifo off (default)
	
	delay(100);
    Serial.println("Settings applied.");
}



void loop()
{
	now = micros();
	if (now - lastRead >= PERIOD_US)
	{
		lastRead = now;
		
		if (myISM.getRawAccel(&rawAccelData)) {
    // Print raw data for X, Y, Z axes separated by commas
    Serial.print(rawAccelData.xData);
    Serial.print(",");  // Separate values with a comma
    Serial.print(rawAccelData.yData);
    Serial.print(",");  // Separate values with a comma
    Serial.println(rawAccelData.zData);  // Print Z value and move to next line
  } else {
    Serial.println("Failed to read raw accelerometer data.");
  }
		
	}
}
