# code for calculating bias and then plot the uncalib data and calib data 
# we are using the first_5_sec of the 6hr dataset we had. (we can also use other data such as - run a arduino code and get 5 min data and find bias)
# Remember -> first 3/5/10 -Second kay data ka bias IS BETTER than full_6hr data ka bias(avg) -because overtime there is bias drift
# Only see the Plot, no need to use these Bias. since At each startup we calculate the bias .

# to know how many readings / second to collect the data -> 3/5/10 ...
# we also plots the bias of first 1000 readings (1 , 2 , 3 , 4....  and in each it does avg with all the previous data points)
# and plots -> the difference between these bias. 


# ----------------------------------------

# conclusion :- as we can see in the plot that after calib (raw - bias) , 
# the bias is reduced and all the axis values are closer to 0 (at rest all axis must be 0)
# bias dosent reduce the noise and thus you can see the noise (spikes)
# further to remove the noise and drift -> the sf handles it (it does filtering-> averaging of the current value with past n values to reduce the noise, and uses acc, mag data to solve drift problem)
# and as the second chat ka down plot shows that - after 5 seconds / 500 readings - the differnce between the bias is in 10^-5 . 
# so 5 seconds / 500 readings is good since after that no significant gain in bias (10^-5 -> 0.00001 is very small) and it can capture bias drift if done more than that.
# we can get data as per second or reads - both are fine - but the problem with seconds is that if we change the datarate 104hz to 204 hz then  time may it collected the same time may it now collects 2x more data amount of data, which is fine. 
# and if we do 500 samples readings then it dosent matter what hz we keep the samples will be the same. and it does avg with the old values - like seconds may jaisa hota hai. thus bias will be the same.

# So final -> we will do 1000 readings. (dosent matter do it as per Second or Samples - 1000 samples hone chaheay -> ex 1000_reading_at_208hz or 10sec_read_at_104hz --- 1000 readings)


# also by seeing in plot we can see that - after calib  +-0.1dps to +-0.2dps ka noise hai - this means +-0.4 DEGREES ERROR in gyro reading. 

# -----------------------------------------------------------------------




import numpy as np
import matplotlib.pyplot as plt

# ------------ CONFIG -------------
DATA_FILE = "Analysis_big_data/data_dps_125dps_Nofilter.txt"
# ---------------------------------


# ---------- LOAD DATA ----------
data = np.genfromtxt(DATA_FILE, delimiter=',', skip_header=1)

t_ms = data[:, 0]
gx = data[:, 1]
gy = data[:, 2]
gz = data[:, 3]

t_s = (t_ms - t_ms[0]) / 1000.0


# ---------- BIAS FUNCTION ----------
def calc_gyro_bias(gx, gy, gz, t_s, duration_sec):
    mask = t_s <= duration_sec
    return (
        np.mean(gx[mask]),
        np.mean(gy[mask]),
        np.mean(gz[mask])
    )


# ---------- CALCULATE 5s BIAS ----------
bias_5s = calc_gyro_bias(gx, gy, gz, t_s, 5)

print("\n===== Gyro Bias (First 5 sec) =====")
print(f"bx = {bias_5s[0]:.6f} dps")
print(f"by = {bias_5s[1]:.6f} dps")
print(f"bz = {bias_5s[2]:.6f} dps")


# ---------- APPLY CALIBRATION ----------
gx_cal = gx - bias_5s[0]
gy_cal = gy - bias_5s[1]
gz_cal = gz - bias_5s[2]


# ---------- LIMIT TO FIRST 5 MIN ----------
mask_5min = t_s <= 300  # 5 minutes


# ---------- PLOTTING ----------
fig, axs = plt.subplots(1, 2, figsize=(12, 4), sharey=True)
axs[1].tick_params(labelleft=True)
# ---- UNCALIBRATED ----
axs[0].plot(t_s[mask_5min], gx[mask_5min], label="gx")
axs[0].plot(t_s[mask_5min], gy[mask_5min], label="gy")
axs[0].plot(t_s[mask_5min], gz[mask_5min], label="gz")
axs[0].set_title("Uncalibrated (first 5 min)")
axs[0].set_xlabel("Time [s]")
axs[0].set_ylabel("Gyro rate [dps]")
axs[0].set_xlim(0, 300)
axs[0].set_ylim(-0.10, 0.50)
axs[0].grid(True)
axs[0].legend()

# ---- CALIBRATED ----
axs[1].plot(t_s[mask_5min], gx_cal[mask_5min], label="gx")
axs[1].plot(t_s[mask_5min], gy_cal[mask_5min], label="gy")
axs[1].plot(t_s[mask_5min], gz_cal[mask_5min], label="gz")
axs[1].set_title("Calibrated (5s bias, first 5 min)")
axs[1].set_xlabel("Time [s]")
axs[1].set_xlim(0, 300)
axs[1].set_ylim(-1.0, 0.50)
axs[1].grid(True)
axs[1].legend()

plt.tight_layout()
plt.show()


# =========================================================
# COMBINED BIAS CONVERGENCE ANALYSIS (FIRST 1000 READINGS)
# =========================================================

N = 2000  # number of samples to analyze

gx_1000 = gx[:N]
gy_1000 = gy[:N]
gz_1000 = gz[:N]

samples = np.arange(1, N + 1)

# Running bias estimate
bx_run = np.cumsum(gx_1000) / samples
by_run = np.cumsum(gy_1000) / samples
bz_run = np.cumsum(gz_1000) / samples

# Change in bias (delta)
bx_diff = np.abs(np.diff(bx_run))
by_diff = np.abs(np.diff(by_run))
bz_diff = np.abs(np.diff(bz_run))


# ---------- COMBINED PLOT ----------
fig, axs = plt.subplots(2, 1, figsize=(10, 7), sharex=True)

# ---- TOP: RUNNING BIAS ----
axs[0].plot(samples, bx_run, label="gx bias")
axs[0].plot(samples, by_run, label="gy bias")
axs[0].plot(samples, bz_run, label="gz bias")

axs[0].set_ylabel("Estimated bias [dps]")
axs[0].set_title("Bias convergence (running average)")
axs[0].grid(True)
axs[0].legend()

# ---- BOTTOM: BIAS CHANGE (DELTA) ----
axs[1].plot(samples[1:], bx_diff, label="gx Δbias")
axs[1].plot(samples[1:], by_diff, label="gy Δbias")
axs[1].plot(samples[1:], bz_diff, label="gz Δbias")

axs[1].set_xlabel("Number of samples")
axs[1].set_ylabel("Change in bias [dps]")
axs[1].set_yscale("log")  # log scale for convergence visibility
axs[1].grid(True, which="both")
axs[1].legend()

plt.tight_layout()
plt.show()
