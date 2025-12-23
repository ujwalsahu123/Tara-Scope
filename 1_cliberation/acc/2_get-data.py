# this code Reads the output data (lsb data) from the arduino and then scales it to g and stores it into txt file .
# in a single reading - it takes 25 data points and then average them and then scale the averaged value and stores it.
# scaling is important since - sf wants acc in g . so (LSB * 0.00006103515625 = g)  dont do (lsb * 0.061035... = mg)

# place the sensor at a still orientation - then read . ... again and again at differnt orientations.

# then use magneto to get offset then paste it and see the 3_plot , 4_magnitude , 5_live_Magnitude



import os
import math
import pandas as pd
import serial

# Configuration
MAX_MEAS = 2000  # Maximum number of measurements // max it takes 2000 reads, but we can increase the number . and stop the read whenever we want.
AVG_MEAS = 25   # Number of samples to average for one reading // basically when collecting one data point - we dont just store 1 value , we actually take 25 readings and average them to get 1 data point.
SER_PORT = 'COM6'  # Update to your Arduino's COM port (e.g., 'COM4' on Windows, '/dev/ttyUSB0' on Linux)
SER_BAUD = 115200  # Baud rate for serial communication
FILENAME = os.path.join(os.getcwd(), 'acc/acceldata_ism.txt')  # File to save accelerometer data

# SerialPort Class to manage Arduino communication
class SerialPort:
    def __init__(self, port, baud):
        if not isinstance(port, str):
            raise TypeError('Port must be a string.')
        if not isinstance(baud, int):
            raise TypeError('Baud rate must be an integer.')

        self.port = port
        self.baud = baud
        self.ser = serial.Serial(self.port, self.baud, timeout=None)
        self.ser.flushInput()
        self.ser.flushOutput()

    def read(self, clean_end=True) -> str:
        """Read data from the serial port."""
        self.ser.flushInput()
        try:
            bytes_to_read = self.ser.readline()
            decoded_msg = bytes_to_read.decode('utf-8')
            if clean_end:
                decoded_msg = decoded_msg.rstrip()  # Remove newline characters
            return decoded_msg
        except Exception as e:
            print(f"[ERROR]: Serial read error: {e}")
            return ""

    def close(self):
        """Close the serial port."""
        self.ser.close()

# Function to record a single data point (averaged over AVG_MEAS readings)
def record_data_point(ser: SerialPort) -> tuple:    
    ax, ay, az = 0.0, 0.0, 0.0

##### i think its better to do average of lsb and then convert to g or mg using scale factor (0.061 , etc)
##### and not really first converting to mg or g and then averaging. 
#### since its the same thing - averaging first and then scaling or scaling first and then averaging - both are same mathematically.
#### ex - 1+2+3 /3  = 2 * (10) = 20 . and 1*10 + 2*10 +3*10 /3 = 20 . both are same.
#### but averaging first and then scaling is better since - we only scale 1 time - so faster.

    for _ in range(AVG_MEAS):
        try:
            data = ser.read().split(',')
            ax_now = (int(data[0])) # * 0.00006103515625 # we will not scale here - we will scale after averaging
            ay_now = (int(data[1])) # * 0.00006103515625
            az_now = (int(data[2])) # * 0.00006103515625
        except (ValueError, IndexError) as e:
            print(f"[ERROR]: Data parsing error: {e}")
            ser.close()
            raise SystemExit("[ERROR]: Exiting due to invalid serial data.")
        ax += ax_now
        ay += ay_now
        az += az_now

    # after loop ends
    ax = (ax / AVG_MEAS) * 0.00006103515625  # average out, and Convert to g
    ay = (ay / AVG_MEAS) * 0.00006103515625 
    az = (az / AVG_MEAS) * 0.00006103515625  
    return ax , ay , az 

# Function to save the data to a file
def save_data_to_file(data: list, filename: str, delimiter: str = '\t'):
    # Check if file exists and is non-empty
    file_exists = os.path.exists(filename) and os.path.getsize(filename) > 0
    
    # Convert the data to a DataFrame
    df = pd.DataFrame(data)
    
    # Append data to file
    with open(filename, 'a') as file:
        df.to_csv(file, sep=delimiter, index=False, header=not file_exists)  # Write header only if file is empty

    print(f"[INFO]: Data appended to {filename}")

# Main function
def main():
    ser = SerialPort(SER_PORT, baud=SER_BAUD)
    data = []  # To store accelerometer readings

    print('[INFO]: Place the sensor level and stationary on a flat surface.')
    input('[INPUT]: Press Enter to begin calibration...')

    for _ in range(MAX_MEAS):
        user_input = input("[INPUT]: Ready for measurement? Type 'm' to measure or 'q' to quit: ").lower()
        if user_input == 'm':
            ax, ay, az = record_data_point(ser)

            # Avg values will be floats, preserving high precision in the file
            print(f'[INFO]: Avg Readings -> ax: {ax}, ay: {ay}, az: {az}')
            data.append([ax, ay, az])  # Store the readings
        elif user_input == 'q':
            print('[INFO]: Saving data and exiting...')
            save_data_to_file(data, FILENAME)
            ser.close()
            print('[INFO]: Done!')
            return
        else:
            print(f"[ERROR]: Unknown input '{user_input}'. Saving data and exiting!")
            save_data_to_file(data, FILENAME)
            ser.close()
            return

    print("[WARNING]: Maximum number of measurements reached. Saving data...")
    save_data_to_file(data, FILENAME)
    ser.close()
    print('[INFO]: Done!')

# Run the program
if __name__ == '__main__':
    main()
