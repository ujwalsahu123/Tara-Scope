import serial
import time
import os

PORT = 'COM7'          # change if needed
BAUD = 115200
OUTFILE = 'motor_read.txt'
SCALE = 0.00006103515625  # LSB → g

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
