// apply caliberation on live data - and see the xyz and Magnitude . 
// by seeing the magnitude you can judge if the offset are good or bad as per (magnitude close to 1).


////////////////////////////////////////////////////
// update this code ka setup() as per 1_basic_ino



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

    // Set accelerometer configuration
    myISM.setAccelDataRate(ISM_XL_ODR_208Hz); // data rate best ->  104, 208 fine. // 416 fast and good.
    myISM.setAccelFullScale(ISM_2g); 
    myISM.setAccelFilterLP2(false);                // Enable low-pass filter best -> 
    myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100); // Slope filter best -> 

    Serial.println("Settings applied.");
}

double calib[3] = {0.0, 0.0, 0.0};
double rawX;
double rawY;
double rawZ;
long lastRead = 0;
long now = 0;
int DataRate_HZ = 104; // keep same as sensor_Hz
long PERIOD_US = (1000000UL / DataRate_HZ);
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
        rawX = (rawAccelData.xData) * 0.00006103515625;
        rawY = (rawAccelData.yData) * 0.00006103515625;
        rawZ = (rawAccelData.zData) * 0.00006103515625;

        // Apply calibration: h_calib = A * (h_meas - b)

        // -------- SCALED -> CALIB --------
        // -------- Raw_g -> CALIB using offset/bias_g -> Calib_g  --------
        calib[0] = A_acc[0][0] * (rawX - b_acc[0]) + A_acc[0][1] * (rawY - b_acc[1]) + A_acc[0][2] * (rawZ - b_acc[2]);
        calib[1] = A_acc[1][0] * (rawX - b_acc[0]) + A_acc[1][1] * (rawY - b_acc[1]) + A_acc[1][2] * (rawZ - b_acc[2]);
        calib[2] = A_acc[2][0] * (rawX - b_acc[0]) + A_acc[2][1] * (rawY - b_acc[1]) + A_acc[2][2] * (rawZ - b_acc[2]);


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










