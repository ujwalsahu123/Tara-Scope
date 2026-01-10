
// see sparkfun hookup guide to know more about the chip back connection .


// last -> int2 pin is used for sensor hub data sync ... so know more about it

// final write -> should i use sensor hub or not ...

// ######################################################### Sensor Hub #########################################################
// The sensor hub lets the ISM330DHCX act as an I²C master and automatically read external sensors without the MCU talkin
// So insted of multiple I2C devices connected directly to MCU -> they are connected to the ISM and then ISM reads their data and stores it in registers 
// and then the MCU can use those registers to read that data . 
// benefit -> sync 
// So the Sensor_Hub -> Control external sensors , Periodically read external sensors (at a set ODR) thus , Store external data inside ISM330DHCX , Reduce MCU workload.
// the ISM sensor reads all the data -> gyro , acc , external sensor and store them at a timestamp (ex- 1:20:00 acc = ... , gyro = ... , external sensor) and thus all the data is in sync.

// So the sensor has 2 chips -> ISM and MMC and both are connected to the I2C bus. 
// so we can communicate with ism or mmc directly using the MCU (MCU->ISM , MCU->MMC)  // dont think we need to do alag alag wriring, since internally both are connected to the i2c bus. so when we read both the sensors data normally (not using sensor hub) in a single code file - then we are actually readings data from 2 alag alag sensor*** (ism , mmc) but since interanlly wiring hua hai , thus no need to do alag say wirring. (both chip are on same board but MMC is still an external sensor)
// or use ISM as a I2C master to communicate with the MMC sensor. (MCU->ISM->MMC)

// There is a Seen of MMC sensor -> “The magnetometer requires an initiate measurement bit to be flipped before every reading" 
// 1) so when we use MCU->MMC directly using the MMC library then we dont need to seperately flip the bit -> and it happens internally 
// 2) but when we use MCU->ISM->MMC using sensor hub then we need to manually filp the bit ever time before every reading.

// so when we do 1) MCU->ISM , MCU->MMC  then we will get synced acc , gyro value . but since MMC is used as external sensor - thus its not synced (also randomly/non synced bit flips) . but we dont have to manaully flip the bit 
// and when we do 2) MCU->ISM->MMC then we flip the bit maunally and for doing so - we need to need to turn off the ISM and sensor_hub then flip the bit then again turn on sensor hub and ISM after every read. - so timming can jitter, and clunky (since when turning the ISM on / off it needs 300us micro sec to actually turn it on off so need to put a delay of 300 micro seconds). but now we get synced data.
//// process of 2) -> 
// // Disable accel/gyro
// Disable sensor_hub
// wait 300us microsec to actually disable
// Write control bit
// Re-enable accel/gyro
// Re-enable hub
// wait 300us microsec to actually Re-enable
// Read MMC data
// Repeat
//// (in the code examples it does the disable/Re-enbale/flip_bit part in a function writeControlBit() and in the void loop it just reads the data.)

// so sensor_hub is best for free running sensors they just updates the data directly (no need to do bit flipping) . MMC5983 is not a free-running sensor.
// Thats why in code example it says -> "You might notice that we have a 9DoF with these two parts but not in this configuration. The reason is that the magnetometer requires
// an initiate measurement bit to be flipped before every reading, while this is possible (it's demonstrated below) it's also not ideal for this setup. A more
// ideal setup would be a sensor that is just turned on and data is pulled periodically."

// Sensor hub is designed for sensors that:
// Run continuously
// Update data registers automatically
// Don’t need per-sample command writes
// So the example says “not ideal” because:
// Sensor hub likes passive reads
// MMC needs active command → wait → read each time
// ⚠️ Not because it doesn’t work
// ⚠️ But because it breaks the “fire-and-forget” model

// 1)
// ✅ Pros:
// Simple
// MMC library handles bit flipping
// No enable/disable gymnastics
// Lowest code complexity
// ❌ Cons:
// Accel/Gyro and Mag are not hardware-synchronized
// Timing jitter depends on loop execution
// 🟢 Reality check:
// At 104 Hz, with a deterministic loop, sync error is tiny (<<1 ms).

// 2) 
// ✅ Pros:
// Accel + Gyro + Mag sampled at same ODR domain
// Best theoretical sync
// Cleaner data for offline logging
// ❌ Cons:
// Must:
// Disable accel/gyro
// Disable hub
// Write MMC control bit
// Re-enable hub
// Re-enable accel/gyro
// Wait ~300 µs twice
// Clunky
// Not what MMC was designed for
// You are 100% right that:
// 300 µs is small compared to 104 Hz (≈9600 µs)
// Yes — timing impact is small
// But state toggling adds complexity + risk

// 🔹 Option 1: MCU → ISM + MCU → MMC (NO sensor hub)
// Pros
// Simple
// MMC library handles bit-flip internally
// No enable/disable gymnastics
// Cons
// ❌ No true sync
// Acc/Gyro timestamp ≠ Mag timestamp
// Magnetometer latency varies
// Option 2: MCU → ISM → MMC (Sensor Hub)
// Pros
// ✅ Best synchronization possible
// Acc + Gyro + Mag aligned
// Single timing domain
// Cons
// Extra steps per sample:
// Disable ISM
// Flip bit
// Re-enable
// ~600 µs overhead



// Option A — Sensor Hub (❌ not ideal for YOU)
// Pros
// Slightly better timestamp alignment
// One bus read
// Cons (big ones)
// MMC trigger requirement breaks timing
// More complex cod
// Not real-time friendly
// Worse for fusion stability
// 👉 Not recommended for MMC5983MA + real-time SF
// Does sensor hub improve sync? (important clarification)
// YES — but only when the external sensor is free-running



// so what to do ? 1) or 2)  ?????????????????????????????????????????????//

// if we do 1) MCU->ISM , MCU->MMC then we can use a time controlled loop as per datarate so that it reads data at almost the same time maybe. 
// or if we do 2) MCU->ISM->MMC then we can turn off sensor_hub & ISM then flip the bit and turn the sensor_hub , ISM again , 
// and check if it actually is laggy or clunky ? or not . (example says not ideal, but need to test) (300 microsec is suepr small so maybe it will not put too delay)

// so the main problem in 1) is the sync and we can solve it by using time controlled loop .
// and main problem in 2) is the delay requied for (disable, eable, flip) which is 310 * 2 microsecond - which is very small so maybe it dosent effect or delays our 104hz loop // need to test.


// also i think sensor hub is nice , but for our mmc which needs to flip bit and enable / re-enable the sensor hub is not good , 
// but i am not sure since the delay is like not more than 1000 microsecond which is very small if we do 104 hz - so may be we dont even see the jitter / delay - 
// but i have not tested it yet so i dont know ... if its works then it may be better than - using contorolled loop (since in controlled loop the mmc does bit write / wait / read) ...


////////// internally also i think it does flipping bit in the library..... thats why it feels clunky in the serial monitor when it prints the mag xyz values
// and i think it has to flip bit after every all 3 xyz read not like x read then bit file , then y read then bit filp ... NO.


// idk why but when we do 1) then also the mag y,z values jitter..... (it may be because the library doestn do bit flip after every xyz read then bit flip. it randomly or contineously does bit flip or after every x or y or z axis read.)
// need to look out the mmc library to know more aobut this part (how does the library does bit flip -> randomly/contineously , after all xyz read , after sing x or y or z read...)



// The MMC5983MA is NOT a free-running magnetometer.
// So we cannot remove the flipping bit stuff, since its the hardware seen not just library seen.
// ✅ You can only decide who flips the bit and when -> in 1) the library flips the bit without delay or  2) we filp the bit manually using sensor hub with dealy
// so first lets check ki both ways take the same time ... if library wala also does bit flipping and takes 600us ( i dont think it takes 600us since the dealy is for sensor hub on/off , but if we connect I2C seperatly to MMC then the library does simply bit flip without any sensnor_hub seen or dealy(since dealy is for sensor_hub on/off))
// then why not to do it manually using sensor hub... in this case we also get sync data....

// so - i think the tradeoff is sync vs dealy. (1 faster since no dealy but not sync. 2 is slower since it has dealy but has sync)
// so need to check if actually sync hai and better then 1) synce hai 2) ka. ..
// and also need to check if - 2) is more dealy than  1) since the dealy is very small. 

// and if 2) is better then we can also lower the dealy seen since 
//  104 Hz → 9.6 ms per sample
// Sensor hub overhead → ~0.6 ms
// idea -> so is there a way that jabtak loop is waiting for next iteration as per the timer - tab tak we flip the bit..... 



// need to see actually 2) sync hai yaa nahi // actually 2) is slower or same speed
// MMC library bit flip need to see. 
// if 2) is more sync then we go with it. if 2) is more sync but slower then we try to make it faster using loop timer technique (up). 
// but still it is slow then we go with 1) and compromise sync. 
// but we also need to see if MMC also does dealy or what ... and how much aysync it is (since values are jitter- which means library is contineouly/randomly/etc  flipping the bit)
// ..........



// so our assumption is that -> tradeoff is time vs sync . 
// and we assume that library does bit flip wihout any dealy (but we need to check the library to confirm) ,
//  and sensor_hub needs dealy 600us per iteraration but does that make jitter and chunky at 104,208,416 hz  ? (need to also check that.)
// and we assume that sensor_hub is more sync (since we manually filp the bit and not randomly) (but we need to check if really in sync)  ,
// and the library is not sync (since it randomly/contineourly does bit filp) (but we need to check how does it deal with bit flip and is it really a-sync with acc/gyro)
// .....
// idea -> so is there a function in the MMC library using which we can flip the bit when we want ?? so that randomly nahi hoga bit flip 
// so that we get fastness also and sync also... and no need to do sensor_hub...


// need to also study -> interrupt pin int2 if we do sensor_hub. since it tells when is the external sensor data is ready.




 








// gpt is telling dont do sf right now , do it later :
// ✅ Move magnetometer to sensor hub
// Enable hub FIFO batching
// Feed FIFO data directly into SF
// Enjoy cleaner fusion and less drift
// gpt this what does it mean ? if sensor_hub takes so much time 



// -------------------------------------------------------------------------------
////// Without sensor hub:---
// Accel, gyro, mag sampled at slightly different times
// Fusion sees timing noise
// More drift, jitter
////// With sensor hub:---
// Accel + gyro + mag sampled on same clock
// Same ODR
// Same FIFO
// Same timestamps
// Much better attitude estimation
////// Use sensor hub if:
// ✅ You want tight sync between accel, gyro, mag
// ✅ You want clean FIFO-based SF
// ✅ You want less MCU load
////// Avoid sensor hub if:
// ❌ Magnetometer needs complex per-read commands **********
// ❌ You’re okay with MCU polling mag separately
// ❌ You want simpler code during early prototyping
////The sensor hub works best when:
// External sensor runs continuously
// ISM just reads registers periodically
// No “write before every read” needed








// ----------------------------------------------------------------------------------------------------------------
// Sensor Hub Settings
		bool setHubODR(uint8_t rate); // How often the hub reads external sensors. keep same as the ISM sensor acc, gyro ODR rate.
		bool setHubSensorRead(uint8_t sensor, sfe_hub_sensor_settings_t* settings); // Read sensor #index using this I²C address, register, length
		bool setHubSensorWrite(sfe_hub_sensor_settings_t* settings); // One-time write to external sensor (e.g., start measurement)
		bool setNumberHubSensors(uint8_t numSensors);  // How many external sensors (0–3) the hub will read
		bool enableSensorI2C(bool enable); // Turns sensor hub I²C master ON/OFF . ⚠️ Hub configuration must be done while this is OFF
		bool readPeripheralSensor(uint8_t* shReg, uint8_t len); // Copies external sensor data from ISM internal registers
		bool setHubWriteMode(uint8_t config); //
		bool readMMCMagnetometer(uint8_t* magData, uint8_t len); //
		bool setHubPassThrough(bool enable = true); //
		bool setHubFifoBatching(bool enable = true); // External sensor data also goes into FIFO //////// maybe good for SF
		bool setHubPullUps(bool enable = true); //
		bool getHubStatus(); // Tells you: “External sensor read finished”
		bool getExternalSensorNack(uint8_t sensor); // Debug: external sensor didn’t respond
		bool resetSensorHub(); //
		
	
// later try in SF -> use fifo ... ??? ->	bool setHubFifoBatching(bool enable = true); // External sensor data also goes into FIFO //////// maybe good for SF
		

#define ISM_SH_ODR_104Hz 0x00
#define ISM_SH_ODR_52Hz  0x01
#define ISM_SH_ODR_26Hz  0x02
#define ISM_SH_ODR_13Hz  0x03












#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"
#include <SparkFun_MMC5983MA_Arduino_Library.h>

SparkFun_ISM330DHCX myISM;
  SFE_MMC5983MA myMag;
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

  if (!myMag.begin()) {
    Serial.println("Error initializing MMC5983MA sensor. Check connections.");
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
	
	

	
	delay(100);
    Serial.println("Settings applied.");
}




void loop()
{
	now = micros();
	if (now - lastRead >= PERIOD_US)
	{
		lastRead = now;
		
		if (myISM.getRawGyro(&rawGyroData) && myISM.getRawAccel(&rawAccelData))
		{

			Serial.print("     ACC: ");
			Serial.print("X: ");
			Serial.print(rawAccelData.xData);
			Serial.print(" ");
			Serial.print(rawAccelData.yData);
			Serial.print("Y: ");
			Serial.print(" ");
			Serial.print("Z: ");
			Serial.print(rawAccelData.zData);
			Serial.print(" ");

			Serial.print("     GYRO: ");
			Serial.print("X: ");
			Serial.print(rawGyroData.xData);
			Serial.print(" ");
			Serial.print("Y: ");
			Serial.print(rawGyroData.yData);
			Serial.print(" ");
			Serial.print("Z: ");
			Serial.print(rawGyroData.zData);
			Serial.print(" ");

			Serial.print("    MAG: ");
      Serial.print(myMag.getMeasurementX()); Serial.print(",");
      Serial.print(myMag.getMeasurementY()); Serial.print(",");
      Serial.println(myMag.getMeasurementZ()); Serial.print(",");
		}
		
	}
}