# get the raw gryo data using serial monitor 
# stores data {timestamp millie sec, x, y ,z} in raw.txt file
# it dosent do scaling (lsb -> dps) here. 
# we use the scaling code file to scale lsb_txt file to dps and do analysis /calib on that scaled data.



import serial
import time
import sys

# ---------------- CONFIG ----------------
SERIAL_PORT = 'COM7'        # CHANGE THIS CAREFULLY
BAUD_RATE = 115200
OUT_FILE = 'Analysis_big_data/raw.txt'

DURATION_SEC = 21600        # None = run until stopped  # 60 (60 sec) # 21600 (6hrs)
FLUSH_EVERY_N = 100
# ----------------------------------------


def main():

    # ---- Try opening serial port ----
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    except serial.SerialException as e:
        print(f"[ERROR] Could not open serial port {SERIAL_PORT}")
        print(e)
        sys.exit(1)

    print(f"[OK] Connected to {SERIAL_PORT} @ {BAUD_RATE}")
    time.sleep(2)  # allow Arduino reset

    start_time = time.time()
    sample_count = 0

    try:
        with open(OUT_FILE, 'a') as f:
            # f.write("t_ms,gx_lsb,gy_lsb,gz_lsb\n")  # we alredy wite the header in arduino code.

            print("[INFO] Logging started. Press Ctrl+C to stop.")

            while True:

                if DURATION_SEC is not None:
                    if time.time() - start_time >= DURATION_SEC:
                        break

                raw = ser.readline()

                # ---- Decode safely (NO CRASH) ----
                try:
                    line = raw.decode('utf-8', errors='ignore').strip()
                except Exception:
                    continue

                if not line:
                    continue

                parts = line.split(',')
                if len(parts) != 4:
                    continue

                f.write(line + "\n")
                sample_count += 1

                if sample_count % FLUSH_EVERY_N == 0:
                    f.flush()

    except KeyboardInterrupt:
        print("\n[INFO] Stopped by user (Ctrl+C)")

    finally:
        ser.close()
        print(f"[DONE] Samples saved: {sample_count}")
        print(f"[DONE] File: {OUT_FILE}")


if __name__ == "__main__":
    main()
