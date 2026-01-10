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

// so basically data is keep updating and the pin stay high until you read a data , and after you read - then it bcomes low -> unlit the data is updata again....


#define INT1_PIN 2
#define INT2_PIN 3

pinMode(INT1_PIN, INPUT_PULLUP);   // this adds a pull up resistor of arduino internally .......
pinMode(INT2_PIN, INPUT_PULLUP);

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








// ######################################################################################################

// 
//////////// gpt kar and understand this topic - ki setpinmode(ture/false)  - kya rakhna hia . i think low is better .....
// read  below chat of gpt. 



// okay just to complete my underastanding can you tell me how does the high , low stuff work. 
// what setPinMode to keep -> suppose if we keep pin mode false then it gives high when new data available - but i think high is bad - 
// since when not connected then also it gives high . so we should keep low ... since since when not connected it gives high , 
// which means new data is not avalable and so thus we should not read .....


// 1️⃣ First: separate TWO different “pin modes” (this is key)

// You are mixing two different things:

// A) myISM.setPinMode(...)

// ➡️ This configures the IMU’s INT pin driver

// B) pinMode(pin, ...)

// ➡️ This configures the Arduino’s input behavior

// They are NOT the same thing.

// 2️⃣ What myISM.setPinMode(true / false) actually means

// This controls how the IMU drives its INT pin.

// Value	Meaning
// true	Push-pull
// false	Open-drain
// Push-pull (recommended)

// IMU actively drives HIGH and LOW

// Strong signal

// Needs NO external resistor

// Open-drain

// IMU can only pull LOW

// Needs pull-up resistor

// Common in multi-device systems

// 👉 In your tests, you used:

// myISM.setPinMode(true); // push-pull


// This is correct.

// 3️⃣ What pinMode(pin, ...) on Arduino does

// This controls the Arduino pin, not the IMU.

// Arduino mode	Electrical behavior
// INPUT	Floating ❌
// INPUT_PULLUP	Weak pull-up (~20–50kΩ) ✅
// OUTPUT	Actively drives (dangerous here) ❌
// Why INPUT is bad

// Floating pin

// Random HIGH / LOW

// Reads HIGH even when disconnected

// That’s what confused you earlier.

// 4️⃣ HIGH vs LOW does NOT mean “good” or “bad”

// This is the big mental shift you need.

// HIGH and LOW have NO meaning by themselves.
// Meaning comes from your convention.

// 5️⃣ The CORRECT mental model (this clears everything)
// You must define:

// Idle state

// Event state

// Example (what you are using now):

// State	Arduino pin	Meaning
// Idle	HIGH	No new data
// Event	LOW	New data ready

// This is achieved by:

// pinMode(INT_PIN, INPUT_PULLUP);


// Now:

// Disconnected → HIGH (idle) → safe

// IMU event → pulls LOW → event

// After read → returns HIGH

// This is perfect design.

// 6️⃣ Why “HIGH is bad” is the wrong conclusion

// You said:

// “HIGH is bad because disconnected also gives HIGH”

// ❌ That’s not bad — that’s exactly what we want.

// Disconnected = no IMU = no data
// So:

// HIGH = do nothing


// LOW = only when IMU actively asserts interrupt.

// That is fail-safe behavior.

// 7️⃣ Why LOW is usually chosen as “event”

// Because:

// Pull-ups are easy

// Floating defaults to HIGH

// Noise tends to spike HIGH, not LOW

// LOW = intentional action

// That’s why:

// I²C

// SPI CS

// Interrupts
// often use active-LOW

// 8️⃣ What happens when new data arrives (step-by-step)

// With your setup:

// myISM.setPinMode(true);        // push-pull
// pinMode(INT_PIN, INPUT_PULLUP);

// Timeline:

// IMU idle → INT pin HIGH

// New sample ready → IMU drives pin LOW

// Arduino sees LOW → reads data

// Reading clears DRDY latch

// IMU releases pin → HIGH again

// Perfect.

// 9️⃣ Final answers to your exact doubts

// ❓ Should we keep HIGH or LOW?

// Doesn’t matter. YOU choose.
// But LOW = event is safer.

// ❓ Why not keep HIGH = event?

// You can — but then you need:

// Pull-down resistor

// Or reliable open-drain setup

// Which you don’t have.

// ❓ Why does disconnected give HIGH?

// Because pull-up defines idle state.
// That’s a feature, not a bug.

// 🔟 Final “rule of thumb” (remember this)

// Never read a floating pin.
// Always define idle state.
// LOW as event is safest.

// Your FINAL correct setup (lock this in)
// myISM.setPinMode(true);          // push-pull
// pinMode(INT_PIN, INPUT_PULLUP); // idle = HIGH
// if (digitalRead(INT_PIN) == LOW)
// {
//   // new data ready
// }













// update the below code - see gpt chat for updated code. ....


// also check if interrupt loop works good ? since at 104 ODR - it was giving 114 Hz par values.....   so gpt this ... and see upper gpt chat...

//  last final write whats better loop timer or interrup pin ?? 






/// ########################################################################################################
// ----------------------------------------------------------------------------------------------------------
// full Proper code for ISM using Interrupt pins . 


#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM;
sfe_ism_raw_data_t rawAccelData;
sfe_ism_raw_data_t rawGyroData;

#define INT1_PIN 2
#define INT2_PIN 3


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
	myISM.setAccelDataRate(ISM_XL_ODR_104Hz); // data rate (best -> 104, 208, 416) (for faster use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12Hz5, 1Hz6 )
	myISM.setGyroDataRate(ISM_GY_ODR_104Hz); // data rate (best -> 104, 208, 416) (for fast use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12 )
	DataRate_HZ = 104; // keep same as sensor_Hz
    PERIOD_US = (1000000UL / DataRate_HZ); // using this we Control the iteration time.


	// Range -> always need to set, even if scaling maunally
	myISM.setAccelFullScale(ISM_2g); 
	myISM.setGyroFullScale(ISM_125dps); 
	// test: in Motor_code if motor moves faster than 2g or 125dps then the sensor values can break - so in that senario we can do 4g or 250dps, but that can lower the precision. other jugad we can do is -> move motor slowly or etc.
	// if you wanna update the range of acc or gyro then - not just you have to change the scaling factor , but again calib (as per that_range_lsb_data * that_range_scaling_factor) and get new offset.
	

	// Filter ->
	myISM.setAccelFilterLP2(false); // can keep it ON also, // later, TRY : ON in SensorFusion and see the results. 
    // myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_20); // can keep 20, 45 (not 10) // (if you keep the filter then based on the filter -> update the hardcoded offset/bias as per that)
	myISM.setGyroFilterLP1(false); // strictly off
	

	// fifo config (not much to do here, can try stream mode in SF, see ISM/1_full_info.ino ka final setting section for more info)
	myISM.setFifoMode(ISM_BYPASS_MODE); // fifo off (default)
	
	//////// IMP....
    pinMode(INT1_PIN, INPUT_PULLUP);
    pinMode(INT2_PIN, INPUT_PULLUP);

	// Interrup Pin - for Accessing DATA Properly.
	myISM.setAccelStatustoInt1(true);   // ACC → INT1 // INT1 fires when new accelerometer data is ready
	myISM.setGyroStatustoInt2(true);    // GYRO → INT2 // INT2 fires when new gyroscope data is ready
	myISM.setDataReadyMode(0);   // 1 = pulsed, 0 = latched  
	myISM.setIntNotification(ISM_ALL_INT_LATCHED); // Latched -> Interrupt pin stays HIGH/LOW until the latest data is read // So we dont miss anydata
	myISM.setPinMode(false); //When data is ready → drive the pin "HIGH” and after data read it gives LOW until the data is updated

	
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









