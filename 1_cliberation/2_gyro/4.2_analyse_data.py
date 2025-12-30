# here we analyse the 6hr data that we stored. 
# we can see the noise and drift of the gyro readings 
# also we can see the avg readings (bias) of differnt time intervals 
# here all the plots are on UNcalib_data (dps).

############################################ conclusion ############################################
# 125dps_Nofilter:
# First 5 sec     | gx = 0.193792, gy = -0.741340, gz = 0.315934
# First 1 min     | gx = 0.196034, gy = -0.740922, gz = 0.316692
# First 10 min    | gx = 0.195218, gy = -0.740397, gz = 0.318198
# First 1 hr      | gx = 0.191960, gy = -0.736065, gz = 0.321653
# Full (6hr)      | gx = 0.189009, gy = -0.732939, gz = 0.319918

# 3 IMP things -> Bias(starting from a differnt position) , Noise(unstable data) , Drift(slow change of the bias over long time durations- this makes Data drift over time, since data is-> og_data + bias)

# At rest, the ideal gyro output should be exactly 0 dps on all axes. However, the measured averages show non-zero values (e.g. ~0.19, −0.74, 0.31 dps), 
# so to we can remove the bias we can do -> calib = raw - bias(avg) . so then instead of 0.19 it will be 0.00..
# but removing bias dosent remove Noise or Drift.
# as we can see the Plot that the Data is very Noisy and have many spikes and all.
# and we also see the drift -> gx was 0.19 in startings and it drifted and came to 0.18 , same drift we can see in gy and gz also.
# so we can do calib (raw - bias) and remove the bias, and then sf handles the Noise and drift part.
# for calculating Avg_bias we only do Avg of 5sec . and not long term since over time the bias drifts -> so thus long_term_bias_avg will be a drifted bias_avg . so thats why short_time (5 sec) ka avg we use for bias calibertaion.
# caliberation needs to be done at each startup. - since gyro bias changes as per the temp (ex-> 250dps full night ka bias wasy 18 and 125dps full night ka bias was 19. so ek din rat may jyda thandi thi and ek din kam tha - so even 2-3deg temp change effects the gyro bias), etc.
# and it only takes 5 sec to calc Avg_bias. and this is 5sec is best also.


import numpy as np
import matplotlib.pyplot as plt

# ------------ CONFIG -------------
DATA_FILE = "Analysis_big_data/data_dps_125dps_Nofilter.txt"   # t_ms,gx_dps,gy_dps,gz_dps
FS = 100                     # sampling rate (Hz)
AVG_WINDOW_SEC = 1           # block average window (seconds)
# ---------------------------------


# ---------- LOAD DATA ----------
data = np.genfromtxt(DATA_FILE, delimiter=',', skip_header=1)

t_ms = data[:, 0]
gx = data[:, 1]
gy = data[:, 2]
gz = data[:, 3]

# Convert time to seconds (relative)
t_s = (t_ms - t_ms[0]) / 1000.0


# ---------- 6hr Plot (FULL, VERY NOISY) ----------
plt.figure(figsize=(10, 5))
plt.plot(t_s, gx, label="gx")
plt.plot(t_s, gy, label="gy")
plt.plot(t_s, gz, label="gz")

plt.xlabel("Time [s]")
plt.ylabel("Gyro rate [dps]")
plt.title("6 hr, Uncalib, very noisy")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# ---------- AVERAGING 1sec to plot 6hr data ----------
samples_per_block = int(FS * AVG_WINDOW_SEC)
num_blocks = len(gx) // samples_per_block

gx_avg = np.zeros(num_blocks)
gy_avg = np.zeros(num_blocks)
gz_avg = np.zeros(num_blocks)
t_avg  = np.zeros(num_blocks)

for i in range(num_blocks):
    start = i * samples_per_block
    end   = start + samples_per_block

    gx_avg[i] = np.mean(gx[start:end])
    gy_avg[i] = np.mean(gy[start:end])
    gz_avg[i] = np.mean(gz[start:end])
    t_avg[i]  = np.mean(t_s[start:end])


# ---------- AVERAGED TIME SERIES ----------
plt.figure(figsize=(10, 5))
plt.plot(t_avg, gx_avg, label="gx (1s avg)")
plt.plot(t_avg, gy_avg, label="gy (1s avg)")
plt.plot(t_avg, gz_avg, label="gz (1s avg)")

plt.xlabel("Time [s]")
plt.ylabel("Gyro rate [dps]")
plt.title("Gyro 6hr (1-second averaged), Uncalib")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# ---------- ZOOM: FIRST 1 SECOND ----------
mask_1s = t_s <= 1

plt.figure(figsize=(8, 4))
plt.plot(t_s[mask_1s], gx[mask_1s], label="gx")
plt.plot(t_s[mask_1s], gy[mask_1s], label="gy")
plt.plot(t_s[mask_1s], gz[mask_1s], label="gz")

plt.xlabel("Time [s]")
plt.ylabel("Gyro rate [dps]")
plt.title("first 1 second, Uncalib")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()


# ---------- ZOOM: FIRST 5 SECONDS ----------
mask_5s = t_s <= 5

plt.figure(figsize=(8, 4))
plt.plot(t_s[mask_5s], gx[mask_5s], label="gx")
plt.plot(t_s[mask_5s], gy[mask_5s], label="gy")
plt.plot(t_s[mask_5s], gz[mask_5s], label="gz")

plt.xlabel("Time [s]")
plt.ylabel("Gyro rate [dps]")
plt.title("first 5 second, Uncalib")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()


###################
# bias is basically avg of the readings , so we do avg of the readings with all its previous readings and thus we get bias
# and by plotting this bias of 6hr we can see the bias drift

# ---------- DRIFT 6hr (RUNNING AVERAGE, 10-MIN WINDOW) ----------

WINDOW_SEC = 600                     # 10 minute
SAMPLES_PER_WIN = int(FS * WINDOW_SEC)

num_windows = len(gx) // SAMPLES_PER_WIN

gx_drift = np.zeros(num_windows)
gy_drift = np.zeros(num_windows)
gz_drift = np.zeros(num_windows)
t_drift  = np.zeros(num_windows)

for i in range(num_windows):
    end = (i + 1) * SAMPLES_PER_WIN

    gx_drift[i] = np.mean(gx[:end])
    gy_drift[i] = np.mean(gy[:end])
    gz_drift[i] = np.mean(gz[:end])
    t_drift[i]  = t_s[end - 1]


 # PLOT DRIFT 
plt.figure(figsize=(10, 5))
plt.plot(t_drift / 60, gx_drift, label="gx drift")
plt.plot(t_drift / 60, gy_drift, label="gy drift")
plt.plot(t_drift / 60, gz_drift, label="gz drift")

plt.xlabel("Time [minutes]")
plt.ylabel("Running average [dps]")
plt.title("Gyro bias drift 6hr (10-minute resolution)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()


# ---------- DRIFT (FIRST 1 MINUTE) ----------

END_SEC = 60
mask = t_s <= END_SEC

gx_1m = gx[mask]
gy_1m = gy[mask]
gz_1m = gz[mask]
t_1m  = t_s[mask]

gx_drift_1m = np.cumsum(gx_1m) / np.arange(1, len(gx_1m) + 1)
gy_drift_1m = np.cumsum(gy_1m) / np.arange(1, len(gy_1m) + 1)
gz_drift_1m = np.cumsum(gz_1m) / np.arange(1, len(gz_1m) + 1)

plt.figure(figsize=(8, 4))
plt.plot(t_1m, gx_drift_1m, label="gx drift")
plt.plot(t_1m, gy_drift_1m, label="gy drift")
plt.plot(t_1m, gz_drift_1m, label="gz drift")

plt.xlabel("Time [s]")
plt.ylabel("Running average [dps]")
plt.title("Gyro drift (first 1 minute)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

# ---------- DRIFT (FIRST 10 second) ----------

END_SEC = 10
mask = t_s <= END_SEC

gx_5m = gx[mask]
gy_5m = gy[mask]
gz_5m = gz[mask]
t_5m  = t_s[mask]

gx_drift_5m = np.cumsum(gx_5m) / np.arange(1, len(gx_5m) + 1)
gy_drift_5m = np.cumsum(gy_5m) / np.arange(1, len(gy_5m) + 1)
gz_drift_5m = np.cumsum(gz_5m) / np.arange(1, len(gz_5m) + 1)

plt.figure(figsize=(8, 4))
plt.plot(t_5m / 60, gx_drift_5m, label="gx drift")
plt.plot(t_5m / 60, gy_drift_5m, label="gy drift")
plt.plot(t_5m / 60, gz_drift_5m, label="gz drift")

plt.xlabel("Time [minutes]")
plt.ylabel("Running average [dps]")
plt.title("Gyro drift (first 10 seconds)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()



# ---------- AVERAGE CALCULATIONS ----------
def avg_until(seconds):
    mask = t_s <= seconds
    return (
        np.mean(gx[mask]),
        np.mean(gy[mask]),
        np.mean(gz[mask]),
    )


windows = [
    ("First 5 sec", 5),
    ("First 1 min", 60),
    ("First 10 min", 600),
    ("First 1 hr", 3600),
    ("Full duration", t_s[-1]),
]

print("\n===== Gyro Average (dps) =====")
for name, sec in windows:
    ax, ay, az = avg_until(sec)
    print(f"{name:15s} | "
          f"gx = {ax:.6f}, "
          f"gy = {ay:.6f}, "
          f"gz = {az:.6f}")
