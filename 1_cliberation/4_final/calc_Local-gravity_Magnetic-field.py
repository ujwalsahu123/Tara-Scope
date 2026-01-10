# press "H" -> then python send to aurdino and the arudino uses the gps module to fetch the lati, longi, height and sends back to python & then python uses that values to get the local_gravity and Magnetic_field and then we store that lati/longi/height/local_g/Mag_field in location.txt
# but for now we are asking user in terminal for lati, longi, height value. 
# The press "K" , "L" for mag, acc calib then arduino sends collected datapoints to python. & then python uses those data points and local_g / Mag_field from the location.txt to get the offsets/ bias and then stores that into offset.txt

############################################################# 





##########################using gps height for Local g and gps height for Mag filed.###############
# import math
# import requests
# import datetime

# ##################### calculate Local_g using formula (accurate)
# def local_gravity(latitude_deg, height_m):
#     # Convert latitude to radians
#     phi = math.radians(latitude_deg)

#     # WGS-84 constants
#     g0 = 9.7803253359
#     k  = 0.00193185265241
#     e2 = 0.00669437999013

#     # Gravity at sea level
#     g_lat = g0 * (1 + k * math.sin(phi)**2) / math.sqrt(1 - e2 * math.sin(phi)**2)

#     # Height correction
#     g = g_lat - 3.086e-6 * height_m

#     return g  # m/s^2

# ################################## calculate magnetic_field using NOAA api  ( YES )
# def magnetic_field_noaa(lat, lon, alt_m):
#     MODEL = "WMMHR"      # "WMM" or "WMMHR"
#     API_KEY = "EAU2y"

# # -------- AUTO DATE (TODAY) --------
#     today = datetime.date.today()

# # -------- NOAA REQUEST --------
#     url = "https://www.ngdc.noaa.gov/geomag-web/calculators/calculateIgrfwmm"

#     params = {
#     "lat1": lat,
#     "lon1": lon,
#     "elevation": alt_m,
#     "elevationUnits": "M",
#     "coordinateSystem": "D",  #Change it as per the Altitude u put.  # M -> Mean Sea Level height , D -> GPS Height // NOAA app gives the gps height - so put D when using the Noaa mobile app for height.
#     "model": MODEL,
#     "startYear": today.year,
#     "startMonth": today.month,
#     "startDay": today.day,
#     "endYear": today.year,
#     "endMonth": today.month,
#     "endDay": today.day,
#     "resultFormat": "json",
#     "key": API_KEY
#     }

#     data = requests.get(url, params=params).json()
#     return data["result"][0]["totalintensity"]

# # ================= MAIN LOOP =================
# print("Press 'h' to enter location & compute local g + magnetic field")
# print("Press 'q' to quit")

# while True:
#     cmd = input(">> ").strip().lower()

#     if cmd == "q":
#         print("Exiting.")
#         break

#     if cmd == "h":
#         #  asks user for input 
#         #### later instead of asking user - it send the cmd to Arduino where Arduino calculates the lati , longi , height using the gps module and then sends these values to python .
#         lat = float(input("Enter latitude (deg): "))
#         lon = float(input("Enter longitude (deg): "))
#         height = float(input("Enter GPS height (meters): "))

#         # # Save to file
#         # with open("location.txt", "w") as f:
#         #     f.write(f"Latitude  : {lat}\n")
#         #     f.write(f"Longitude : {lon}\n")
#         #     f.write(f"GPS Height: {height} m\n")

#         # Compute local gravity
#         g_local = local_gravity(lat, height)

#         # Compute magnetic field
#         mag_field = magnetic_field_noaa(lat, lon, height)

#         print("\n===== RESULTS =====")
#         print(f"Local gravity (m/s²):      {g_local:.8f}")
#         print(f"Magnetic field (nT):       {mag_field:.2f}")
#         print("===================\n")





















###################################### using MSL height for local_g and gps height for Mag-field ####################
# first convert gps height to msl height - since for local_g it wants MSL (mean sea level) height.


import math
import datetime
import requests
import subprocess
import time
import os

# ============================================================
# EGM2008 (2.5 min) TOOL PATHS
# ============================================================
GEOID_DIR = r"S:\ujwal sahu\Tara_Scope\geoids\EGM2008_Interpolation_Grid"
INTERP_EXE = os.path.join(GEOID_DIR, "interp_2p5min.exe")
INPUT_DAT  = os.path.join(GEOID_DIR, "INPUT.DAT")
OUTPUT_DAT = os.path.join(GEOID_DIR, "OUTPUT3.DAT")


# ============================================================
# LOCAL GRAVITY (WGS-84, accurate)
# ============================================================
def local_gravity(latitude_deg, height_msl_m):
    """
    latitude_deg : degrees
    height_msl_m : meters above mean sea level
    returns local gravity (m/s^2)
    """
    phi = math.radians(latitude_deg)

    g0 = 9.7803253359
    k  = 0.00193185265241
    e2 = 0.00669437999013

    g_lat = g0 * (1 + k * math.sin(phi)**2) / math.sqrt(1 - e2 * math.sin(phi)**2)

    # Free-air correction
    g = g_lat - 3.086e-6 * height_msl_m
    return g


# ============================================================
# GPS → MSL HEIGHT USING EGM2008 (2.5′)
# ============================================================
def gps_to_msl_height(lat_deg, lon_deg, gps_height_m):
    """
    Uses NGA interp_2p5min.exe
    Returns Mean Sea Level height (meters)
    """

    # Write INPUT.DAT (lat lon)
    with open(INPUT_DAT, "w") as f:
        f.write(f"{lat_deg:.6f} {lon_deg:.6f}\n")

    # Run interpolation executable
    subprocess.run(
        [INTERP_EXE],
        cwd=GEOID_DIR,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL
    )

    time.sleep(0.05)

    # Safety check
    if not os.path.exists(OUTPUT_DAT):
        raise RuntimeError("EGM2008 failed: OUTPUT3.DAT not generated")

    # Read geoid undulation
    with open(OUTPUT_DAT, "r") as f:
        line = f.readlines()[-1]

    geoid_N = float(line.split()[-1])   # meters

    # MSL height
    return gps_height_m - geoid_N


# ============================================================
# NOAA MAGNETIC FIELD (WMMHR)
# ============================================================
def magnetic_field_noaa(lat, lon, gps_height_m):
    MODEL = "WMMHR"
    API_KEY = "EAU2y"

    today = datetime.date.today()

    url = "https://www.ngdc.noaa.gov/geomag-web/calculators/calculateIgrfwmm"

    params = {
        "lat1": lat,
        "lon1": lon,
        "elevation": gps_height_m,
        "elevationUnits": "M",
        "coordinateSystem": "D",   # GPS height
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


# ============================================================
# MAIN LOOP (TEST)
# ============================================================
print("Press 'h' → compute msl_height, local g, magnetic field")
print("Press 'q' → quit\n")

while True:
    cmd = input(">> ").strip().lower()

    if cmd == "q":
        print("Exiting.")
        break

    if cmd == "h":
        lat = float(input("Latitude  (deg): "))
        lon = float(input("Longitude (deg): "))
        h_gps = float(input("GPS height (m): "))

        # --- Compute ---
        h_msl = gps_to_msl_height(lat, lon, h_gps)
        g_loc = local_gravity(lat, h_msl) 
        mag   = magnetic_field_noaa(lat, lon, h_gps)

        # --- Results ---
        print("\n===== RESULTS =====")
        print(f"MSL height (m):       {h_msl:.3f}")
        print(f"Local gravity (m/s²): {g_loc:.6f}")
        print(f"Magnetic field (nT):  {mag:.2f}")

        print("===================\n")
