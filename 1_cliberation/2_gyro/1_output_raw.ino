// prints raw data - lsb
// set the Range 125, 250dps as per you want the lsb.
// Run this Arduino code and then Run the 2_get-data.py to read this output data.



#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"
SparkFun_ISM330DHCX myISM;
// Structs for X,Y,Z data
sfe_ism_raw_data_t rawGyroData; 


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
	myISM.setGyroDataRate(ISM_GY_ODR_104Hz); // data rate (best -> 104, 208, 416) (for fast use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12 )


	// Range -> always need to set, even if scaling maunally
	myISM.setGyroFullScale(ISM_125dps); 
	// test: in Motor_code if motor moves faster than 2g or 125dps then the sensor values can break - so in that senario we can do 4g or 250dps, but that can lower the precision. other jugad we can do is -> move motor slowly or etc.
	// if you wanna update the range of acc or gyro then - not just you have to change the scaling factor , but again calib (as per that_range_lsb_data * that_range_scaling_factor) and get new offset.
	

	// Filter ->
	myISM.setGyroFilterLP1(false); // strictly off
	

	// fifo config (not much to do here, can try stream mode in SF, see ISM/1_full_info.ino ka final setting section for more info)
	myISM.setFifoMode(ISM_BYPASS_MODE); // fifo off (default)
	

	delay(100);
  Serial.println("Settings applied.");
  Serial.println("t_ms,gx_lsb,gy_lsb,gz_lsb");
}



unsigned long now;
unsigned long last = 0;
const unsigned long PERIOD = 10000; // 100 Hz = 10000 micro second
unsigned long ts ; // time stamp to print


void loop(){

 now = micros(); // now is unsigned long which can store uptill 4,294,960,000 . but 6hr = 21,600,000,000 which is more than the storing capability of now_variable so it will return to 0 again after 4,294,967,295. but thats fine since we are not storing now , we are storing ts. // and condition also works.

 if (now - last >= PERIOD) {
  // we did this because the loop was trying to run as fast as possible , but now it checks last run ka time and now time. so that 100hz par hee it will run. 
   
    last = now; // put it here so that strict 100Hz gaping. (dont put it after print)
    
    if (myISM.getRawGyro(&rawGyroData)) {
      ts = millis(); // printing ts in millis not micro, since cleaner - 10,20,30...
      Serial.print(ts);
      Serial.print(",");
      Serial.print(rawGyroData.xData);
      Serial.print(",");
      Serial.print(rawGyroData.yData);
      Serial.print(",");
      Serial.println(rawGyroData.zData);
    }    
  }

}
