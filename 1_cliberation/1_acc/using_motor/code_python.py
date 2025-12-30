# here it just simply reads the arduino averaged values and then simply scale it to g . and store it. 
# 5 orientations is enough ( | straight , 45 deg / , -45 deg \ , 90 deg -- ,  sideways up )
# then use magneto to get offset then check the 3_plot , 4_magnitude , 5_live magnitude

# close the data.txt tab - then read . verna it dosent store the values . 
# also be inside the current folder in termainl and then run the code 
# cd /get_data_using_motor then python code_python.py
# dont do python /get_data_using_motor/python.py
# can see the plot and magnitude using the plot and magnitude codefile.


import serial
import time
import os

PORT = 'COM7'          # change if needed
BAUD = 115200
OUTFILE = 'Data.txt'
SCALE = 0.00006103515625  # 2g_LSB → 2g_G

ser = serial.Serial(PORT, BAUD, timeout=1)
time.sleep(2)

def save_line(line):
    with open(OUTFILE, 'a') as f:
        f.write(line + '\n')

print("[INFO] Press s to start, q to quit")

while True:
    cmd = input(">> ").strip().lower()

    if cmd == 's':
        ser.write(b's')
        print("[INFO] Started")

        while True:
            line = ser.readline().decode().strip()

            if not line:
                continue

            if line == "DONE":
                print("[INFO] Cycle done")
                break

            if line == "START":
                continue

            # Expect: ax,ay,az (RAW LSB)
            try:
                ax, ay, az = map(int, line.split(','))
            except:
                continue

            gx = ax * SCALE
            gy = ay * SCALE
            gz = az * SCALE

            out = f"{gx}\t{gy}\t{gz}"
            save_line(out)

            print(out)

    elif cmd == 'q':
        ser.write(b'q')
        print("[INFO] Quit")
        break

ser.close()
