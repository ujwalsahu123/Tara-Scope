// apply caliberation on live data - and see the xyz and Magnitude . 
// by seeing the magnitude you can judge if the offset are good or bad as per (magnitude close to 1).


////////////////////////////////////////////////////
// update this code ka setup() as per 1_basic_ino



#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM;

// Define the structure for raw accelerometer data
sfe_ism_raw_data_t rawAccelData;



// Define calibration parameters
const double A[3][3] = { // Calibration matrix
    {0.996986, 0.000024, -0.000549},
    {0.000024, 1.001778, 0.000595},
    {-0.000549, 0.000595, 1.003680}
};

const double b[3] = { // Bias vector
    0.005380, -0.014230, 0.009798
};

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

void loop() {
    // Get raw accelerometer data
    if (myISM.getRawAccel(&rawAccelData)) {
        // Convert raw LSB values to g, using *0.00006103515625. 
        // (we are not doing 0.061 since that's mg not g. and the sf wants g . dont do 0.000061, since that has binary representation noise)

        double rawX = (rawAccelData.xData) * 0.00006103515625;
        double rawY = (rawAccelData.yData) * 0.00006103515625;
        double rawZ = (rawAccelData.zData) * 0.00006103515625;

        // Apply calibration: h_calib = A * (h_meas - b)
        double meas[3] = {rawX, rawY, rawZ};
        double calib[3] = {0.0, 0.0, 0.0};

        // Subtract bias and apply matrix multiplication
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                calib[i] += A[i][j] * (meas[j] - b[j]);
            }
        }

        // Calculate magnitude of the calibrated vector
        // at static position the magnitude should be same as g value

        double magnitude = sqrt(calib[0] * calib[0] + calib[1] * calib[1] + calib[2] * calib[2]);  // no need to do in sf.

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

