//orange -white -scl-A5 , red - red - sda-A4 ,brown - brown - 3.3 , black - black - gnd


// basic code : 
#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM;

// Structs for X,Y,Z data
sfe_ism_data_t accelData;
sfe_ism_data_t gyroData; 


// Define the structure for raw data
sfe_ism_raw_data_t rawAccelData;
sfe_ism_raw_data_t rawGyroData;

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

}




void loop(){
 // Check if both gyroscope and accelerometer data is available
  if( myISM.checkStatus() ){   // if it cannot read the sensor data - then remove the if condition - since -> .checkstatus() dosent work

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

 // Print raw data for X, Y, Z axes of ACC and gyro
 myISM.getRawAccel(&rawAccelData)
 myISM.getRawGyro(&rawGyroData)
 Serial.print("Accelerometer: ");
 Serial.print("X: ");
 Serial.print(rawAccelData.xData);
 Serial.print(" ");
 Serial.print("Y: ");
 Serial.print(rawAccelData.yData);
 Serial.print(" ");
 Serial.print("Z: ");
 Serial.print(rawAccelData.zData);
 Serial.println(" ");
 Serial.print("Gyroscope: ");
 Serial.print("X: ");
 Serial.print(rawGyroData.xData);
 Serial.print(" ");
 Serial.print("Y: ");
 Serial.print(rawGyroData.yData);
 Serial.print(" ");
 Serial.print("Z: ");
 Serial.print(rawGyroData.zData);
 Serial.println(" ");

 // use (var,n) to print more dicimals , ex -  Serial.print(rawGyroData.zData, 5) // so it prints 5 decimals 

  }
}



// ######################################################### Seetings ###########################################################

//######################################################### library functions ######################################################### 

// main library files to read
// sfe_ism330dhcx.h => (has all the function() list - joo joo hum call kar sakte hai -> setting functions , getting data wale functions , output data ka datatype and struct etc)
// sfe_ism330dhcx_defs.h => (has all the Arguments we can pass to the inbuild library functions)
// sfe_ism330dhcx.cpp => (has all the codes for the functions - ex get_raw_accel(), etc ) (high level code - function is calling the low level function of ism330dhcx_reg.c)
// st_src/ism330dhcx_reg.c => (has low level read/write and main code - ex functions that read buffer and lsb values , and acctual code of all functions in the sfe_ism330dhcx.cpp code file -> such as the ism convert_Lsb_to_2g() , convert_Lsb_to_250dps() , etc )
// st_src/ism330dhcx_reg.h



// 		General Settings functions: 
// ----------------------------------------------------------------------
//      bool setAccelFullScale(uint8_t val); // Acc Range
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

//      Data retrieval functions : 
// -----------------------------------------------------------------------------------
// 		int16_t getTemp(); // get raw temperature lsb data
// 		bool getRawAccel(sfe_ism_raw_data_t* accelData); // get raw Accel lsb data
// 		bool getRawGyro(sfe_ism_raw_data_t* gyroData); // get raw Gyro lsb data
// 		bool getAccel(sfe_ism_data_t* accelData); // get converted Accel data in Mg
// 		bool getGyro(sfe_ism_data_t* gyroData); // get converted Gyro data in Mdps


 
//  The output data is output in 2 formats : 
// --------------------------------------------------------------------------------------
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

// Conversion from lsb -> Mg , Mdps
// --------------------------------------------------------------------------------------------
// it depends on the Range you set : 2g , 4g ... 125dps 250dps ...
// g stands for gravity, and Mg stands for milli-gravity (1/1000 of a g)
// dps stands for degrees per second, and Mdps stands for milli-degrees per second (1/1000 of a dps)
// so we convert lsb to Mg and Mdps , as per the g and dps range set

// library conversion code from LSB to Mg/Mdps : 
// ---------------------------------------------------------------------------------------------

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

//######################################################### RANGE #########################################################

// myISM.setAccelFullScale(ISM_4g); // 2 , 4 , 8 ,16g                    
// myISM.setGyroFullScale(ISM_500dps); // 125, 250, 500, 1000, 2000 4000dps

// Range is basically the Sensitivity - smaller range means it can detect smaller changes and is more precise, but big movements can be out of range
// larger range means it can detect bigger movements, but is cannot detect smaller changes and it is less precise
// more sensitive then can detect smaller changes (so precise), but can capture more noise since - smaller noise will also look big since the range is small 



// Range() Value seen : 
// ------------------------------------------------------------------------------------------------------
// so there is only 1 differnce between raw lsb data and library converted Mg/Mdps data - (that it is scaled as per Range value) - meaning the lsb value par Range laga kar convert kia jata hai to Mg/Mdps data me.
// NO THAT IS NOT TRUE - THAT we get LSB value and uspar jo man hai voo range laga doo -> 2g , 8g , 125dps, 500dsp .  
// SINCE AS PER THE RANGE THE LSB VALUES ARE GIVEN -> ex 2g_lsb = 16494, 4g_lsb = 8849, 8g_lsb = 4135 ...
// AND THEN AS PER THAT LSB_RANGE WE SCALE -> 2g_lsb * 2g_scale(0.061) ..  4g_lsb * 4g_scale(..)  like that.
// so we can also do - get raw LSB value and then khud say manually convert to Mg/Mdps. // YES WE CAN DO THIS BUT NEED TO SET RANGE FOR LSB ALSO AND FIR USS HEE RANGE KA SCALE KARNA HOGA 2G_RANGE_KA_LSB THEN 2G_RANGE_KA_SCALE.

// So what i am doing in calibration and sensor fusion is - not using the library converted data , but using the raw lsb data_2G and then 2G_range scale it.
// and then storing that raw lsb data_2g - then usko i scale to g as per LSB_data ka range and then voo data ko i send to magneto to get the offset values
// and in sensor fusion - i use the raw lsb data_2g and then khudsay scale and then apply the offset (so proper unit ka offset is applied) and then use that data for sensor fusion calculations
// why didnt i use the library converted data and khud say manually convert kia, is because - in library it does 0.061, but for more precision i do (more decimal points) - so more precise conversion . and the library wala gives in mg , but we want in g. 
// but i think it also make it slower calculation - so i need to try doing the calibration and sf using the library converted data (less precise conversion hoga but i dont think bohot jyda hee farak padega and calculation speed may hoga... (need to try and see) ) 
// -> NO library converted data takes more time than manually converting using more decimal points (since thereis no - binary representation noise in manually scaling 0.00006103515625)
// SO ON LSB VALUES ALL THESE THINGS ARE APPLIED -> *RANGE* , DATARATE , FILTERS ..

// default is 2g and 250dps. // if you dont set then it will be default set.

// SO for acc -> LSB_2g * 0.000061015625
// SO for gyro -> LSB_125dps * 0.004375

// WRONG :- so no need to put any value for set_Range(). since use use raw lsb.
// RIGHT :- Always need to set the range whether we get lsb or scaled data. since even lsb data it gives as per the range().

myISM.setAccelFullScale(ISM_2g); // always set range             
myISM.setGyroFullScale(ISM_125dps); // always set range



// ######################################################### DATA RATE #########################################################

//  myISM.setAccelDataRate(ISM_XL_ODR_104Hz); // ISM_XL_ODR_OFF, ISM_XL_ODR_12Hz5, ISM_XL_ODR_26Hz, ISM_XL_ODR_52Hz, ISM_XL_ODR_104Hz, ISM_XL_ODR_208Hz, ISM_XL_ODR_416Hz, ISM_XL_ODR_833Hz, ISM_XL_ODR_1666Hz, ISM_XL_ODR_3332Hz, ISM_XL_ODR_6667Hz, ISM_XL_ODR_1Hz6 
//  myISM.setGyroDataRate(ISM_GY_ODR_104Hz); // ISM_GY_ODR_OFF, ISM_GY_ODR_12Hz, ISM_GY_ODR_26Hz, ISM_GY_ODR_52Hz, ISM_GY_ODR_104Hz, ISM_GY_ODR_208Hz, ISM_GY_ODR_416Hz, ISM_GY_ODR_833Hz, ISM_GY_ODR_1666Hz, ISM_GY_ODR_3332Hz, ISM_GY_ODR_6667Hz

// Data rate is basically how fast the data is readed. 
// and it can also affects noise & stability
// Higher rate → more noise per sample
// Lower rate → smoother data but slower response
// Correct rate + filtering → better usable precision
// need to put this code in setup() if want to use 416Hz or faster -> //Wire.setClock(400000); 

// Data Rate() Value seen : 
// -----------------------------------------------------------------------------------------------------------------
// Data rate is the main choke when i used sensor fusion. 
// 104hz -> 104 Hz → new data every 9.6 ms,  208 Hz → every 4.8 ms,  416 Hz → every 2.4 ms
//////// Wire.setClock(400000); // put the code in void setup to use 400kHz I2C. Essential when running the accel and gyro at 416Hz or faster.
//////// 104, 208, 416hz is the best.
//////// (if feel slower then use -> 833, 1666, 3332, 6667) (if feel faster then use -> 52, 26, 12 )
// Faster Data Rate also has cons -> 
// more noise ( since its not getting enough time to avg or properly calculate the hardware readings),
// less precise output -> since bandwidth is busy with other values also.
// Noise density stays the same but Noise per sample increases . So: 104 Hz → smoother samples, 416 Hz → noisier samples, 833+ Hz → very noisy unless filtered. This is why filters are very imp at high ODR.
// there is FIFO -> its not like -> we can just ask the sensor for the latest value and it gives us the latest value. (so high data rate rakhte hai and we will ask data whenever we want <- in this case old data from fifo is read and not the latest one)

// in FIFO mode ON : it keeps the data in a buffer -> so FIFO hota hai, so it keeps a lot of data in FIFO buffer and if you read slower than its insertion rate - then you are reading older values of the buffer. 
// in FIFO mode OFF : Registers always contain only the latest sample, old samples are lost, You only get the most recent value, This is called “latest-value mode”
// i think this was Making the SF slower - since the sf was reading older values from the buffer. ( so may be we can try - fifo off )
// gpt tell to put fifo ON at high speed , because FIFO is needed at high data rates so the MCU doesn’t miss motion data; missing data makes fusion respond very slowly.
// matlab - fusion reads skipped data , and thats why it dosent get all data , and it becomes slow. 
// need to figure out what fifo to put in sf . 

myISM.setAccelDataRate(ISM_XL_ODR_208Hz); // data rate (best -> 104, 208, 416) (for faster use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12Hz5, 1Hz6 )
myISM.setGyroDataRate(ISM_XL_ODR_208Hz); // data rate (best -> 104, 208, 416) (for faster use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12Hz5, 1Hz6 )


// ######################################################### fifo settings #########################################################


// FIFO Mode Settings :
// -------------------------------------------------------------------------------------------------------
// myISM.setFifoMode(); // put in void 
// ISM_BYPASS_MODE -> FIFO disabled (default) , Sensor registers always hold latest value only , Old samples are lost
// ISM_FIFO_MODE -> FIFO fills up, When full → stops, Good for finite captures , ex : “Collect N samples and stop”
// ISM_STREAM_MODE -> FIFO never stops, Oldest data is overwritten, Always contains latest history, ex : Rolling buffer of recent samples , used in SF
// ISM_STREAM_TO_FIFO_MODE -> change mode
// ISM_BYPASS_TO_STREAM_MODE  
// ISM_BYPASS_TO_FIFO_MODE



// FIFO Accelerometer Batch Settings
// -------------------------------------------------------------------------------------------------------
// myISM.setAccelFifoBatchSet();  -> decide what gets stored into FIFO and at what rate., ex: Store accel data into FIFO at this rate
// this is different from sensor Data Rate
// ex : Sensor ODR = 416 Hz , FIFO batch = 208 Hz -> so fifo stores every 2nd sample
// ISM_XL_NOT_BATCHED (default) // Accel data NOT stored in FIFO
// ISM_XL_BATCH_AT_12Hz5
// ISM_XL_BATCH_AT_26Hz
// ISM_XL_BATCH_AT_52Hz
// ISM_XL_BATCH_AT_104Hz
// ISM_XL_BATCH_AT_208Hz
// ISM_XL_BATCH_AT_417Hz
// ISM_XL_BATCH_AT_833Hz
// ISM_XL_BATCH_AT_1667Hz
// ISM_XL_BATCH_AT_3333Hz
// ISM_XL_BATCH_AT_6667Hz
// ISM_XL_BATCH_AT_6Hz5



// FIFO Gyroscope Batch Settings
// -------------------------------------------------------------------------------------------------------
// myISM.setGyroFifoBatchSet() ;  -> same as above
// ISM_GY_NOT_BATCHED (dafault)
// ISM_GY_BATCH_AT_12Hz5
// ISM_GY_BATCH_AT_26Hz
// ISM_GY_BATCH_AT_52Hz
// ISM_GY_BATCH_AT_104Hz
// ISM_GY_BATCH_AT_208Hz
// ISM_GY_BATCH_AT_417Hz
// ISM_GY_BATCH_AT_833Hz
// ISM_GY_BATCH_AT_1667Hz
// ISM_GY_BATCH_AT_3333Hz
// ISM_GY_BATCH_AT_6667Hz
// ISM_GY_BATCH_AT_6Hz5

// fifo is basically default off - so we can make fifo on and at particular rate, and timestamp -> in SF
// data rate and fifo rate -> keep same or differnt as you want.




// time : 
// -------------------------------------------------------------------------------------------------------
// myISM.setFifoWatermark(200); // indicate when n samples are stored // ex : Tell me when FIFO has 200 samples stored // defalut : 0
// it does not print anything or stop fifo . it just makes a interrupt pin vlaue -> high . and use can use that pin in condition to print / run some command (when the watermark is hit)
// FIFO keeps filling normally , and When stored samples ≥ watermark: A status flag becomes TRUE and Optionally an interrupt pin goes HIGH
// it exists : To avoid reading FIFO too early, To let MCU know: “Enough data is ready, now read me”
// INT1 or INT2 interrupt pin is high - but checkout the library interrupt pin section // or route the FIFO watermark event to an interrupt pin.
// this can be imp in SF . to know the data rate.


// Decimation rate -> Put timestamp once every N samples
// myISM.setFifoTimestampDec() 
// ISM_NO_DECIMATION -> dont put time stamp // default
// ISM_DEC_1 ->  every sample
// ISM_DEC_8  ->  every 8th sample
// ISM_DEC_32 ->  every 32th sample
// Controls how often a timestamp is stored into FIFO
// it does not print time , etc. , it just stores the timestamp in fifo after every n sample . 
// ex : fifo does not store [X, Y, Z, TIMESTAMP] . it stores in packets : [ACC sample] [GYRO sample] [ACC sample] [TIMESTAMP sample] [GYRO sample] //
// and then need to access the packet and parse it to get timestamp in it. 
// so not usefull. 




// final fifo values :
// -------------------------------------------------------------------------------------------------------
myISM.setFifoMode(ISM_BYPASS_MODE); // default is bypass - Sensor always exposes ONLY the latest sample and fifo is off.  (ISM_FIFO_MODE) FIFO buffer fills until full, then STOPS writing new samples. (ISM_STREAM_MODE) for contineous updating old data circular buffer (old data overwritten by new data).
// below funcions use only if fifo is enabled (fifo_mode, stream_mode)
// myISM.setAccelFifoBatchSet(ISM_XL_BATCH_AT_104Hz); // keep same Hz as data rate or differnt as per how you want to store the readings
// myISM.setGyroFifoBatchSet(ISM_GY_BATCH_AT_104Hz) ; // 
// myISM.setFifoWatermark(n); //  Useful to know "FIFO has accumulated n samples" - itne samples hogaye hai. (can use in sf for debugging)
// myISM.setFifoTimestampDec(ISM_NO_DECIMATION) // Timestamp stored for every sample  // not usefull since we cannot access the timestamps.
// lets try FIFO_mode and Stream_mode in SF (maybe it can solve the laggy problem).....



// ######################################################### Filter ######################################################### 


// ACC filter :
// ----------------------------------------------------------------------------------------------------------------------
// myISM.setAccelFilterLP2(true); // it Enables internal low-pass filter2 which Removes high-frequency vibration

// myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100); 
// ISM_HP_PATH_DISABLE_ON_OUT
// ISM_SLOPE_ODR_DIV_4
// ISM_HP_ODR_DIV_10  
// ISM_HP_ODR_DIV_20
// ISM_HP_ODR_DIV_45
// ISM_HP_ODR_DIV_100
// ISM_HP_ODR_DIV_200
// ISM_HP_ODR_DIV_400
// ISM_HP_ODR_DIV_800
// ISM_HP_REF_MD_ODR_DIV_10
// ISM_HP_REF_MD_ODR_DIV_20
// ISM_HP_REF_MD_ODR_DIV_45
// ISM_HP_REF_MD_ODR_DIV_100
// ISM_HP_REF_MD_ODR_DIV_200
// ISM_HP_REF_MD_ODR_DIV_400
// ISM_HP_REF_MD_ODR_DIV_800
// ISM_LP_ODR_DIV_10
// ISM_LP_ODR_DIV_20
// ISM_LP_ODR_DIV_45
// ISM_LP_ODR_DIV_100
// ISM_LP_ODR_DIV_200
// ISM_LP_ODR_DIV_400
// ISM_LP_ODR_DIV_800
// HP - high pass filters - we dont want - they removes slow signals,  Gravity is a slow (DC) signal, Breaks tilt & orientation, Used for motion / wake-up detection only
// Low Pass Filters we want Low-pass = keeps gravity , Smooths noise, Controls responsiveness, works for sensor fusion

// Slope filter - smoothens data.
// Controls how fast the accelerometer output is allowed to change.
// Lower divider  -> faster response, less smoothing
// Higher divider -> slower response, more smoothing (stable gravity)
// cutoff ≈ ODR / divider
// DIV_10   : Fast motion, noisy
// DIV_20   : Moderate smoothing
// DIV_45   : Balanced
// DIV_100  : Very smooth (good for gravity & tilt)
// DIV_200+ : Extremely slow (almost static)



// gyro filter :
// ------------------------------------------------------------------------------------------------------------------------

// myISM.setGyroFilterLP1(true); // Enables internal low-pass filter1 which Removes high-frequency vibration

// myISM.setGyroLP1Bandwidth(ISM_MEDIUM); 
// ISM_ULTRA_LIGHT
// ISM_VERY_LIGHT
// ISM_LIGHT
// ISM_MEDIUM
// ISM_STRONG
// ISM_VERY_STRONG
// ISM_AGGRESSIVE
// ISM_XTREME

// ISM_HP_FILTER_NONE
// ISM_HP_FILTER_16mHz
// ISM_HP_FILTER_65mHz
// ISM_HP_FILTER_260mHz
// ISM_HP_FILTER_1Hz04
// we dont want HP , we want LP.

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

// Example : 
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

// so Property of filters -
// time will be almost the same for all filters 
// higher filter does more noise reduction when the sensor is at motion
// so if we think ki -> higher filter does more noise reduction in almost same time - then its the best (wrong)
// since filters make Phase-lag Slow . Ex - You rotate sensor by 10° -> With LP_DIV_10 → output moves immediately , With LP_DIV_800 → output reaches 10° after many samples
// becuase it does averaging with old samples. 
// Heavy filtering can make Responsiveness slower since it does average with  more previous values.
// trade off is -> noise vs responsiveness
// maybe this cased the sf - to take 1min to come to posi. // so we can try to turn of the filters in sf
// DIV_10  → noisy, fast
// DIV_20  → less noise, still fast
// DIV_45  → good balance
// DIV_100 → usable but starting to lag
// DIV_200+→ too slow for fusion



// filter Value seen : 
// -----------------------------------------------------------------------------------------------------------
// we should keep filters - as they make things more smooth. 
// for other applications -> (Acc - off / 20 / 45) (gyro - off , very_light) - // Read the filter_test.py for more info. 
// But for Sensor fusion :  responsiveness >  smoothining . because it does averaging and smoothining himself.
// Sensor fusion algorithms are temporal filters, adding hardware filters causes double-filtering and lag. // maybe this was the reason of slow sf rpy
// so for sf -> no filtering is better.


// can keep acc filter on and gyro filter off - and its fine. since internally there is nothing like gyro data are influenced by acc data (so if acc may filter hai then if effect the gyro data also) NO. 
// both data are independent so we can keep acc-on and gyro-off. 

myISM.setAccelFilterLP2(false); 
//.setAccelSlopeFilter(ISM_LP_ODR_DIV_20); // can keep 10, 20, 45  
myISM.setGyroFilterLP1(false); // strictly off




// ######################################################### Block Data #########################################################


	bool setBlockDataUpdate(bool enable = true); // set block data update ( data is not updated until both MSB and LSB have been read from output registers , so you get correct data)
	uint8_t getBlockDataUpdate(); // get block data update 

// Each axis value is 16-bit, but the MCU reads it in two steps:
// read LSB
// read MSB
// Without protection, the sensor might update the value in between those two reads.
//  When enabled, the sensor freezes the output registers until both MSB and LSB are read.
// Bascially - these make sure that agay peechay wala data naa read hoo. 
// example : BDU = ON (safe mode) -> Once new data is ready → it freezes the registers, then Waits until both LSB + MSB are read, and Only then updates to next sample


// or  is it like -> // Prevent overwriting of unread data
// matlab -> it will not update the registers until the last reading is readed. ? and then only it will update the new values.
// in this case -> if the sensor was x = 90 and we didnt read the value after then and then we move the sensor to x = 120 and read the value then x = 90 wala value read hoga(since it didnt update the unread data) ? 

// Block Data Update (BDU) works like this:
// The sensor keeps measuring internally all the time (orientation 90 → 120 → 150 etc. keeps updating inside).
// BDU does NOT stop the sensor from measuring.
// It only controls when the output registers are allowed to change.
// What actually happens
// Each axis value is stored as LSB + MSB.
// Without BDU ❌
// → You might read LSB from old sample and MSB from new sample → corrupted value.
// With BDU ✅
// → Once you start reading either LSB or MSB, the sensor freezes both until both are read.
// → After both are read, registers update to the latest available measurement.
// Important point (your main doubt)
// ❌ It does NOT mean: “If you didn’t read last time (x=90), next read will still give 90”
// ✅ Wrong fear
// The sensor will give the latest completed sample, not an old one.




// // ######################################################### temperature  #########################################################

// Function calls ------------
// bool checkTempStatus(); -> check if we can get the temp or not.
// int16_t getTemp(); -> to get the raw temp value from sensor.
// float convertToCelsius(int16_t data); -> function to convert raw temp to degree Celcius.
		
// inner library functions ----------
// float ism330dhcx_from_lsb_to_celsius(int16_t lsb)
// {
// return (((float)lsb / 256.0f) + 25.0f);
// }
// float QwDevISM330DHCX::convertToCelsius(int16_t data)
// {
// return(ism330dhcx_from_lsb_to_celsius(data));
// } 

////////   Temp sensor only works when the ISM sensor is ON , other wise the checkTempStatus() gives False. and getTemp() gives 0 value.
// so to turn ON the ISM sensor -> we need to either turn on the Accel or Gyro . 
// and to do that -> you have to set the Datarate of anyone of them -> and thus the ISM sensor turns on.
// (dosent matter the data_rate value , bass you must call the DataRate() function.) 
myISM.setAccelDataRate(ISM_XL_ODR_104Hz); 
myISM.setGyroDataRate(ISM_GY_ODR_104Hz);
// (calling any one also is fine.)


// code for getting the Temp ----------------------
if (myISM.checkTempStatus())
{
  int16_t rawTemp = myISM.getTemp(); // get the rawTemp
  float tempC = myISM.convertToCelsius(rawTemp); // conver to Celsius

  Serial.print("Raw temp: ");
  Serial.print(rawTemp);
  Serial.print(" | Temperature: ");
  Serial.print(tempC, 2);
  Serial.println(" °C");
  delay(500);
}

// very accurate -> +-0.5 deg celcius max differnce from the Local Weather app . so i think pretty accurate Temp.



// // ######################################################### check status / test #########################################################		

// Status
		bool checkStatus();
		bool checkAccelStatus();
		bool checkGyroStatus();
		bool checkTempStatus();

// Self Test
		bool setAccelSelfTest(uint8_t val);
		bool setGyroSelfTest(uint8_t val);

// // ######################################################### offset seen #########################################################









// ################################################### LOOP ########################################################################

// Big problem : The loop Tries to run as fast as possible, so it may run faster than the DataRate, which can cause poblems
// ex -> Sensor DataRate 104hz so it gives around 104_values per second but the loop runs at 812hz (loop runs as fast as the MCU can execute the code)
// so the Loop treies to get the Sensor data but more faster than it updates new values , so thus we get Repeated data.
// So to solve -> "loop tries to run as fast as possible." we need to put a timer before each iteration so that the loop can run at same Hz as the Sensor Datarate
// so need to use Timing_system in all Loops : 
// 1) Main_loop()
// 2) While() / for() -> Whenever you Want to READ Sensor Data using a LOOP 
// this works perfectly - i tested at 104hz for 5sec and the count was 520 (exact)


// ------------------------------------------------------------------------
// need to always set this in the top or setup() function.
unsigned int DataRate_HZ = 104; // keep same as sensor_Hz
unsigned long PERIOD_US= (1000000UL / DataRate_HZ);
unsigned long lastRead = 0; // correct // assign at the time of defining 
unsigned long now = 0;
// lastRead = 0; // wrong - since in global only decalration is allowed not statements are not (reassigning a variable is a statement)
// now = 0;

// -----------------------------------------------
void loop()
{
	now = micros();
  if (now - lastRead >= PERIOD_US)
  {
	  lastRead = now;
    
	  if (myISM.getRawGyro(&rawGyroData)&&myISM.getRawAccel(&rawAccelData))
    {
		//   get data / code ...
    }
	
  }
}



// ------------------------------------------------------------------------
lastRead = 0; // reset before use
// need to reset this at both -> before and after the loop (you will tell - but last may if we reset then why need to reset at top -> because since if Main function may bhi use hua hai LastRead and got updated, and then vaha say fun() call hua where we run this loop then last time ka values - use hoga on the first loop.... kuch fark nahi padega but its safe...)
// so now you dont need to write lastRead =0 in the main loop before calling a function which has this kinda technique ka loop.
while( condition -> N_samples / N_seconds / true / anyother condition.  )
{
	// Run loop as per Sensor Hz and not Run as fast as possible.
    now = micros();
    if (now - lastRead >= PERIOD_US)
    {
		lastRead = now;
		
		if (myISM.getRawGyro(&rawGyroData)&&myISM.getRawAccel(&rawAccelData))
		{
	  //   get data / code ... 
		}

    }
}
// After the loop Always retset these for further use in the codefile. 
// if didnt reset then next time if we use these variables then it already contains some value. 
lastRead = 0; // reset after use



// Loop for getting N_samples from sensor -----------------------------------------------------------
unsigned int count = 0;
unsigned int N_samples = N; 
lastRead = 0; // reset before use

while (count < N_SAMPLES) 
{
	now = micros();
    if (now - lastRead >= PERIOD_US)
    {
		lastRead = now;
		
		if (myISM.getRawGyro(&rawGyroData)&&myISM.getRawAccel(&rawAccelData))
		{
			//   get data / code ...
			count++;
		}
		
    }
}
lastRead = 0; // reset after  use



// Loop for getting samples for N_seconds from sensor -------------------------------------------------------
unsigned long N_seconds = N; // 5 sec -> 5000
unsigned long start = millis();
lastRead = 0; // reset before use
while (millis() - start < TIME_MS)
{
	  now = micros();
	  if (now - lastRead >= PERIOD_US)
    {
      lastRead = now;
	  
      if (myISM.getRawGyro(&rawGyroData)&&myISM.getRawAccel(&rawAccelData))
      {
		  //   get data / code ...
	  }
		
    }
}
lastRead = 0; // reset after use




// ######################################################### Inerrupt Pin #########################################################

// Interrupt pins are hardware “ready signals” from the IMU to the MCU.
// Instead of:
// MCU repeatedly asking: “New data? New data?” (polling)
// You do:
// IMU says: “Hey! New data is ready NOW.” (interrupt)
// This is more accurate, lower jitter, and more efficient.

// What do they signal?
// On ISM330DHCX, interrupts can signal:
// ✅ Accelerometer data ready
// ✅ Gyroscope data ready
// FIFO watermark / overflow
// Embedded functions (FSM, ML core, etc.)
// Sensor hub data ready

// Your board has 3 interrupt pins
// 🔹 INT1 (ISM330DHCX)
// Output only
// Can signal:
// Accel data ready
// Gyro data ready
// FIFO events
// Motion / wake-up / step events

// 🔹 INT2 (ISM330DHCX)
// Input or output
// Used for:
// Data ready (same as INT1)
// Sensor Hub synchronization
// FSM / ML core events
// 👉 INT2 is special because it can be used to sync sensor-hub reads.

// 🔹 MINT (MMC5983MA magnetometer)
// Interrupt from magnetometer only
// Signals:
// Measurement done
// Data ready


// using Interrup pin :-
// You never read half-updated data
// You read exactly at ODR
// Your timing jitter → near zero

// Interrupt notification modes (important)
// ISM_ALL_INT_PULSED
// 🔹 Pulsed
// Short pulse
// Must catch it in time (fast MCU)

// ISM_ALL_INT_LATCHED
// 🔹 Latched ✅ (recommended)
// Pin stays HIGH/LOW until you read status register
// Much safer
// 👉 For Arduino-level work: LATCHED

// How interrupts help YOU specifically
// ✅ Accelerometer + Gyro
// INT1 or INT2 → data ready
// Read acc + gyro exactly at 104 Hz
// Perfect for sensor fusion

// ⚠️ Magnetometer (MMC)
// Has MINT
// Fires when measurement is done
// But measurement must be triggered first
// This is why MMC is tricky.
// INT1 and INT2 are output pins from ISM, configurable.
// MINT is output from MMC, totally separate.


// xtra:
// Sensor Hub + Interrupts (key insight)
// In Sensor Hub mode:
// ISM controls MMC
// INT2 can be used to:
// Trigger hub reads
// Sync acc + gyro + mag
// BUT…
// ⚠️ MMC needs a trigger bit every read
// → That’s why SparkFun says “not ideal”
// Not because it’s impossible
// But because it’s not free-running

// Pins on your board
// 🔹 ISM330DHCX
// INT1 → typically used for Accel/Gyro Data Ready
// INT2 → can be used for:
// Data ready
// FIFO
// Sensor hub sync

// 🔹 MMC5983MA
// MINT → magnetometer data ready



// Interrupt Settings
bool setAccelStatustoInt1(bool enable = true); // Every time new accel data is produced, INT1 toggles
bool setAccelStatustoInt2(bool enable = true); // Every time new accel data is produced, INT2 toggles
bool setGyroStatustoInt1(bool enable = true); // Every time new Gyro data is produced, INT1 toggles
bool setGyroStatustoInt2(bool enable = true); // Every time new Gyro data is produced, INT2 toggles
// mostly the INT1 is for ISM , and INT2 is for other sensors using sensor hub. but we can use accel for int1 and gyro for int2
// but we can also use INT1 for both accel and gyro 
// so if either one is ready then INT1 toggles. 
// "If accel + gyro are both enabled on INT1 → INT1 fires for either" . so basically both the acc and gyro gets updates at the same time.. so we can simply just check the interrupt pin of acc or gryo any one of them. and then simply read both acc and gyro updated values , but both accel and gyro must be at same ODR rate
// Yes 👍 If ACC + GYRO data-ready are both routed to INT1, then INT1 will trigger whenever either new accel or new gyro data is ready.
// In practice (since both usually run at the same ODR):
// INT1 fires
// You read both accel and gyro
// You get the latest updated values of both


bool setIntNotification(uint8_t val); // Controls HOW the interrupt behaves electrically // This decides which events are allowed to trigger INT pins.
// matlab output kaisa hoga ... just one quick high signal when data is ready or hold karna high signal ...
// ISM_ALL_INT_PULSED -> Interrupt pin pulses briefly (short spike (easy to miss without seperate ISR function-which checks))
// ISM_ALL_INT_LATCHED	-> Interrupt stays HIGH/LOW until cleared (stays asserted until data is read (safe))
// ISM_BASE_LATCHED_EMB_PULSED -> 
// ISM_BASE_PULSED_EMB_LATCHED -> 
// 🔹 Pulsed
// Interrupt pin goes HIGH (or LOW) for a very short pulse
// Then it automatically returns to inactive
// MCU must catch the edge (via hardware interrupt)
// If MCU is busy → you can miss it
// 👉 Think: “tap on the shoulder”
// 🔹 Latched
// Interrupt pin goes HIGH (or LOW) and STAYS there
// It stays active until you read the status / data registers
// Impossible to miss
// 👉 Think: “alarm stays ON until you acknowledge”

// ISM_ALL_INT_PULSED -> All interrupts are pulsed , Base + embedded events = pulses
// ISM_ALL_INT_LATCHED -> All interrupts are latched , Base + embedded events = latched
// ISM_BASE_LATCHED_EMB_PULSED -> Base interrupts (data-ready: accel/gyro) → latched , Embedded events (FSM, MLC, step counter) → pulsed
// ISM_BASE_PULSED_EMB_LATCHED -> Base → pulsed , Embedded → latched
// 👉 These matter only if you use FSM / MLC / step detection

bool setDataReadyMode(uint8_t val); // this is differnt from Notification // This applies only to DATA-READY signals (accel / gyro).
// 0 = Latched → interrupt clears only after data read
// 1 = Pulsed → interrupt auto-clears

bool setPinMode(bool activeLow = true); //Sets electrical polarity of INT pins
// true -> Interrupt goes LOW when active
// false -> Interrupt goes HIGH when active // When data is ready → drive the pin HIGH”




// final --------------------------------------------------

// interrupt pin - is not like blockdata. -> so its not like ki data will only be updated after the data is readed. (data will contineously update and interrupt pin ka kam hai baas ki track ki the data is new or not..)
// 👉 Interrupt pin does NOT hold the sensor data.
// 👉 It only holds the signal (HIGH/LOW).

// we can use the Interrupt pin with the loop so we read all the data and we dont need to do loop timer .... we simply read the data when the new data is available.

#define INT1_PIN ?
#define INT2_PIN ?

myISM.setAccelStatustoInt1(true);   // ACC → INT1 // INT1 fires when new accelerometer data is ready
myISM.setGyroStatustoInt2(true);    // GYRO → INT2 // INT2 fires when new gyroscope data is ready
myISM.setDataReadyMode(0);   // 1 = pulsed, 0 = latched  
myISM.setIntNotification(ISM_ALL_INT_LATCHED); // Latched -> Interrupt pin stays HIGH/LOW until the data is read // So we dont miss anydata
myISM.setPinMode(false); //When data is ready → drive the pin "HIGH” and after data read it gives LOW until the data is updated


// use this way to whenever you want to Get the RAW data from the Senosr.
// check if both acc and gyro interrupt pins are high -> (new data is updated after last read)
if( (digitalRead(INT1_PIN) == HIGH) && (digitalRead(INT2_PIN) == HIGH) )
{	
	// get data
	if (myISM.getRawGyro(&rawGyroData)&&myISM.getRawAccel(&rawAccelData))
		{
			Serial.print("Accelerometer: ");
			Serial.print("X: ");
			Serial.print(rawAccelData.xData);
			Serial.print(" ");
			Serial.print(rawAccelData.yData);
			Serial.print("Y: ");
			Serial.print(" ");
			Serial.print("Z: ");
			Serial.print(rawAccelData.zData);
			Serial.println(" ");
			Serial.print("Gyroscope: ");
			Serial.print("X: ");
			Serial.print(rawGyroData.xData);
			Serial.print(" ");
			Serial.print("Y: ");
			Serial.print(rawGyroData.yData);
			Serial.print(" ");
			Serial.print("Z: ");
			Serial.print(rawGyroData.zData);
			Serial.println(" ");
		}
}












// ##################################################################################################################################### 
// ######################################################### Final Setting ISM ######################################################### 



// final acc settings
// --------------------------------------------------
// Wire.setClock(400000); // uncomment it if using 416hz or higher data rate , other wise comment it.
// datarate -> 104 hz is good since fast and less noisy . and as you increase the Hz the noise also increase (not increases but noise occurs more)
myISM.setAccelDataRate(ISM_XL_ODR_104z); // data rate (best -> 104, 208, 416) (for faster use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12Hz5, 1Hz6 )
myISM.setAccelFullScale(ISM_2g); // always need to set range , even if scaling manually.
// test: in Motor_code if motor moves faster than 2g then the sensor values can break - so in that senario we can do 4g or 8g or 250dps , etc  , but that can lower the precision. other jugad we can do is -> move motor slowly or etc.
myISM.setAccelFilterLP2(false); 
// myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_20); // can keep 20, 45 (not 10) // (if you keep the filter then based on the filter -> update the offset as per that)




// final gyro settings
// --------------------------------------------------
myISM.setGyroDataRate(ISM_GY_ODR_104Hz); // data rate (best -> 104, 208, 416) (for fast use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12 )
myISM.setGyroFullScale(ISM_125dps); //always need to keep on even if scalling manually.
myISM.setGyroFilterLP1(false); // strictly off




// final fifo values :
// -------------------------------------------------------------------------------------------------------
myISM.setFifoMode(ISM_BYPASS_MODE); // default is bypass - Sensor always exposes ONLY the latest sample and fifo is off.  (ISM_FIFO_MODE) FIFO buffer fills until full, then STOPS writing new samples. (ISM_STREAM_MODE) for contineous updating old data circular buffer (old data overwritten by new data).
// below funcions use only if fifo is enabled (fifo_mode, stream_mode)
// myISM.setAccelFifoBatchSet(ISM_XL_BATCH_AT_104Hz); // keep same Hz as data rate or differnt as per how you want to store the readings
// myISM.setGyroFifoBatchSet(ISM_GY_BATCH_AT_104Hz) ; // 
// myISM.setFifoWatermark(n); //  Useful to know "FIFO has accumulated n samples" - itne samples hogaye hai. (can use in sf for debugging)
// myISM.setFifoTimestampDec(ISM_NO_DECIMATION) // Timestamp stored for every sample  // not usefull since we cannot access the timestamps.
// lets try FIFO_mode and Stream_mode in SF (maybe it can solve the laggy problem).....




// sensor hub seen , interrupt pin seen , offset seen ...







// full Proper code for ISM using timer loop . 
// ----------------------------------------------------------------------------------------------------------


#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM;
sfe_ism_raw_data_t rawAccelData;
sfe_ism_raw_data_t rawGyroData;


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
	Serial.println("Applying settings.");
	delay(100);
	
	
    // Apply device settings
    myISM.setDeviceConfig();
    myISM.setBlockDataUpdate(); 
	
	// Wire.setClock(400000); // uncomment it if using 416hz or higher data rate , other wise comment it.
	
	// Range -> always need to set, even if scaling maunally
	myISM.setAccelFullScale(ISM_2g); 
	myISM.setGyroFullScale(ISM_125dps); 
	// test: in Motor_code if motor moves faster than 2g or 125dps then the sensor values can break - so in that senario we can do 4g or 250dps, but that can lower the precision. other jugad we can do is -> move motor slowly or etc.
	// if you wanna update the range of acc or gyro then - not just you have to change the scaling factor , but again calib (as per that_range_lsb_data * that_range_scaling_factor) and get new offset.
	
	// DataRate -> 104Hz is good (fast & low noise) // can also try 208 or more in SF since it needs frequent values.
	myISM.setAccelDataRate(ISM_XL_ODR_104Hz); // data rate (best -> 104, 208, 416) (for faster use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12Hz5, 1Hz6 )
	myISM.setGyroDataRate(ISM_GY_ODR_104Hz); // data rate (best -> 104, 208, 416) (for fast use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12 )
	DataRate_HZ = 104; // keep same as sensor_Hz
    PERIOD_US = (1000000UL / DataRate_HZ); // using this we Contorl the iteration time.

	// Filter ->
	myISM.setAccelFilterLP2(false); // can keep it ON also, // later, TRY : ON in SensorFusion and see the results. 
    // myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_20); // can keep 20, 45 (not 10) // (if you keep the filter then based on the filter -> update the offset as per that)
	myISM.setGyroFilterLP1(false); // strictly off
	
	// fifo config (not much to do here, can try stream mode in SF, see 1_full_info.ino ka final setting section for more info)
	myISM.setFifoMode(ISM_BYPASS_MODE); // fifo off (default)
	
	
	............. sensor hub , interrup pin , offset , etc seen .....

	
	delay(100);
    Serial.println("Settings applied.");
}




void loop()
{
	now = micros();
	if (now - lastRead >= PERIOD_US)
	{
		lastRead = now;
		
		if (myISM.getRawGyro(&rawGyroData)&&myISM.getRawAccel(&rawAccelData))
		{
			Serial.print("Accelerometer: ");
			Serial.print("X: ");
			Serial.print(rawAccelData.xData);
			Serial.print(" ");
			Serial.print(rawAccelData.yData);
			Serial.print("Y: ");
			Serial.print(" ");
			Serial.print("Z: ");
			Serial.print(rawAccelData.zData);
			Serial.println(" ");
			Serial.print("Gyroscope: ");
			Serial.print("X: ");
			Serial.print(rawGyroData.xData);
			Serial.print(" ");
			Serial.print("Y: ");
			Serial.print(rawGyroData.yData);
			Serial.print(" ");
			Serial.print("Z: ");
			Serial.print(rawGyroData.zData);
			Serial.println(" ");
		}
		
	}
}


// // ----to access Sensor data in other fucntion----: 
// void fun()
// {
//	LastRead = 0; // reset before use
// 	while( condition -> N_samples / N_seconds / true / anyother condition.  )
// 	{
//      // Run loop as per Sensor Hz and not Run as fast as possible.
//      now = micros();
//      if (now - lastRead >= PERIOD_US)
//      {
//			lastRead = now;
//		    if (myISM.getRawGyro(&rawGyroData)&&myISM.getRawAccel(&rawAccelData))
//       	{
// 	  			//   get data / code ... 
//       	}		
//		 }
// 	} 
// 	lastRead = 0; // reset after use
// 	}
			
			
			
			
			
			















// full Proper code for ISM using Interrup pins . 
// ----------------------------------------------------------------------------------------------------------


#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM;
sfe_ism_raw_data_t rawAccelData;
sfe_ism_raw_data_t rawGyroData;

#define INT1_PIN ?
#define INT2_PIN ?


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
	
	// Range -> always need to set, even if scaling maunally
	myISM.setAccelFullScale(ISM_2g); 
	myISM.setGyroFullScale(ISM_125dps); 
	// test: in Motor_code if motor moves faster than 2g or 125dps then the sensor values can break - so in that senario we can do 4g or 250dps, but that can lower the precision. other jugad we can do is -> move motor slowly or etc.
	// if you wanna update the range of acc or gyro then - not just you have to change the scaling factor , but again calib (as per that_range_lsb_data * that_range_scaling_factor) and get new offset.
	
	// DataRate -> 104Hz is good (fast & low noise) // can also try 208 or more in SF since it needs frequent values.
	myISM.setAccelDataRate(ISM_XL_ODR_104Hz); // data rate (best -> 104, 208, 416) (for faster use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12Hz5, 1Hz6 )
	myISM.setGyroDataRate(ISM_GY_ODR_104Hz); // data rate (best -> 104, 208, 416) (for fast use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12 )

	// Filter ->
	myISM.setAccelFilterLP2(false); // can keep it ON also, // later, TRY : ON in SensorFusion and see the results. 
    // myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_20); // can keep 20, 45 (not 10) // (if you keep the filter then based on the filter -> update the offset as per that)
	myISM.setGyroFilterLP1(false); // strictly off
	
	// fifo config (not much to do here, can try stream mode in SF, see 1_full_info.ino ka final setting section for more info)
	myISM.setFifoMode(ISM_BYPASS_MODE); // fifo off (default)
	
	// Interrup Pin - for Accessing DATA Properly.
	myISM.setAccelStatustoInt1(true);   // ACC → INT1 // INT1 fires when new accelerometer data is ready
	myISM.setGyroStatustoInt2(true);    // GYRO → INT2 // INT2 fires when new gyroscope data is ready
	myISM.setDataReadyMode(0);   // 1 = pulsed, 0 = latched  
	myISM.setIntNotification(ISM_ALL_INT_LATCHED); // Latched -> Interrupt pin stays HIGH/LOW until the data is read // So we dont miss anydata
	myISM.setPinMode(false); //When data is ready → drive the pin "HIGH” and after data read it gives LOW until the data is updated

	----------------------------- offset , etc seen .....

	
	delay(100);
    Serial.println("Settings applied.");
}




void loop()
{
	
// check if both acc and gyro interrupt pins are high -> (new data is updated after last read)
if( (digitalRead(INT1_PIN) == HIGH) && (digitalRead(INT2_PIN) == HIGH) )
{	
	// get data
	if (myISM.getRawGyro(&rawGyroData)&&myISM.getRawAccel(&rawAccelData))
		{
			Serial.print("Accelerometer: ");
			Serial.print("X: ");
			Serial.print(rawAccelData.xData);
			Serial.print(" ");
			Serial.print(rawAccelData.yData);
			Serial.print("Y: ");
			Serial.print(" ");
			Serial.print("Z: ");
			Serial.print(rawAccelData.zData);
			Serial.println(" ");
			Serial.print("Gyroscope: ");
			Serial.print("X: ");
			Serial.print(rawGyroData.xData);
			Serial.print(" ");
			Serial.print("Y: ");
			Serial.print(rawGyroData.yData);
			Serial.print(" ");
			Serial.print("Z: ");
			Serial.print(rawGyroData.zData);
			Serial.println(" ");
		}
}

}














// ###################################################################################################
// update these code in all ACC , Gryo , final_calib 
// (Gothrough each code file, and read every line - ino/py both)
			
// which to use loop timer ? or interrup pin seen ?

// update the loop in every ardu.ino file ACC -> all main() loop and every other loop  where we read sensor data.
// only use the Hz timer loop technique where you are reading sensor data - should i also use it in in sf loop ? , etc.... ask gpt ??????????


// In 5_ardu_calib_acc code & 8_Auto_calib_acc code & 6_Auto_calib_gyro -> keeps all the above comments also. dont remove it. verna if you use that codefile (without comment wala) in another imp codefile -- then voo imp comment ka kuch fyda nahi 





















































