# press "H" -> then python send to aurdino and the arudino uses the gps module to fetch the lati, longi, height and sends back to python & then python uses that values to get the local_gravity and Magnetic_field and then we store that lati/longi/height/local_g/Mag_field in location.txt
# but for now we are asking user in terminal for lati, longi, height value. 
# The press "K" , "L" for mag, acc calib then arduino sends collected datapoints to python. & then python uses those data points and local_g / Mag_field from the location.txt to get the offsets/ bias and then stores that into offset.txt

############################################################# 






import math
import requests
import datetime

##################### calculate Local_g using formula (accurate)
def local_gravity(latitude_deg, height_m):
    # Convert latitude to radians
    phi = math.radians(latitude_deg)

    # WGS-84 constants
    g0 = 9.7803253359
    k  = 0.00193185265241
    e2 = 0.00669437999013

    # Gravity at sea level
    g_lat = g0 * (1 + k * math.sin(phi)**2) / math.sqrt(1 - e2 * math.sin(phi)**2)

    # Height correction
    g = g_lat - 3.086e-6 * height_m

    return g  # m/s^2

################################## calculate magnetic_field using NOAA api  ( YES )
def magnetic_field_noaa(lat, lon, alt_m):
    MODEL = "WMMHR"      # "WMM" or "WMMHR"
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
    "coordinateSystem": "D",  #Change it as per the Altitude u put.  # M -> Mean Sea Level height , D -> GPS Height // NOAA app gives the gps height - so put D when using the Noaa mobile app for height.
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
    return data["result"][0]["totalintensity"]

# ================= MAIN LOOP =================
print("Press 'h' to enter location & compute local g + magnetic field")
print("Press 'q' to quit")

while True:
    cmd = input(">> ").strip().lower()

    if cmd == "q":
        print("Exiting.")
        break

    if cmd == "h":
        #  asks user for input 
        #### later instead of asking user - it send the cmd to Arduino where Arduino calculates the lati , longi , height using the gps module and then sends these values to python .
        lat = float(input("Enter latitude (deg): "))
        lon = float(input("Enter longitude (deg): "))
        height = float(input("Enter GPS height (meters): "))

        # Save to file
        with open("location.txt", "w") as f:
            f.write(f"Latitude  : {lat}\n")
            f.write(f"Longitude : {lon}\n")
            f.write(f"GPS Height: {height} m\n")

        # Compute local gravity
        g_local = local_gravity(lat, height)

        # Compute magnetic field
        mag_field = magnetic_field_noaa(lat, lon, height)

        print("\n===== RESULTS =====")
        print(f"Local gravity (m/s²):      {g_local:.8f}")
        print(f"Magnetic field (nT):       {mag_field:.2f}")
        print("===================\n")