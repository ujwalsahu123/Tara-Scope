# write code so that i sends cmd -> j k l -> to arduino -> for calib gyro , mag , acc.  (single tab - not like press karke rakha hai to 10-20 times it send "j" .... cmd ....use the laser wala code how implmented the single tab functionality)

# 1) arduino asks for offsets and bias from the python and python sees that code and gives the offsets from offset.txt
# arduino always asks for offsets -> so write this part in setup() code. and if it fails to get or etc -> then it uses the hardcoded one -> 
# so write hardcoded offsets and bias and if it gets from python then it updates those arrays ... and uses that new one. and if it dosent get then it simply uses those hardcoded one.
# if it not able to get the data then send - not_available. and when ard sees not available it uses just prints not able to fetch offset.txt and simply dosent updates the hardcoded one - and uses that only.

# 2) it calculates the offset/ bias and updates the hardcode/offset.txt say joo mila.  and prints it.

# 3) the python wait and gets the new printed offset , bias and then stores it in offset. (and it knows ki konsa offset bias update krna hai - acc or gyro or mag kaa since we press that specific key "k" mag, "j" gyro... ) 
# if ard fails to calc the offset/bias then it prints - cant calculate , and py sees it and thus it dosent update the offse.txt and moves onn in the code .

# ard code -> will be like -> in loop() function it checks for any cmd like -> j k l . and using a switch cmd it calls that calib function....  and below that switch cmd -> the timer will be there for 104hz - and it calculates the calib values and prints it, and the py file shows that printed calib values in terminal
# 4) 

# 5) so now what happens is ardu get the latest offsets from py . and stores the latest once, and if not calc the offset then no update in offset.txt file. and if not able to get offset from offset.txt file then also no worries and the arduino uses the hardcoded once.


























# 1) g  → send latest offsets.txt values to → Arduino (so it can update hardcoded values)
# 2) h  → compute local_g + magnetic field → offsets.txt  (before doing "j" / "l" -> you first must do "h")
# 3) j  → gyro calibration (Arduino calcualtes the bias then sends to → Python which stores it in → offsets.txt)
# 4) l  → accel calibration (Arduino prints → Python collects & computes)
# 5) k  → Mag calib ...
# 6) q  → quit
# p = print offset/bias used in Arduino
# y = print 10 calib output values

# hardcoded values -> fine but not best. (intially it uses this hardcoded values for calib)
# 1st always do "g" when you RUN the code (so that we get last calibrated offset/bias values , so that it updates those hardcoded values and use this once for calib)
# then always do "j" gyro calib after 1 min of startup , it also getes updates in offset.txt using python (dont need to do this after every Run , but do this after some hrs has passed from when you previously did)
# do "h" only & always before when you want to do "l" acdel calib , or "k" mag calib.  
# do "l" always when you are on a new location or new height. (more than 1km away from previously calib location , or more than 100meter height difference - then only do ... else not)
# do "k" when at a new location (when you do "l")  OR  when you are in a new setup (ex- you put phone nearby... or placed the device somewhere else ...)

# works -> h , g , j , p , y,  
# so i tested the code and it actaully works ...
# first i did "p" to see the hardcoded values and "y" to see the output values.
# then i did "h" to compute new local_g and mag_field .
# then i did "g" to send offset.txt values to arduino , then i did "p" and "y" to see and all offset.txt values actually got updated in arudino 
# then i did "j" and it did gyro_calib , and i saw the offset.txt and it actually updated there, i did "p" to see the hardcoded upates and the gyro was actually updated
# then i did "l" to do accel calib & it actaully updated the offset.txt and then i did "p" and "y" to saw that the arduino hardcoded values were updated.

######################################################################################################
# // NOTE:
# // Arduino continuously streams sensor data in loop(), which fills the serial buffer.
# // Before doing a command-based handshake (like loading offsets),
# // we intentionally:
# // 1) pause briefly (delay) to let any in-flight serial prints finish
# // 2) flush the serial buffer on Python side
# // This ensures Python receives ONLY the handshake message
# // (e.g. "READY_FOR_OFFSETS") without old sensor data corrupting the protocol.
# // thats why we do this in python side
    # time.sleep(0.05)   # 50 milliseconds dealy to get sync , and then clear the buffer. other wise even after clearing the buffer some values (accel , gyro contineously printed values) slip in the buffer.
    # flush_serial()  # need to clear the buffer -> other wise the python reads all that old accel , gyro contineously printed values , which are in buffer.
    # then read handshake data
# // and this in the arudino side 
    # delay(100); // # 100 milliseconds dealy so that first python clears the buffer at t = 50milli sec par and then uskay 50 millie sec baad -> at t = 100millie sec the hand shake msg is written. (python kay buffer clear karnay kay baad hee hand shake msg print hona chaheay - other wise the handshake msg also gets cleared in buffer. so thats why python clers buffer at 50 ms ,and arduino prints handshake msg at 100ms so 100-50 = 50ms , so buffer clear hone kay 50 ms bad arudino prints the handshake msg.)
    # Serial.println("READY_FOR_OFFSETS"); // handshake message.

# for now we need the method for sync ... but i dont think we need it later .... 
# since we are using this technique so that we clear the buffer and then the handshake msg happens , but if later we dont print accel, gyro , mag values then the buffer will be empty -> thus no need to clear the buffer.
# but still we can keep this technique for safety
##########################################################################################################

# NOTE for later -> here we take input like cmd_press + enter , but in laser pointing we do like keyboard.press ..... 
# so when we mix THIS code with Laser wala code then need to choose a single technique. 
# the cmd+enter technique is better but in that we cant do Arrow button fast movement ... , space_bar laser toggle , (Py dosent always reads data from arduino....)
# so need to think on this... (cmd+enter wala rakha and think how to mix the arrow button , etc with this...)




import serial
import time
import os
import math
import requests
import datetime
import subprocess 
import numpy as np

GEOID_DIR = r"S:\ujwal sahu\Tara_Scope\geoids\EGM2008_Interpolation_Grid"
INTERP_EXE = os.path.join(GEOID_DIR, "interp_2p5min.exe")
INPUT_DAT  = os.path.join(GEOID_DIR, "INPUT.DAT")
OUTPUT_DAT = os.path.join(GEOID_DIR, "OUTPUT3.DAT")

OFFSET_FILE = "1.3_offset.txt"

# ================= SERIAL CONFIG =================
PORT = "COM7"          # CHANGE THIS
BAUD = 115200

ser = serial.Serial(PORT, BAUD, timeout=2)
time.sleep(2)  # Arduino reset delay

# ================= LOCAL GRAVITY calculate using formula (accurate) =================

def local_gravity(latitude_deg, height_msl_m):
    """
    Compute local gravity using WGS-84 normal gravity formula
    Inputs:
      latitude_deg : latitude in degrees
      height_msl_m : height above mean sea level (meters)
    Returns:
      local gravity in m/s^2
    """

    # Convert latitude to radians
    phi = math.radians(latitude_deg)

    # WGS-84 constants
    g0 = 9.7803253359
    k  = 0.00193185265241
    e2 = 0.00669437999013

    # Normal gravity at sea level
    g_lat = g0 * (1 + k * math.sin(phi)**2) / math.sqrt(1 - e2 * math.sin(phi)**2)

    # correction (MSL height) , gives gravity as per height.
    g = g_lat - 3.086e-6 * height_msl_m
        
    g = g * 0.1 # g is like 9.7832.. , but for we want like 0.97832..  so simply multiply by 0.1

    return g  

# ================== Convert Height ==================
def gps_to_msl_height(lat_deg, lon_deg, gps_height_m):
    """
    Uses NGA interp_2p5min.exe to get geoid undulation N
    """

    # --- Write INPUT.DAT ---
    # Format expected by NGA tool:
    # latitude longitude (degrees)
    with open(INPUT_DAT, "w") as f:
        f.write(f"{lat_deg} {lon_deg}\n")

    # --- Run interpolation EXE ---
    result = subprocess.run(
        [INTERP_EXE],
        cwd=GEOID_DIR,
        capture_output=True,
        text=True
        )

    if result.returncode != 0:
        raise RuntimeError(f"EGM2008 exe failed: {result.stderr}")


    time.sleep(0.05)  # allow OUTPUT3.DAT to flush

    # --- Read OUTPUT3.DAT ---
    if not os.path.exists(OUTPUT_DAT):
        raise RuntimeError("EGM2008 interpolation failed: OUTPUT3.DAT not found")

    with open(OUTPUT_DAT, "r") as f:
        lines = f.readlines()
        if not lines:
            raise RuntimeError("OUTPUT3.DAT is empty")

    # Last column = geoid undulation (meters)
    # Example line format:
    # LAT  LON   GEOID(m)
    parts = lines[-1].split()
    geoid_N = float(parts[-1])

    # GPS height → Mean Sea Level
    msl_height = gps_height_m - geoid_N
    return msl_height

# ================= NOAA MAG FIELD =================
def magnetic_field_noaa(lat, lon, alt_m):
    MODEL = "WMMHR"
    API_KEY = "EAU2y"

    # -------- AUTO DATE (TODAY) --------
    today = datetime.date.today()

    # -------- NOAA REQUEST --------
    url = "https://www.ngdc.noaa.gov/geomag-web/calculators/calculateIgrfwmm"

    params = {
        "lat1": lat,
        "lon1": lon,
        "elevation": alt_m,
        "elevationUnits": "M",
        "coordinateSystem": "D", #Change it as per the Altitude u put.  # M -> Mean Sea Level height , D -> GPS Height // NOAA app gives the gps height - so put D when using the Noaa mobile app for height.
        "model": MODEL,
        "startYear": today.year,
        "startMonth": today.month,
        "startDay": today.day,
        "endYear": today.year,
        "endMonth": today.month,
        "endDay": today.day,
        "resultFormat": "json",
        "key": API_KEY
    }

    data = requests.get(url, params=params).json()
    return data["result"][0]["totalintensity"]  # nT

# ================= Update local_g , Mag_field =================
def update_geo_values(g_local, mag_field):
    lines = []

    if os.path.exists(OFFSET_FILE):
        with open(OFFSET_FILE, "r") as f:
            lines = f.readlines()

    # remove old geo entries
    lines = [l for l in lines
             if not l.startswith("LOCAL_G=")
             and not l.startswith("MAG_FIELD=")]

    # append updated values
    lines.append(f"\nLOCAL_G={g_local:.6f}\n")     # 6 decimal is more than enough for Local_g (its too much)
    lines.append(f"MAG_FIELD={mag_field:.2f}\n")    # 2 decimal is more than enought for Mag_field

    with open(OFFSET_FILE, "w") as f:
        f.writelines(lines)
    # print the local_g and Mag_filed in terminal
    print(f"\nLocal gravity (m/s²): {g_local:.6f}")
    print(f"Magnetic field (nT):  {mag_field:.2f}")
    print("✔ LOCAL_G and MAG_FIELD updated in offsets.txt\n")


# ================= SEND LATEST OFFSET/BIAS =================

def send_offsets_to_arduino():
    if not os.path.exists(OFFSET_FILE):
        print("offsets.txt not found")
        return

    # read file
    with open(OFFSET_FILE, "r") as f:
        lines = [l.strip() for l in f if l.strip() and not l.startswith("#")]

    data = {}
    for l in lines:
        k, v = l.split("=")
        data[k] = v

    # ---------- SAFETY CHECK ----------
    required = ["GYRO", "ACC_B", "ACC_A"]
    for k in required:
        if k not in data:
            raise RuntimeError(f"{k} missing in offsets.txt")

    # Flush BEFORE handshake, never after
    time.sleep(0.05)   # 50 milliseconds dealy to get sync , and then clear the buffer. other wise even after clearing the buffer some values (accel , gyro contineously printed values) slip in the buffer.
    flush_serial()  # need to clear the buffer -> other wise the python reads all that old accel , gyro contineously printed values , which are in buffer.


    # wait for Arduino ready # handshake
    while True:
        if ser.in_waiting:
            msg = ser.readline().decode().strip()
            print("ARDUINO:", msg) # good for seeing what did arduino send in buffer.
            if msg == "READY_FOR_OFFSETS":
                break
        time.sleep(0.01) # add dealy since otherwise it loops millions of times per sec

    # dont put any delay here... other wise arduino will read before python send values
    # time.sleep(0.10)

    # send values
    ser.write(f"GYRO,{data['GYRO']}\n".encode())
    ser.write(f"ACC_B,{data['ACC_B']}\n".encode())
    ser.write(b"ACC_A\n")

    for row in data["ACC_A"].split(";"):
        ser.write((row + "\n").encode())

    ser.write(b"END\n") # for safety

    time.sleep(0.10) # 100ms dealy. not necessary , but good .

# wait for Arduino to tell if it actually updated the values # handshake
    while True:
        if ser.in_waiting:
            msg = ser.readline().decode().strip()
            print("ARDUINO:", msg) # good for seeing what did arduino send in buffer.
            if msg == "Updated all offset/bias values":
                print("✔ Done updating\n")
                break
            elif msg == "Error":
                print("Any Error may occured, Arduino was not able to catch 'END' or Read the offset values from python\n")
                break
        time.sleep(0.01) # add dealy since otherwise it loops millions of times per sec

    

# ================= GYRO CALIB & OFFSET UPDATE =================
def do_gyro_calibration():


    time.sleep(0.05)   # 50 milliseconds dealy to get sync , and then clear the buffer. other wise even after clearing the buffer some values (accel , gyro contineously printed values) slip in the buffer.
    flush_serial()  # need to clear the buffer -> other wise the python reads all that old accel , gyro contineously printed values , which are in buffer.

    # No need of handshake here.... since arduino simply prints the single line gyro_bias and python reads it & stores it simply
    
    time.sleep(2) # imp , let arduino write the gryo_bias first before python starts reaings it.

    while True:
        if ser.in_waiting:
            line = ser.readline().decode().strip()
            print("ARDUINO:", line)

            if line.startswith("GYRO_BIAS"):
                _, data = line.split(",", 1)
                bx, by, bz = map(float, data.split(","))

                lines = []

                if os.path.exists(OFFSET_FILE):
                    with open(OFFSET_FILE, "r") as f:
                        lines = f.readlines()

                time.sleep(0.10) # 100ms dealy before writting data in file. not necessary , but good .

                # remove old gyro line (anywhere)
                lines = [l for l in lines if not l.startswith("GYRO=")]

                # append new gyro bias
                lines.append(f"\nGYRO={bx},{by},{bz}\n")

                with open(OFFSET_FILE, "w") as f:
                    f.writelines(lines)

                time.sleep(0.10) # 100ms dealy. not necessary , but good .

                print("✔ Gyro bias updated in offset.txt\n")
                return
            
        time.sleep(0.01)  # add dealy since otherwise it loops millions of times per sec




#  ================= Accel offset/bias Algo =================

def calibrate_accelerometer(data, g=1.0):
    """
    Robust Magneto-style accelerometer calibration.

    data : Nx3 numpy array (scaled to g units)
    g    : target gravity (1.0 or local g)

    Returns:
        A : 3x3 scale / misalignment matrix
        b : 3x1 bias vector
    """

    x = data[:, 0]
    y = data[:, 1]
    z = data[:, 2]

    # Design matrix
    D = np.column_stack([
        x*x, y*y, z*z,
        2*x*y, 2*x*z, 2*y*z,
        2*x, 2*y, 2*z,
        np.ones_like(x)
    ])

    # Solve least squares
    _, _, Vt = np.linalg.svd(D, full_matrices=False)
    beta = Vt[-1, :]  # smallest singular vector

    # Quadratic form
    Q = np.array([
        [beta[0], beta[3], beta[4]],
        [beta[3], beta[1], beta[5]],
        [beta[4], beta[5], beta[2]]
    ])

    p = beta[6:9]
    c = beta[9]

    # Bias (center of ellipsoid)
    b = -np.linalg.inv(Q) @ p

    # Normalize so surface maps to g
    k = (b @ Q @ b - c)
    Qn = Q / k * (g * g)

    # Eigen-decomposition (SAFE)
    eigvals, eigvecs = np.linalg.eigh(Qn)

    # Build correction matrix
    A = eigvecs @ np.diag(np.sqrt(eigvals)) @ eigvecs.T

    return A, b



# ================= ACCEL CALIB & OFFSET Update =================
def do_accel_calibration():
    """
    1. Collect raw accel data from Arduino
    2. Scale to g
    3. Read LOCAL_G from offsets.txt
    4. Compute ACC_A, ACC_B
    5. Store in offsets.txt
    6. Send to Arduino
    """

    time.sleep(0.05)   # 50 milliseconds dealy to get sync , and then clear the buffer. other wise even after clearing the buffer some values (accel , gyro contineously printed values) slip in the buffer.
    flush_serial()  # need to clear the buffer -> other wise the python reads all that old accel , gyro contineously printed values , which are in buffer.

    raw_samples = []

    # --- Collect data ---

    while True:
        if ser.in_waiting:
            line = ser.readline().decode().strip()

            # ---- START MARKER ----
            if line == "ACC_RAW_DATA_SHARING_START":
                print("ARDUINO:", line)
                raw_samples.clear()
                continue

            # ---- END MARKER ----
            if line == "ACC_RAW_DATA_SHARING_DONE":
                print(f"ARDUINO: {line} ")
                break

            # ---- DATA LINE (ax,ay,az) (append new data points) ----
            try:
                ax, ay, az = map(float, line.split(","))
                raw_samples.append([ax, ay, az])
            except ValueError:
                # Ignore any garbage / unexpected prints safely
                pass

        time.sleep(0.005)


    if len(raw_samples) < 10:
        raise RuntimeError("Not enough accel samples collected")
    
    print(f"✔ Collected {len(raw_samples)} samples")

    raw = np.array(raw_samples)

    # --- Scale LSB → g ---
    # best is that we give raw Avg LSB values from Arduino and Python may scale it , otherwise if we scale in arduino then arduino float cant handle that many decimals, and preision loss hoga.
    ACC_LSB_TO_G = 0.00006103515625
    data_g = raw * ACC_LSB_TO_G

    # --- Read LOCAL_G ---
    g_local = None
    with open(OFFSET_FILE, "r") as f:
        for l in f:
            if l.startswith("LOCAL_G="):
                g_local = float(l.split("=")[1])
                break

    if g_local is None:
        raise RuntimeError("LOCAL_G not found in offsets.txt")

    # --- Calibrate ---
    A, b = calibrate_accelerometer(data_g, g = g_local)

    # ---- BASIC SAFETY CHECK ----    
    # so if the data is not covering many orientations then the calib_Algo fails to make a ellipsoid/etc and it returns A = nan values .. so we ckech if values are good or not here.,
    # if values are not good then we simply print -> failed , and return the function & stop, so that it dosent store that bad values in offset.txt and dont send to arduino.
    if (
        not np.isfinite(A).all() or
        not np.isfinite(b).all() or
        np.allclose(A, 0) or
        np.allclose(b, 0)
        ):
        print("\n❌ ACCEL CALIB FAILED → bad values (NaN / zero). Likely insufficient orientations or bad data.")
        ser.write("ACCEL_CALIB_FAILED\n") # also tell arduino that accel calib failed. 
        # return  - dont return now ... lets first confirm with arduino side also....
        while True:
            if ser.in_waiting:
                msg = ser.readline().decode().strip()
                print("ARDUINO:", msg) # good for seeing what did arduino send in buffer.
                if msg == "ERROR_GETTING_UPDATED_VALUES":
                    print("Accel_Calib Process Stopped Safely in Arduino & python\n")
                    return # now after confirming with arduino side also we return from function
                else:
                    print("Any Error Occured.(arduino was not even able to read the 'ACCEL_CALIB_FAILED' msg) still exiting from - Arduino & Python Accel Calib function\n")
                    return # exit the function if any error occurs
            time.sleep(0.01) # add dealy since otherwise it loops millions of times per sec

    
    # ---- SAFE TO CONTINUE FURTHER ----
    # if we have good values then move futher in function & store the values and send to ard.


    # --- Save to offsets.txt ---
    lines = []
    if os.path.exists(OFFSET_FILE):
        with open(OFFSET_FILE, "r") as f:
            lines = f.readlines()

    # remove old accel entries
    lines = [l for l in lines if not l.startswith("ACC_A=") and not l.startswith("ACC_B=")]
    
    
    A_flat = ";".join(
        ",".join(f"{A[i,j]:.6f}" for j in range(3))        ################# storing 6 decimals of ACC offset/bias in offset.txt (uno -> 6 , esp -> 8)
        for i in range(3)
    )
    b_flat = ",".join(f"{b[i]:.6f}" for i in range(3))

    lines.append(f"\nACC_B={b_flat}\n")
    lines.append(f"\nACC_A={A_flat}\n")

    with open(OFFSET_FILE, "w") as f:
        f.writelines(lines)

    print("✔ Updated ACC_A and ACC_B written to offsets.txt")


    # ---------- SEND ACC offset/bias TO ARDUINO ----------

    # --- HANDSHAKE ---
    ser.write("REQUEST_FOR_SENDING_UPDATED_VALUES\n")
    while True:
        if ser.in_waiting:
            msg = ser.readline().decode().strip()
            print("ARDUINO:", msg) # good for seeing what did arduino send in buffer.
            if msg == "READY_FOR_UPDATED_VALUE":
                print("Sending Updated Values To Arduino\n")
                break
            else:
                print("Any Error may occured, While HandShake. Exit from - Arduino & Python Calib function \n")
                return # exit the function if any error occurs
        time.sleep(0.01) # add dealy since otherwise it loops millions of times per sec

    # time.sleep(0.05) dont put any delay here ... other wise arduino will read before python writting the values 
    #


    # ---- ACC_B ----
    b_str = ",".join(f"{v:.6f}" for v in b)   ################# sending 6 decimals of ACC offset/bias to ard (uno -> 6 , esp -> 8)
    ser.write(f"ACC_B,{b_str}\n".encode())

    # ---- ACC_A ----
    ser.write(b"ACC_A\n")

    for i in range(3):
        row_str = ",".join(f"{A[i, j]:.6f}" for j in range(3))
        ser.write((row_str + "\n").encode())
    ser.write(b"END\n")

    # ---------- WAIT FOR ACK ----------

    while True:
        if ser.in_waiting:
            msg = ser.readline().decode().strip()
            print("ARDUINO:", msg) # good for seeing what did arduino send in buffer.
            if msg == "ACC_UPDATED":
                print("✔ Accel offsets updated on Arduino\n")
                break
            elif msg == "Error":
                print("Any Error may occured, Arduino was not able to catch 'END' or not able to Read the offset values from python\n")
                break
        time.sleep(0.01) # add dealy since otherwise it loops millions of times per sec




# ================= flush buffer =================
def flush_serial():
    ser.reset_input_buffer()
    ser.reset_output_buffer()
    time.sleep(0.05)   # allow hardware buffer to settle




# ================= MAIN LOOP =================


print("\nIMU Python Controller Ready")
print("g = send offsets | h = geo calc | j = gyro calib | l = accel calib | p = print offset/bias used in Arduino | q = quit\n")

while True:
    
    cmd = input(">> ").strip().lower()

    # ---------- KEYBOARD COMMANDS ----------
    if cmd == "g": # send latest offset/bias from offset.txt to arduino and update the hardcoded ones (works)
        print("→ Sending offsets to Arduino")
        ser.write(b'g\n')           # trigger Arduino
        send_offsets_to_arduino()   # mostly all offset/bias values are in 6-8 decimals so we directly send the values of offset.txt without any trimming (like :.6f)
    
    elif cmd == "j": # send the j cmd to arudino where it does gyro calib , and then python cathes the bias value and stores it in offset.txt (works)
        print("→ Starting gyro calibration")
        ser.write(b'j\n')    # trigger Arduino
        do_gyro_calibration() # we get 6/8 decimal values of gyro bias from arduino (set n_gyro_bias in arduino) - and we directly store that in offset.txt without any trimming (like :.6f)

    elif cmd == "l": # send the l cmd to arduino so that arduino prints the acc data - and python collects it also python uses the local_g data from offset.txt then using the collected data and local_g it computes the accel offset/bias and stores in offset.txt and also sends to arudino to update the hardcoded accel offset/bais. 
        print("→ Starting accel calibration")
        ser.write(b'l\n')     # trigger Arduino
        do_accel_calibration()
      

    elif  cmd == "h": # calculate the Local_g and Magnetic field and store it in offset.txt (works)
        print("→ Computing local gravity & magnetic field")
        #  asks user for input 
        ############# later instead of asking user - it send the cmd to Arduino where Arduino calculates the lati , longi , height using the gps module and then sends these values to python .
        lat = float(input("Enter latitude (deg): "))  # ex : 19.111111..
        lon = float(input("Enter longitude (deg): ")) # ex : 71.111111..
        height = float(input("Enter GPS height (meters): ")) # ex : -51.12..
        # compute Local gravity and Magnetic filed
        msl_height = gps_to_msl_height(lat, lon, height) # first compute the Mean Sea Level Height using the Gps height  # ex: msl_height = -20.81..
        g_local = local_gravity(lat, msl_height) # ex : 9.785930783.... 
        mag_field = magnetic_field_noaa(lat, lon, height) # Use gps hieght for Magnetic field.  # ex: 43357.30383....
        update_geo_values(g_local, mag_field) # store values in offset.txt  # ex: Local_g = 9.785930 (6 dicimals enough) || Mag_field = 43357.30 (2 dcimals enough)


    elif cmd == "p": # print offset/bias values currently used in ARDUINO. (works)
        ser.write(b'p\n')     # trigger Arduino
        time.sleep(0.05) #imp
        flush_serial() #imp
        while True:
            if ser.in_waiting:
                msg = ser.readline().decode().strip()
                print("ARDUINO:", msg) 
                if msg == "DONE":
                    print("DONE\n")
                    break


    elif cmd == "y": # Prints 10 calib values output... # not use full , just for debugging  (works)
        ser.write(b'y\n')     # trigger Arduino
        time.sleep(0.05) #imp
        flush_serial() #imp
        while True:
            if ser.in_waiting:
                msg = ser.readline().decode().strip()
                print("ARDUINO:", msg) 
                if msg == "DONE":
                    print("DONE\n")
                    break

    elif cmd == "q":
        print("Exiting.")
        break

    
    ##### Later we can put a code for serial buffer cearing after every x interval. ( think of what methoed to use -> count++ / time based /// . also run in barckgorund or over here only... )
    # also since we are using input() for user input and not keyboard.press so the loop dosent run contineously -> it only iterates after user has given some input... so thats why i told above to run in backgound -> to check x interval / time and clear the buffer.
    # but in sf , starpointing we will not do any Calib_value/ RPY prining so no need to clear the buffer .... 


    time.sleep(0.05)

ser.close()
 