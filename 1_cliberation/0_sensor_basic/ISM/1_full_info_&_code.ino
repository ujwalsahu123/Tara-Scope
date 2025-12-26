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
// sfe_ism330dhcx.h (has all the function list -> setting functions , getting data wale functions , output data ka datatype and struct etc)
// sfe_ism330dhcx.cpp (has all the codes for the functions - ex get_raw_accel(), etc ) (high level code - function is calling the low level function of ism330dhcx_reg.c)
// ism330dhcx_reg.c (has low level read/write and main code - ex functions that read buffer and lsb values , also convert_Lsb_to_2g() , convert_Lsb_to_250dps() , etc )
// ism330dhcx_reg.h



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


myISM.setAccelFilterLP2(false); 
//.setAccelSlopeFilter(ISM_LP_ODR_DIV_20); // can keep 10, 20. but best is to keep it off 
// since internally in the library it does calib, etc the value of gyro can be devived from acc values (which if you keep filter on) then it can effect gyro non filter values also. but i am not sure about this fact.
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




// ######################################################### Inerrupt Pin #########################################################


// Interrupt Settings
		bool setAccelStatustoInt1(bool enable = true);
		bool setAccelStatustoInt2(bool enable = true);
		bool setGyroStatustoInt1(bool enable = true);
		bool setGyroStatustoInt2(bool enable = true);
		bool setIntNotification(uint8_t val);
		bool setDataReadyMode(uint8_t val);
		bool setPinMode(bool activeLow = true);

/Interrupt pin notification settings.
#define ISM_ALL_INT_PULSED            0x00
#define ISM_BASE_LATCHED_EMB_PULSED   0x01
#define ISM_BASE_PULSED_EMB_LATCHED   0x02
#define ISM_ALL_INT_LATCHED           0x03

#define ISM_SH_ODR_104Hz 0x00
#define ISM_SH_ODR_52Hz  0x01
#define ISM_SH_ODR_26Hz  0x02
#define ISM_SH_ODR_13Hz  0x03

// ######################################################### Sensor Hub #########################################################

 code example see 
 libarary functions see

// Sensor Hub Settings
		bool setHubODR(uint8_t rate);
		bool setHubSensorRead(uint8_t sensor, sfe_hub_sensor_settings_t* settings);
		bool setHubSensorWrite(sfe_hub_sensor_settings_t* settings);
		bool setNumberHubSensors(uint8_t numSensors);
		bool enableSensorI2C(bool enable);
		bool readPeripheralSensor(uint8_t* shReg, uint8_t len);
		bool setHubWriteMode(uint8_t config);
		bool readMMCMagnetometer(uint8_t* magData, uint8_t len);
		bool setHubPassThrough(bool enable = true);
		bool setHubFifoBatching(bool enable = true);
		bool setHubPullUps(bool enable = true);
		bool getHubStatus();
		bool getExternalSensorNack(uint8_t sensor);
		bool resetSensorHub();





// // ######################################################### offset seen #########################################################















// ##################################################################################################################################### 
// ######################################################### Final Setting ISM ######################################################### 



// final acc settings
// --------------------------------------------------
// Wire.setClock(400000); // uncomment it if using 416hz or higher data rate , other wise comment it.
myISM.setAccelDataRate(ISM_XL_ODR_208Hz); // data rate (best -> 104, 208, 416) (for faster use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12Hz5, 1Hz6 )
myISM.setAccelFullScale(ISM_2g); // always need to set range , even if scaling manually.
// test: in Motor_code if motor moves faster than 2g then the sensor values can break - so in that senario we can do 4g or 8g or 250dps , etc  , but that can lower the precision. other jugad we can do is -> move motor slowly or etc.
myISM.setAccelFilterLP2(false); 
// myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_20); // can keep 10, 20. but best is to keep it off 




// final gyro settings
// --------------------------------------------------
myISM.setGyroDataRate(ISM_GY_ODR_208Hz); // data rate (best -> 104, 208, 416) (for fast use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12 )
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







// setup() code for ISM
// ----------------------------------------------------------------------------------------------------------

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
	// test: in Motor_code if motor moves faster than 2g or 125dps then the sensor values can break - so in that senario we can do 4g or 250dps, but that can lower the precision. other jugad we can do is -> move motor slowly or etc.
	myISM.setAccelFullScale(ISM_2g); 
	myISM.setGyroFullScale(ISM_125dps); 
	// if you wanna update the range of acc or gyro then - not just you have to change the scaling factor , but again calib (as per that_range_lsb_data * that_range_scaling_factor) and get new offset.

	// DataRate -> 
	myISM.setAccelDataRate(ISM_XL_ODR_208Hz); // data rate (best -> 104, 208, 416) (for faster use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12Hz5, 1Hz6 )
	myISM.setGyroDataRate(ISM_GY_ODR_208Hz); // data rate (best -> 104, 208, 416) (for fast use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12 )
	
	// Filter ->
	myISM.setAccelFilterLP2(false); 
	// myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_20); // can keep 10, 20. but best is to keep it off 
	myISM.setGyroFilterLP1(false); // strictly off
	
	// fifo config (not much to do here, can try stream mode in SF, see 1_full_info.ino ka final setting section for more info)
	myISM.setFifoMode(ISM_BYPASS_MODE); // fifo off (default)
	
	
	.............
	
	
	
	delay(100);
    Serial.println("Settings applied.");
}














// ###################################################################################################
// update these code in all ACC , Gryo ,  SF code files (Gothrough each code file, and read every line - ino/py both)





// in sf ->   // Note: we can't do prints inside the while loop. They slow things down too much... so print every 10th loop . 


// tune beta / gain in SF (this matters more than filters)

// verify ODR vs SF update rate

// check why some axes show higher noise


// /// for sf the most important thing is responsiveness . and any kind of delay make things bad. 
// ex - you steer the car ,but tire takes 5 second to actually move. this was happening -> when it took 1 min by sf to come to read posi.
// and if it tries to correct it then it ossicalates ->   0-> 100 then comes to 25 then 75 ... till it comes 50. 
// So remove all things that does dealy or lowers responsiveness before giving the sensors data to sf  -> filter , printing linges every loop, distorting delays, non distorting delays ..... find out.... what this video https://www.mathworks.com/videos/control-systems-in-practice-part-4-why-time-delay-matters-1536913253300.html




// gyro may joo ARW and Bias_instability calculate kiya that can be use in sf , kalman filter...