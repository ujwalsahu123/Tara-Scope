# here it just simply reads the arduino averaged values and then simply scale it to g . and store it. 
# 5 orientations is enough ( | straight , 45 deg / , -45 deg \ , 90 deg -- ,  sideways up )
# no need to change orientation by self . since we do it using 2 motors (best).
# then use magneto / our code.file to get offset then check the 3_plot , 4_magnitude , 5_live magnitude
#

# close the data.txt tab - then read . verna it dosent store the values . 
# also be inside the current folder in termainl and then run the code 
# cd /get_data_using_motor then python code_python.py
# dont do python /get_data_using_motor/python.py
# can see the plot and magnitude using the plot and magnitude codefile.

# press "s" to start and it will move 360degress and then you can change the orientation ...and again press "s" to capture another 360 degree of differnt orientation.
# it will store the data in a .txt file
# press "q" after done - plot the data. (you can also use the 3_calib_plot to plot the data.txt file)

import serial
import time
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# ---------------- CONFIG ----------------
PORT = 'COM7'          # change if needed
BAUD = 115200
OUTFILE = 'data_g_2g_45filter.txt'
SCALE = 0.00006103515625  # LSB → g (2g range)
# ----------------------------------------

ser = serial.Serial(PORT, BAUD, timeout=1)
time.sleep(2)

data = []  # store [x,y,z] in g

print("[INFO] Press 's' to start collection")
print("[INFO] Press 'q' to stop & plot")

def plot_data(arr):
    data = np.array(arr)

    if len(data) < 2:
        print("Not enough data to plot")
        exit()

    x, y, z = data[:,0], data[:,1], data[:,2]

    x0, y0, z0 = data[0]
    xN, yN, zN = data[-1]

    plt.figure(figsize=(14, 10))

    # -------- 3D PLOT --------
    ax = plt.subplot(221, projection='3d')
    ax.scatter(x, y, z, s=8, alpha=0.5, label="Samples")
    ax.scatter(x0, y0, z0, c='green', s=80, label="First")
    ax.scatter(xN, yN, zN, c='red', s=80, label="Last")
    ax.set_title("3D Accelerometer Data")
    ax.set_xlabel("X (g)")
    ax.set_ylabel("Y (g)")
    ax.set_zlabel("Z (g)")
    ax.legend()

    # -------- XY --------
    plt.subplot(222)
    plt.scatter(x, y, s=8, alpha=0.5)
    plt.scatter(x0, y0, c='green', s=80)
    plt.scatter(xN, yN, c='red', s=80)
    plt.xlabel("X (g)")
    plt.ylabel("Y (g)")
    plt.title("XY Plane")
    plt.grid(True)

    # -------- XZ --------
    plt.subplot(223)
    plt.scatter(x, z, s=8, alpha=0.5)
    plt.scatter(x0, z0, c='green', s=80)
    plt.scatter(xN, zN, c='red', s=80)
    plt.xlabel("X (g)")
    plt.ylabel("Z (g)")
    plt.title("XZ Plane")
    plt.grid(True)

    # -------- YZ --------
    plt.subplot(224)
    plt.scatter(y, z, s=8, alpha=0.5)
    plt.scatter(y0, z0, c='green', s=80)
    plt.scatter(yN, zN, c='red', s=80)
    plt.xlabel("Y (g)")
    plt.ylabel("Z (g)")
    plt.title("YZ Plane")
    plt.grid(True)

    plt.tight_layout()
    plt.show()


# ---------------- MAIN LOOP ----------------
while True:
    cmd = input(">> ").strip().lower()

    if cmd == 's':
        ser.write(b's')
        print("[INFO] Collecting data...")

        with open(OUTFILE, 'a') as f:
            f.write("")  # clear file

        while True:
            line = ser.readline().decode().strip()

            if not line:
                continue

            if line == "START":
                continue

            if line == "DONE":
                print("[INFO] Collection finished")
                break

            try:
                ax, ay, az = map(int, line.split(','))
            except:
                continue

            gx = ax * SCALE
            gy = ay * SCALE
            gz = az * SCALE

            data.append([gx, gy, gz])
            
            # store data in a .txt file.
            with open(OUTFILE, 'a') as f:
                f.write(f"{gx}\t{gy}\t{gz}\n")

            print(f"{gx:.6f}\t{gy:.6f}\t{gz:.6f}")

    elif cmd == 'q':
        ser.write(b'q')
        print("[INFO] Quitting & plotting...")

        ser.close()

        if len(data) > 1:
            plot_data(data)
        else:
            print("[WARN] Not enough data to plot")

        break



