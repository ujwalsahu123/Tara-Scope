# Allen Variance & white gausian noise Analysis on the scaled data.txt. 
# we are not using timestamp, since timestamp is of no use, but its a good practice to store it since it can be used for degugging any issues later
# we can check the Hz of our DataStored to see if we did proper Sampling. -ex : in 21600 sec we recoded 2159900 data points. so (No._of_dataPoints / time_sec) => 2159900/21600 => 100hz, so we did proper data sampling, & we can use this Hz in plotting also.



# There are two major error categories in a gyroscope:
# 1 Noise:
# Random, short-term fluctuations
# Dominated by Angle Random Walk (ARW)
# 2 Drift:
# Slow variation of bias over time
# Characterized by Bias Instability

# using the allen deviation analyses we find the ARW and bias instability


# 1️⃣ Angle Random Walk (ARW)
# Caused by white noise in rate measurements
# Dominates short integration times
# Appears as a −0.5 slope in Allan deviation
# Determines how fast angle drifts due to noise
# For 250 dps data (X-axis):
# ARW ≈ 0.006227 × 60 = 0.373 deg/√hr

# 2️⃣ Bias Instability
# Represents how stable the gyro bias is over time
# Appears as the minimum point of Allan deviation
# Dominates medium time scales
# For 250 dps data (X-axis):
# Bias Instability ≈ (0.0005623 / 0.664) × 3600 ≈ 3.04 deg/hr

# like this we can find ARW and Bias_instability for y and z axis also. 

# Gaussian White Noise validation : 
# The left side of the Allan deviation plot follows a −0.5 slope
# This confirms the gyro noise is Gaussian white
# This is important because -
# Kalman filters and sensor fusion algorithms assume Gaussian noise
# Validates that standard estimation models apply


#  kalman filter, etc sf - estimation algorithms commonly assume gyro biases are constant. If the gyro sensor bias instability is high, the bias will drift and the estimation algorithms will have a difficult time estimating biases.


# do we only do analysis - or does it also help us in gyro calib ?????
# These numbers are NOT used for calibration.
# They are used for sensor fusion (SF).

# What “sensor fusion” needs
# Sensor fusion (Kalman, EKF, Madgwick, etc.) needs to know:
# How noisy is the gyro?
# How fast does the bias drift?
# How much should I trust the gyro vs accel/mag?
# 👉 THIS is exactly what ARW & bias instability tell you.

# Where your numbers are used (concretely)
# Your results:
# ARW ≈ 0.37 deg/√hr
# Bias instability ≈ 3 deg/hr
# These numbers are used to:
# Set process noise (Q) in Kalman filters
# Decide how quickly to re-estimate gyro bias
# Decide how aggressively accel/mag should correct gyro drift
# Example idea (conceptual):
# Low ARW → trust gyro short-term
# High bias instability → correct bias more often


# Need to learn more how to use this values in SF 
# like is there functions we gives this values to , or etc. 
# i think we use these values to configure the filters and sf - but how ?

# Q_angle = 0.001; // kf  
# Q_bias  = 0.003; // kf
# beta  // Madgwick
# kp, ki // Mahony
# Lower ARW → smaller beta
# Higher bias drift → higher ki


# Case 1: Simple AHRS (Madgwick / Mahony / most Arduino libs)
# ❌ You do NOT give ARW or bias-instability explicitly
# ❌ Filter does NOT calculate them
# ✅ You tune gains (beta, kp, ki) by hand
# 👉 Allan deviation helps you choose good gain values, but the filter never “knows” ARW numerically.

# Case 2: Kalman / EKF (proper probabilistic filters)
# ❌ Filter does NOT auto-estimate ARW or bias instability
# ✅ YOU set them as noise parameters (Q matrix)
# ❌ They are not learned automatically (unless you implement adaptive EKF)
# So you explicitly tell the filter:
# how noisy the gyro is
# how fast bias can drift

# ✅ Case 1: Kalman / EKF (where ARW & BI actually matter)
# You do NOT call a special function like:
# setARW(...)
# Instead, you:
# edit the filter parameters (usually Q or variables like Q_angle, Q_bias)
# this is either:
# in the code
# in a config file
# in constants at the top of the library
# Example idea (conceptual):
# Q_angle = (ARW_in_deg_per_sqrt_s)^2;
# Q_bias  = (bias_drift_in_deg_per_s)^2;
# 👉 So yes:
# You go into the Kalman filter code and set those values.

# ❌ Case 2: Madgwick / Mahony (most Arduino AHRS)
# There is NO place to put ARW / BI explicitly.
# You only tune:
# beta (Madgwick)
# kp, ki (Mahony)
# So:
# You do NOT pass your ARW value
# You use it only to decide good gain values
# 👉 Nothing to edit inside the library except gains.







import numpy as np
import matplotlib.pyplot as plt


# ---------------- CONFIG ----------------
DATA_FILE = "data_dps_250dps_Nofilter.txt"   # t_ms,gx_dps,gy_dps,gz_dps

FS = 2159900 / 21600   # or simply 100     # sampling frequency (Hz) # can put (100) , or can put this to be more precise (No_of_DataPoints / time) which is in my case (2159900/21600)

MAX_M = 500                 # no. of points in the curve // keep 200 dont change.

FIT_TAU_MAX = 1.0            # seconds (fit -0.5 slope on left side)
# --------------------------------------


def AllanDeviation(dataArr: np.ndarray, fs: float, maxNumM: int=100):
    """Compute the Allan deviation (sigma) of time-series data.

    Algorithm obtained from Mathworks:
    https://www.mathworks.com/help/fusion/ug/inertial-sensor-noise-analysis-using-allan-variance.html

    Args
    ----
        dataArr: 1D data array
        fs: Data sample frequency in Hz
        maxNumM: Number of output points
    
    Returns
    -------
        (taus, allanDev): Tuple of results
        taus (numpy.ndarray): Array of tau values
        allanDev (numpy.ndarray): Array of computed Allan deviations
    """
    ts = 1.0 / fs
    N = len(dataArr)
    Mmax = 2**np.floor(np.log2(N / 2))
    M = np.logspace(np.log10(1), np.log10(Mmax), num=maxNumM)
    M = np.ceil(M)  # Round up to integer
    M = np.unique(M)  # Remove duplicates
    taus = M * ts  # Compute 'cluster durations' tau

    # Compute Allan variance
    allanVar = np.zeros(len(M))
    for i, mi in enumerate(M):
        twoMi = int(2 * mi)
        mi = int(mi)
        allanVar[i] = np.sum(
            (dataArr[twoMi:N] - (2.0 * dataArr[mi:N-mi]) + dataArr[0:N-twoMi])**2
        )
    
    allanVar /= (2.0 * taus**2) * (N - (2.0 * M))
    return (taus, np.sqrt(allanVar))  # Return deviation (dev = sqrt(var))




# -------- LOAD DATA --------
data = np.genfromtxt(DATA_FILE, delimiter=',', skip_header=1)

gx = data[:, 1]   # already in dps
gy = data[:, 2]
gz = data[:, 3]

# -------- INTEGRATE RATE → ANGLE --------
ts = 1.0 / FS

theta_x = np.cumsum(gx) * ts
theta_y = np.cumsum(gy) * ts
theta_z = np.cumsum(gz) * ts

# -------- ALLAN DEVIATION --------
tau_x, ad_x = AllanDeviation(theta_x, FS, MAX_M)
tau_y, ad_y = AllanDeviation(theta_y, FS, MAX_M)
tau_z, ad_z = AllanDeviation(theta_z, FS, MAX_M)

# -------- GAUSSIAN WHITE NOISE FIT (-0.5 slope) --------
# Use only small tau region
mask = tau_x <= FIT_TAU_MAX

# Fit line in log-log space with fixed slope = -0.5
log_tau = np.log10(tau_x[mask])
log_ad  = np.log10(ad_x[mask])

# Intercept for slope = -0.5
intercept = np.mean(log_ad + 0.5 * log_tau)

# Construct reference line
tau_ref = tau_x
ad_ref = 10 ** (intercept - 0.5 * np.log10(tau_ref))


# -------- PLOT 1: ALLAN DEVIATION --------
plt.figure(figsize=(8, 6))
plt.loglog(tau_x, ad_x, label="gx")
plt.loglog(tau_y, ad_y, label="gy")
plt.loglog(tau_z, ad_z, label="gz")

plt.xlabel(r"$\tau$ [s]")
plt.ylabel("Allan Deviation [deg]")
plt.title("Gyro Allan Deviations")
plt.grid(True, which="both", ls="--", alpha=0.6)
plt.legend()
plt.tight_layout()
plt.show()


# -------- PLOT 2: GWN CHECK (-0.5 slope) --------
plt.figure(figsize=(8, 6))
plt.loglog(tau_x, ad_x, label="gx")
plt.loglog(tau_ref, ad_ref, "--", label="-0.5 slope")

plt.xlabel(r"$\tau$ [s]")
plt.ylabel("Deviation [deg]")
plt.title("Gaussian White Noise Check (−0.5 Slope)")
plt.grid(True, which="both", ls="--", alpha=0.6)
plt.legend()
plt.tight_layout()
plt.show()














