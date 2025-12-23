# // filter values i will test and do anlysis using the python . 

# // so one by one it will give filter values (10, 20 , 45, 100) (ultra_light, very_light, light, medium) and then we run analysis on it. 


#  arduino code for getting the data for analysis.
####################################################################################################


# // ONLY LP filter

# #include <Wire.h>
# #include "SparkFun_ISM330DHCX.h"

# SparkFun_ISM330DHCX myISM;

# sfe_ism_raw_data_t accRaw;
# sfe_ism_raw_data_t gyroRaw;

# const int SAMPLE_COUNT = 100;

# // --------------------------------------------------
# // Helpers
# // --------------------------------------------------

# void collectAccelSamples(const char *label)
# {
#   unsigned long tStart = micros();

#   Serial.println(label);
#   for (int i = 0; i < SAMPLE_COUNT; i++)
#   {
#     while (!myISM.checkAccelStatus());
#     myISM.getRawAccel(&accRaw);

#     Serial.print(accRaw.xData);
#     Serial.print('\t');
#     Serial.print(accRaw.yData);
#     Serial.print('\t');
#     Serial.println(accRaw.zData);
#   }

#   unsigned long tEnd = micros();
#   Serial.print("Time_us: ");
#   Serial.println(tEnd - tStart);
#   Serial.println();
# }

# void collectGyroSamples(const char *label)
# {
#   unsigned long tStart = micros();

#   Serial.println(label);
#   for (int i = 0; i < SAMPLE_COUNT; i++)
#   {
#     while (!myISM.checkGyroStatus());
#     myISM.getRawGyro(&gyroRaw);

#     Serial.print(gyroRaw.xData);
#     Serial.print('\t');
#     Serial.print(gyroRaw.yData);
#     Serial.print('\t');
#     Serial.println(gyroRaw.zData);
#   }

#   unsigned long tEnd = micros();
#   Serial.print("Time_us: ");
#   Serial.println(tEnd - tStart);
#   Serial.println();
# }

# // --------------------------------------------------
# // Setup
# // --------------------------------------------------

# void setup()
# {
#   Serial.begin(115200);
#   Wire.begin();
#   Wire.setClock(400000);

#   if (!myISM.begin())
#   {
#     Serial.println("ISM not detected");
#     while (1);
#   }

#   myISM.deviceReset();
#   while (!myISM.getDeviceReset());

#   myISM.setDeviceConfig();
#   myISM.setBlockDataUpdate();

#   // ODR = 208 Hz
#   myISM.setAccelDataRate(ISM_XL_ODR_208Hz);
#   myISM.setGyroDataRate(ISM_GY_ODR_208Hz);

#   // Ranges
#   myISM.setAccelFullScale(ISM_2g);
#   myISM.setGyroFullScale(ISM_250dps);

#   // Enable base LP filters
#   myISM.setAccelFilterLP2(true);
#   myISM.setGyroFilterLP1(true);

#   delay(1000);

#   // --------------------------------------------------
#   // ACCEL FILTER TEST
#   // --------------------------------------------------

#   Serial.println("===== ACCEL FILTER TEST (LP DIV) =====");

#   myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_10);
#   collectAccelSamples("ACC_LP_DIV_10");

#   myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_20);
#   collectAccelSamples("ACC_LP_DIV_20");

#   myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_45);
#   collectAccelSamples("ACC_LP_DIV_45");

#   myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100);
#   collectAccelSamples("ACC_LP_DIV_100");

#   myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_200);
#   collectAccelSamples("ACC_LP_DIV_200");

#   myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_400);
#   collectAccelSamples("ACC_LP_DIV_400");

#   myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_800);
#   collectAccelSamples("ACC_LP_DIV_800");

#   // --------------------------------------------------
#   // GYRO FILTER TEST
#   // --------------------------------------------------

#   Serial.println("===== GYRO FILTER TEST (LP1 BW) =====");

#   myISM.setGyroLP1Bandwidth(ISM_ULTRA_LIGHT);
#   collectGyroSamples("GYRO_ULTRA_LIGHT");

#   myISM.setGyroLP1Bandwidth(ISM_VERY_LIGHT);
#   collectGyroSamples("GYRO_VERY_LIGHT");

#   myISM.setGyroLP1Bandwidth(ISM_LIGHT);
#   collectGyroSamples("GYRO_LIGHT");

#   myISM.setGyroLP1Bandwidth(ISM_MEDIUM);
#   collectGyroSamples("GYRO_MEDIUM");

#   myISM.setGyroLP1Bandwidth(ISM_STRONG);
#   collectGyroSamples("GYRO_STRONG");

#   myISM.setGyroLP1Bandwidth(ISM_VERY_STRONG);
#   collectGyroSamples("GYRO_VERY_STRONG");

#   myISM.setGyroLP1Bandwidth(ISM_AGGRESSIVE);
#   collectGyroSamples("GYRO_AGGRESSIVE");

#   myISM.setGyroLP1Bandwidth(ISM_XTREME);
#   collectGyroSamples("GYRO_XTREME");

# }

# void loop()
# {
#   // intentionally empty
# }















################################################################################################


# //ALL filters Analysis

# #include <Wire.h>
# #include "SparkFun_ISM330DHCX.h"

# SparkFun_ISM330DHCX myISM;

# sfe_ism_raw_data_t accRaw;
# sfe_ism_raw_data_t gyroRaw;

# const int SAMPLE_COUNT = 100;

# // --------------------------------------------------
# // Helpers
# // --------------------------------------------------

# void collectAccel(const char *label)
# {
#   unsigned long tStart = micros();

#   Serial.println(label);
#   for (int i = 0; i < SAMPLE_COUNT; i++)
#   {
#     while (!myISM.checkAccelStatus());
#     myISM.getRawAccel(&accRaw);

#     Serial.print(accRaw.xData);
#     Serial.print('\t');
#     Serial.print(accRaw.yData);
#     Serial.print('\t');
#     Serial.println(accRaw.zData);
#   }

#   unsigned long tEnd = micros();
#   Serial.print("Time_us: ");
#   Serial.println(tEnd - tStart);
#   Serial.println();
# }

# void collectGyro(const char *label)
# {
#   unsigned long tStart = micros();

#   Serial.println(label);
#   for (int i = 0; i < SAMPLE_COUNT; i++)
#   {
#     while (!myISM.checkGyroStatus());
#     myISM.getRawGyro(&gyroRaw);

#     Serial.print(gyroRaw.xData);
#     Serial.print('\t');
#     Serial.print(gyroRaw.yData);
#     Serial.print('\t');
#     Serial.println(gyroRaw.zData);
#   }

#   unsigned long tEnd = micros();
#   Serial.print("Time_us: ");
#   Serial.println(tEnd - tStart);
#   Serial.println();
# }

# // --------------------------------------------------
# // Setup
# // --------------------------------------------------

# void setup()
# {
#   Serial.begin(115200);
#   Wire.begin();
#   Wire.setClock(400000);

#   if (!myISM.begin())
#   {
#     Serial.println("ISM not detected");
#     while (1);
#   }

#   myISM.deviceReset();
#   while (!myISM.getDeviceReset());

#   myISM.setDeviceConfig();
#   myISM.setBlockDataUpdate();

#   // ODR = 208 Hz
#   myISM.setAccelDataRate(ISM_XL_ODR_208Hz);
#   myISM.setGyroDataRate(ISM_GY_ODR_208Hz);

#   // Ranges
#   myISM.setAccelFullScale(ISM_2g);
#   myISM.setGyroFullScale(ISM_250dps);

#   // Enable base LP filters
#   myISM.setAccelFilterLP2(true);
#   myISM.setGyroFilterLP1(true);

#   delay(1000);

#     // ======================================================
#   // ACCEL — LP (Slope) FILTERS
#   // ======================================================
#   Serial.println("===== ACCEL FILTER TEST =====");

#   myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_10);  collectAccel("ACC_LP_DIV_10");
#   myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_20);  collectAccel("ACC_LP_DIV_20");
#   myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_45);  collectAccel("ACC_LP_DIV_45");
#   myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100); collectAccel("ACC_LP_DIV_100");
#   myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_200); collectAccel("ACC_LP_DIV_200");
#   myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_400); collectAccel("ACC_LP_DIV_400");
#   myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_800); collectAccel("ACC_LP_DIV_800");

#   // ======================================================
#   // ACCEL — HIGH PASS FILTERS
#   // ======================================================

#   myISM.setAccelSlopeFilter(ISM_HP_ODR_DIV_10);  collectAccel("ACC_HP_DIV_10");
#   myISM.setAccelSlopeFilter(ISM_HP_ODR_DIV_45);  collectAccel("ACC_HP_DIV_45");
#   myISM.setAccelSlopeFilter(ISM_HP_ODR_DIV_100); collectAccel("ACC_HP_DIV_100");
#   myISM.setAccelSlopeFilter(ISM_HP_ODR_DIV_200); collectAccel("ACC_HP_DIV_200");

#   // ======================================================
#   // ACCEL — HP REF MODE (motion detection)
#   // ======================================================

#   myISM.setAccelSlopeFilter(ISM_HP_REF_MD_ODR_DIV_45);  collectAccel("ACC_HP_REF_MD_45");
#   myISM.setAccelSlopeFilter(ISM_HP_REF_MD_ODR_DIV_100); collectAccel("ACC_HP_REF_MD_100");

#   // ======================================================
#   // GYRO — LP1 BANDWIDTH
#   // ======================================================
#   Serial.println("===== GYRO FILTER TEST  =====");

#   myISM.setGyroLP1Bandwidth(ISM_ULTRA_LIGHT);  collectGyro("GYRO_ULTRA_LIGHT");
#   myISM.setGyroLP1Bandwidth(ISM_VERY_LIGHT);   collectGyro("GYRO_VERY_LIGHT");
#   myISM.setGyroLP1Bandwidth(ISM_LIGHT);        collectGyro("GYRO_LIGHT");
#   myISM.setGyroLP1Bandwidth(ISM_MEDIUM);       collectGyro("GYRO_MEDIUM");
#   myISM.setGyroLP1Bandwidth(ISM_STRONG);       collectGyro("GYRO_STRONG");
#   myISM.setGyroLP1Bandwidth(ISM_VERY_STRONG);  collectGyro("GYRO_VERY_STRONG");
#   myISM.setGyroLP1Bandwidth(ISM_AGGRESSIVE);   collectGyro("GYRO_AGGRESSIVE");
#   myISM.setGyroLP1Bandwidth(ISM_XTREME);       collectGyro("GYRO_XTREME");

#   // ======================================================
#   // GYRO — HIGH PASS FILTERS
#   // ======================================================

#   myISM.setGyroFilterLP1(ISM_HP_FILTER_NONE);   collectGyro("GYRO_HP_NONE");
#   myISM.setGyroFilterLP1(ISM_HP_FILTER_16mHz);  collectGyro("GYRO_HP_16mHz");
#   myISM.setGyroFilterLP1(ISM_HP_FILTER_65mHz);  collectGyro("GYRO_HP_65mHz");
#   myISM.setGyroFilterLP1(ISM_HP_FILTER_260mHz); collectGyro("GYRO_HP_260mHz");
#   myISM.setGyroFilterLP1(ISM_HP_FILTER_1Hz04);  collectGyro("GYRO_HP_1Hz04");

# }

# void loop() {}
















import numpy as np
import matplotlib.pyplot as plt

FILE = "filter.txt"

# -------------------------------------------------
# PARSER
# -------------------------------------------------

def parse_file(filename):
    blocks = {}
    current_block = None
    current_mode = None  # ACC or GYRO

    with open(filename, "r") as f:
        for raw_line in f:
            line = raw_line.strip()

            if not line:
                continue

            # Section headers
            if "ACCEL FILTER TEST" in line:
                current_mode = "ACC"
                continue

            if "GYRO FILTER TEST" in line:
                current_mode = "GYRO"
                continue

            # New filter block
            if line.startswith("ACC_") or line.startswith("GYRO_"):
                current_block = line
                blocks[current_block] = {
                    "mode": current_mode,
                    "samples": [],
                    "time_us": None
                }
                continue

            # Time
            if line.startswith("Time_us"):
                if current_block:
                    blocks[current_block]["time_us"] = float(line.split(":")[1])
                continue

            # Sample line
            parts = line.split()
            if len(parts) == 3 and current_block:
                try:
                    x, y, z = map(float, parts)
                    blocks[current_block]["samples"].append([x, y, z])
                except:
                    pass

    return blocks


# -------------------------------------------------
# ANALYSIS (CORRECT METHOD)
# -------------------------------------------------

def analyze(blocks):
    print("\n================ CORRECT FILTER ANALYSIS =================\n")

    summary = []

    for name, data in blocks.items():
        samples = np.array(data["samples"])

        if samples.shape[0] < 10:
            continue

        # Per-axis data
        x = samples[:, 0]
        y = samples[:, 1]
        z = samples[:, 2]

        # Remove DC (gravity / bias)
        x_d = x - np.mean(x)
        y_d = y - np.mean(y)
        z_d = z - np.mean(z)

        # RMS noise per axis
        nx = np.std(x_d)
        ny = np.std(y_d)
        nz = np.std(z_d)

        noise_avg = (nx + ny + nz) / 3.0

        print(name)
        print(f"  Mode            : {data['mode']}")
        print(f"  Samples         : {len(samples)}")
        print(f"  Noise X (RMS)   : {nx:.6f}")
        print(f"  Noise Y (RMS)   : {ny:.6f}")
        print(f"  Noise Z (RMS)   : {nz:.6f}")
        print(f"  Avg Noise (RMS) : {noise_avg:.6f}")
        print(f"  Time (us)       : {data['time_us'] if data['time_us'] else 'N/A'}")
        print()

        summary.append({
            "name": name,
            "mode": data["mode"],
            "noise": noise_avg,
            "time_us": data["time_us"]
        })

    return summary


# -------------------------------------------------
# PLOTS
# -------------------------------------------------

def plot_tradeoff(summary):
    def plot_group(group, title):
        times = []
        noise = []
        labels = []

        for s in group:
            if s["time_us"] is None:
                continue
            times.append(s["time_us"] / 1000.0)  # ms
            noise.append(s["noise"])
            labels.append(s["name"])

        plt.figure(figsize=(9, 5))
        plt.scatter(times, noise)

        for i, lbl in enumerate(labels):
            plt.annotate(lbl, (times[i], noise[i]), fontsize=8)

        plt.xlabel("Time for 100 samples (ms)")
        plt.ylabel("Avg RMS Noise (per-axis)")
        plt.title(title)
        plt.grid(True)
        plt.tight_layout()
        plt.show()

    acc = [s for s in summary if s["mode"] == "ACC"]
    gyro = [s for s in summary if s["mode"] == "GYRO"]

    plot_group(acc, "Accelerometer Filter Trade-off")
    plot_group(gyro, "Gyroscope Filter Trade-off")


# -------------------------------------------------
# MAIN
# -------------------------------------------------

if __name__ == "__main__":
    blocks = parse_file(FILE)
    summary = analyze(blocks)
    plot_tradeoff(summary)


# conclusion -> 
# all time are same - and no one is slow. 
# but the real slowness is the phase-delay (responsivness) (but we cannot analyse that)
# and at rest the best noise reductin is done by lowest filter (but thats not the game)
# and at motion the higher the filter the more noise reduction it did. 
# so all filters take same time
# Lower filters -> more noisy , but fast responsiveness
# Higher filters -> less noisy , but Slower responsiveness . 
# keep acc -> 45 
# keep gryo -> Light 
# in SF can turn off the filters and test. and see what happens. 