//orange -white -scl-A5 , red - red - sda-A4 ,brown - brown - 3.3 , black - black - gnd

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

      // -----------------------(settings)---------------------------------- 

 Serial.println("Reset.");
 Serial.println("Applying settings.");
 delay(100);

 myISM.setDeviceConfig();
 myISM.setBlockDataUpdate();

 // Set the output data rate and precision of the accelerometer
	myISM.setAccelDataRate(ISM_XL_ODR_104Hz);
	myISM.setAccelFullScale(ISM_4g); 

	// Set the output data rate and precision of the gyroscope
	myISM.setGyroDataRate(ISM_GY_ODR_104Hz);
	myISM.setGyroFullScale(ISM_500dps); 

	// Turn on the accelerometer's filter and apply settings. 
	myISM.setAccelFilterLP2();
	myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100);

	// Turn on the gyroscope's filter and apply settings. 
	myISM.setGyroFilterLP1();
	myISM.setGyroLP1Bandwidth(ISM_MEDIUM);

}




void loop(){
 // Check if both gyroscope and accelerometer data is available
  if( myISM.checkStatus() ){
 myISM.getAccel(&accelData);
 myISM.getGyro(&gyroData);
 Serial.print("Accelerometer: ");
 Serial.print("X: ");
 Serial.print(accelData.xData);
 Serial.print(" ");
 Serial.print("Y: ");
 Serial.print(accelData.yData);
 Serial.print(" ");
 Serial.print("Z: ");
 Serial.print(accelData.zData);
 Serial.println(" ");
 Serial.print("Gyroscope: ");
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
 delay(100);
}



// -------------------------------- filters -------------------------------------


// DATA RATE ------------------------------------------

//  myISM.setAccelDataRate(ISM_XL_ODR_104Hz); // ISM_XL_ODR_OFF, ISM_XL_ODR_12Hz5, ISM_XL_ODR_26Hz, ISM_XL_ODR_52Hz, ISM_XL_ODR_104Hz, ISM_XL_ODR_208Hz, ISM_XL_ODR_416Hz, ISM_XL_ODR_833Hz, ISM_XL_ODR_1666Hz, ISM_XL_ODR_3332Hz, ISM_XL_ODR_6667Hz, ISM_XL_ODR_1Hz	 
//  myISM.setGyroDataRate(ISM_GY_ODR_104Hz); // ISM_GY_ODR_OFF, ISM_GY_ODR_12Hz, ISM_GY_ODR_26Hz, ISM_GY_ODR_52Hz, ISM_GY_ODR_104Hz, ISM_GY_ODR_208Hz, ISM_GY_ODR_416Hz, ISM_GY_ODR_833Hz, ISM_GY_ODR_1666Hz, ISM_GY_ODR_3332Hz, ISM_GY_ODR_6667Hz

// Data rate is basically how fast the data is readed. 
// and it can also affects noise & stability
// Higher rate → more noise per sample
// Lower rate → smoother data but slower response
// Correct rate + filtering → better usable precision


// RANGE ------------------------------------------

// myISM.setAccelFullScale(ISM_4g); // 2 , 4 , 8 ,16g                    
// myISM.setGyroFullScale(ISM_500dps); // 125, 250, 500, 1000, 2000 4000dps

// Range is basically the Sensitivity - smaller range means it can detect smaller changes and is more precise, but big movements can be out of range
// larger range means it can detect bigger movements, but is cannot detect smaller changes and it is less precise
// more sensitive then can detect smaller changes (so precise), but can capture more noise since - smaller noise will also look big since the range is small 




// Acc Filter -------------------------------------

// myISM.setAccelFilterLP2(); // ISM_HP_PATH_DISABLE_ON_OUT, ISM_SLOPE_ODR_DIV_4, ISM_HP_ODR_DIV_10, ISM_HP_ODR_DIV_20, ISM_HP_ODR_DIV_45, ISM_HP_ODR_DIV_100, ISM_HP_ODR_DIV_200, ISM_HP_ODR_DIV_400, ISM_HP_ODR_DIV_800, ISM_HP_REF_MD_ODR_DIV_10, ISM_HP_REF_MD_ODR_DIV_20, ISM_HP_REF_MD_ODR_DIV_45, ISM_HP_REF_MD_ODR_DIV_100, ISM_HP_REF_MD_ODR_DIV_200, ISM_HP_REF_MD_ODR_DIV_400, ISM_HP_REF_MD_ODR_DIV_800
//
// Enables internal low-pass filter2 , it Removes high-frequency vibration


// myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100); // 10, 20, 45, 100, 200, 400, 800
//
// Slope filter - further smoothens data.
// Controls how fast the accelerometer output is allowed to change.
// Lower divider  -> faster response, less smoothing
// Higher divider -> slower response, more smoothing (stable gravity)
// cutoff ≈ ODR / divider
// DIV_10   : Fast motion, noisy
// DIV_20   : Moderate smoothing
// DIV_45   : Balanced
// DIV_100  : Very smooth (good for gravity & tilt)
// DIV_200+ : Extremely slow (almost static)



// gyro filter -------------------------------

//myISM.setGyroFilterLP1(); // ISM_HP_FILTER_NONE, ISM_HP_FILTER_16mHz, ISM_HP_FILTER_65mHz, ISM_HP_FILTER_260mHz, ISM_HP_FILTER_1Hz04,
//
// Enables internal low-pass filter1. it Removes high-frequency vibration
 

// myISM.setGyroLP1Bandwidth(ISM_MEDIUM); //  ISM_ULTRA_LIGHT, ISM_VERY_LIGHT,ISM_LIGHT, ISM_MEDIUM, ISM_STRONG, ISM_VERY_STRONG, ISM_AGGRESSIVE, ISM_XTREME	
//
// GYRO LP1 BANDWIDTH
// Controls how aggressively gyro noise is filtered.
// Light filtering -> fast but noisy
// Strong filtering -> smooth but adds delay
// ULTRA_LIGHT : Almost no filtering (very noisy)
// VERY_LIGHT  : Minimal smoothing
// LIGHT       : Slight smoothing
// MEDIUM      : Best balance (smooth + responsive)
// STRONG      : Smooth, some lag
// VERY_STRONG : Heavy smoothing
// AGGRESSIVE  : Very slow response
// XTREME      : Too slow for motion
// Recommended for motor + fusion systems: MEDIUM


// Without slope filter (or low divider), Accelerometer output looks like this: (faster but noisy)
// 1.00g
// 1.06g
// 0.94g
// 1.03g
// 0.97g

// With slope filter (or high divider), Accelerometer output looks like this: (slower but stable)
// 1.00g
// 1.01g
// 0.99g
// 1.02g
// 1.01g



// -----------------------(library functions)---------------------------------- 

// main library files to read
// sfe_ism330dhcx.h (has all the function list -> setting functions , getting data wale functions , output data ka datatype and struct etc)
// sfe_ism330dhcx.cpp (has all the codes for the functions - ex get_raw_accel(), etc ) (high level code - function is calling the low level function of ism330dhcx_reg.c)
// ism330dhcx_reg.c (has low level read/write and main code - ex functions that read buffer and lsb values , also convert_Lsb_to_2g() , convert_Lsb_to_250dps() , etc )
// ism330dhcx_reg.h


//    All functions list : ----------------------------------

// 		General Settings functions: ----------------------------------
//    bool setAccelFullScale(uint8_t val); // Acc Range
//  	bool setGyroFullScale(uint8_t val); // Gyro Range
//   	uint8_t getAccelFullScale(); // get Acc Range
//   	uint8_t getUniqueId();  // get Device ID
// 		bool setDeviceConfig(bool enable = true); // default device settings
// 		bool deviceReset(); // reset device to default settings
// 		bool getDeviceReset(); // check if device reset is done
// 		bool setAccelSlopeFilter(uint8_t val); // set Acc slope filter
// 		bool setAccelFilterLP2(bool enable = true);  // set Acc low pass filter 2
// 		bool setGyroFilterLP1(bool enable = true); // set Gyro low pass filter 1
// 		bool setGyroLP1Bandwidth(uint8_t val); // set Gyro low pass filter 1 bandwidth
// 		bool setBlockDataUpdate(bool enable = true); // set block data update ( data is not updated until both MSB and LSB have been read from output registers , so you get correct data)
// 		uint8_t getBlockDataUpdate(); // get block data update 
// 		bool setAccelDataRate(uint8_t rate); // set Acc data rate
// 		bool setGyroDataRate(uint8_t rate); // set Gyro data rate
// 		bool enableTimestamp(bool enable = true); // enable timestamp - it does internal counting of time in the device
// 		bool resetTimestamp(); // reset timestamp counter

//    Data retrieval functions : ----------------------------------
// 		int16_t getTemp(); // get raw temperature lsb data
// 		bool getRawAccel(sfe_ism_raw_data_t* accelData); // get raw Accel lsb data
// 		bool getRawGyro(sfe_ism_raw_data_t* gyroData); // get raw Gyro lsb data
// 		bool getAccel(sfe_ism_data_t* accelData); // get converted Accel data in Mg
// 		bool getGyro(sfe_ism_data_t* gyroData); // get converted Gyro data in Mdps


 
//  The output data is output in 2 formats : --------------------------------

//  1. Raw LSB data (int16_t) : use getRawAccel() , getRawGyro()
//  2. Converted data (float) : use getAccel() , getGyro()
//  in converted data -> bas (lsb -> Mg) and (lsb -> Mdps) hota hai as per the g, dps range. 

//  struct sfe_ism_raw_data_t :  // this is for raw lsb data
// 	int16_t xData;	
// 	int16_t yData;	
// 	int16_t zData;

// struct sfe_ism_data_t :  // this is for Mg , Mdps data
// 	float xData;	
// 	float yData;	
// 	float zData;



// Conversion from lsb -> Mg , Mdps ----------------------------------
// it depends on the Range you set : 2g , 4g ... 125dps 250dps ...
// g stands for gravity, and Mg stands for milli-gravity (1/1000 of a g)
// dps stands for degrees per second, and Mdps stands for milli-degrees per second (1/1000 of a dps)
// so we convert lsb to Mg and Mdps , as per the g and dps range set

// library conversion code from LSB to Mg/Mdps : -----------

// float ism330dhcx_from_fs2g_to_mg(int16_t lsb)
// {
//   return ((float)lsb * 0.061f);
// }

// float ism330dhcx_from_fs4g_to_mg(int16_t lsb)
// {
//   return ((float)lsb * 0.122f);
// }

// float ism330dhcx_from_fs8g_to_mg(int16_t lsb)
// {
//   return ((float)lsb * 0.244f);
// }

// float ism330dhcx_from_fs16g_to_mg(int16_t lsb)
// {
//   return ((float)lsb * 0.488f);
// }

// float ism330dhcx_from_fs125dps_to_mdps(int16_t lsb)
// {
//   return ((float)lsb * 4.375f);
// }

// float ism330dhcx_from_fs250dps_to_mdps(int16_t lsb)
// {
//   return ((float)lsb * 8.75f);
// }

// float ism330dhcx_from_fs500dps_to_mdps(int16_t lsb)
// {
//   return ((float)lsb * 17.50f);
// }

// float ism330dhcx_from_fs1000dps_to_mdps(int16_t lsb)
// {
//   return ((float)lsb * 35.0f);
// }

// float ism330dhcx_from_fs2000dps_to_mdps(int16_t lsb)
// {
//   return ((float)lsb * 70.0f);
// }

// float ism330dhcx_from_fs4000dps_to_mdps(int16_t lsb)
// {
//   return ((float)lsb * 140.0f);
// }

// float ism330dhcx_from_lsb_to_celsius(int16_t lsb)
// {
//   return (((float)lsb / 256.0f) + 25.0f);
// }

// float ism330dhcx_from_lsb_to_nsec(int32_t lsb)
// {
//   return ((float)lsb * 25000.0f);
// }

// Range seen : - 
// so there is only 1 differnce between raw lsb data and library converted Mg/Mdps data - (that it is scaled as per Range value) - meaning the lsb value par Range laga kar convert kia jata hai to Mg/Mdps data me.
// so we can also do - get raw LSB value and then khud say manually convert to Mg/Mdps.
// So what i am doing in calibration and sensor fusion is - not using the library converted data , but using the raw lsb data
// and then in calibration - storing that raw lsb data - then usko i convert/scale and then voo data ko i send to magneto to get the offset values
// and in sensor fusion - i use the raw lsb data and then khudsay convert/scale and then apply the offset (so proper unit ka offset is applied) and then use that data for sensor fusion calculations
// why didnt i use the library converted data and khud say manually convert kia, is because - in library it does 0.061, but for more precision i do (more decimal points) - so more precise conversion . and the library wala gives in mg , but we want in g. 
// but i think it also make it slower calculation - so i need to try doing the calibration and sf using the library converted data (less precise conversion hoga but i dont think bohot jyda hee farak padega and calculation speed may hoga... (need to try and see) ) -> NO library converted data takes more time than manually converting using more decimal points (since thereis no - binary representation noise in manually scaling 0.00006103515625)
// Other settings like - data rate, filters (leaving the Range) - wo apply hoga hee wheather you use LSB data or converted data. 
// Raw LSB data is not unfiltered ; Raw LSB data is filtered but unscaled

// raw LSB read and manually convert to g using * 0.00006103515625.   and this is basiclly +-2 g only.


// Data Rate seen : -
// Data rate is the main choke when i used sensor fusion. 
// 


// filter seen : -
// filter - small or medium --- need to try and see which to but ?????????????????



// final settings -





// ------------------------------ Examples see : ----------------------------
// you can also ON/off the readings by interrupts
// you can also use SPI for communincation instead of I2C
// super Fastest reading speed using I2C you can get 
// using both the senosr ISM and MMC at same time - then need to use sensor hub mode.





//Q.g )  -> answer
// Range is decided by sensor hardware, not by math.
// ISM Sensor is built to measure only ±2g, ±4g, ±8g, ±16g. (if there was a sensor which does +-1g then we could have done LSB * 2/2^16 .
//  but that would have not made things more precise. - the thing is the range +2g to -2g means it can detect the acceleartion between the rang +2g to -2g. and precisely.
//  but if the range was +1 to -1 then at rest it would be at max value 1 , since gravity at rest is 1. and if we applied more g than +1 then it would not capture it .
//  and it would only capture free fall (-g) between +1 to -1. so for our application - i think the best is +-2 g . since in motor not goes beyond +2g to 0g . and stays +1g at rest. so this is the best i guess.
// Gravity itself is 1g.
// If range was ±1g, gravity alone would hit the limit.
// Any small movement would go out of range (clip).
// Changing numbers in software does not increase real precision.
// Smaller fake range only makes noise look bigger.
// ±2g is the smallest safe and most precise real range.