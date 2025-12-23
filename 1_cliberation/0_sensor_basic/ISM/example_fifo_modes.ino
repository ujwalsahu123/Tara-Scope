#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM;

// Structs for X,Y,Z data
sfe_ism_data_t accelData;
sfe_ism_data_t gyroData; 



// -----------------------------------------------------------

void setup(){
 Wire.begin();
 Serial.begin(115200);
 if( !myISM.begin() ){
 Serial.println("Did not begin.");
 while(1); }
 // Reset the device to default settings. This if helpful is you're doing multiple
 // uploads testing different settings.
 myISM.deviceReset();
 // Wait for it to finish reseting
 while( !myISM.getDeviceReset() ){
 delay(1);
 }


 Serial.println("Reset.");
 Serial.println("Applying settings.");
 delay(100);

 myISM.setDeviceConfig();
 myISM.setBlockDataUpdate();

 // Set the output data rate and precision of the accelerometer
	myISM.setAccelDataRate(ISM_XL_ODR_104Hz);
	myISM.setAccelFullScale(ISM_2g); 

	// Set the output data rate and precision of the gyroscope
	myISM.setGyroDataRate(ISM_GY_ODR_104Hz);
	myISM.setGyroFullScale(ISM_125dps); 

	// Turn on the accelerometer's filter and apply settings. 
	myISM.setAccelFilterLP2();
	myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100);

	// Turn on the gyroscope's filter and apply settings. 
	myISM.setGyroFilterLP1();
	myISM.setGyroLP1Bandwidth(ISM_MEDIUM);


// ########################################################## try FIFO MODES ######################################################
// uncomment the mode you want to try.

// i checked nothing much differnce . idk but in the fifo stream mode - thoda slow reflex/ resposne was coming (i think because it was talking time to print older values of the buffer.)


// MODE 1 - FIFO off
// myISM.setFifoMode(ISM_BYPASS_MODE);  

// MODE 2 - FIFO buffer (stops updating after buffer full)
// myISM.setFifoMode(ISM_FIFO_MODE); 

// MODE 3 - FIFO ciruclar Buffer (updates the old with new)
// myISM.setFifoMode(ISM_STREAM_MODE) ; 
// myISM.setAccelFifoBatchSet(ISM_XL_BATCH_AT_104Hz); 
// myISM.setGyroFifoBatchSet(ISM_GY_BATCH_AT_104Hz) ; 



}




void loop(){
 // Check if both gyroscope and accelerometer data is available
  if( myISM.checkStatus() ){   // if it cannot read the sensor data - then remove the if condition - since -> .checkstatus() dosent work

 myISM.getAccel(&accelData);
 myISM.getGyro(&gyroData);
 Serial.print("Acc:    ");
 Serial.print("X: ");
 Serial.print(accelData.xData);
 Serial.print(" ");
 Serial.print("Y: ");
 Serial.print(accelData.yData);
 Serial.print(" ");
 Serial.print("Z: ");
 Serial.print(accelData.zData);
 Serial.print(" ");
 Serial.print("    Gyro:    ");
 Serial.print("X: ");
 Serial.print(gyroData.xData);
 Serial.print(" ");
 Serial.print("Y: ");
 Serial.print(gyroData.yData);
 Serial.print(" ");
 Serial.print("Z: ");
 Serial.print(gyroData.zData);
 Serial.println(" ");

 

  }
}
