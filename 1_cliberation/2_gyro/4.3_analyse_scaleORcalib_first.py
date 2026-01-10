# what to do ? 
# // raw->scale->calib // or // raw->calib->scale //
# which is better or have less noise binary noise
# do  raw -> bias -> scale


# ===== Bias comparison (first 1000 samples) =====

# Method 1: raw -> bias -> scale
# Bias (LSB):  bx=44.674000, by=-169.227000, bz=72.275000
# Bias (DPS):  bx=0.195448750, by=-0.740368125, bz=0.316203125

# Method 2: raw -> scale -> bias
# Bias (DPS):  bx=0.195448750, by=-0.740368125, bz=0.316203125

# Difference (Method2 - Method1) [DPS]:
# Δbx=0.000000000000
# Δby=0.000000000000
# Δbz=0.000000000000

# so both are same . 


# so when we do -> Raw(lsb) ka Avg to find bias(lsb) and then scaled the bias(dps). 
# or we do -> raw * scale(dps) and then find bias in (dps) .
# both are same .
# so if a new data points come then if we do scale(dps) then  scale(dps) - bias(dps) or do raw(lsb) - bias(lsb) then scale to dps both will be same.

# but option 1) is better since - the scaling factor 0.004375 has binary representation error
# so when we do raw(lsb) - bias(lsb) then there is no error , and then we scale(dps) the error aya small. 
# but if we do raw(dps) - bias(dps) so now raw also has error and bias also has error.. since dono scaled hai and when scaled they have error.
# so better is option 1).

 


import numpy as np

# ---------------- CONFIG ----------------
DATA_FILE = "Analysis_big_data/data_test.txt"   # t_ms,gx_lsb,gy_lsb,gz_lsb
N_SAMPLES = 1000
GYRO_SCALE_125DPS = 0.004375  # dps / LSB
# ----------------------------------------


# ---------- LOAD DATA ----------
data = np.genfromtxt(DATA_FILE, delimiter=',', skip_header=1)

gx_lsb = data[:N_SAMPLES, 1]
gy_lsb = data[:N_SAMPLES, 2]
gz_lsb = data[:N_SAMPLES, 3]


# =========================================================
# METHOD 1: raw -> bias -> scale
# =========================================================
def bias_then_scale(gx, gy, gz, scale):
    bx_lsb = np.mean(gx)
    by_lsb = np.mean(gy)
    bz_lsb = np.mean(gz)

    bx_dps = bx_lsb * scale
    by_dps = by_lsb * scale
    bz_dps = bz_lsb * scale

    return (bx_lsb, by_lsb, bz_lsb), (bx_dps, by_dps, bz_dps)


# =========================================================
# METHOD 2: raw -> scale -> bias
# =========================================================
def scale_then_bias(gx, gy, gz, scale):
    gx_dps = gx * scale
    gy_dps = gy * scale
    gz_dps = gz * scale

    bx_dps = np.mean(gx_dps)
    by_dps = np.mean(gy_dps)
    bz_dps = np.mean(gz_dps)

    return (bx_dps, by_dps, bz_dps)


# ---------- RUN BOTH METHODS ----------
(bias_lsb, bias1_dps) = bias_then_scale(gx_lsb, gy_lsb, gz_lsb, GYRO_SCALE_125DPS)
bias2_dps = scale_then_bias(gx_lsb, gy_lsb, gz_lsb, GYRO_SCALE_125DPS)


# ---------- PRINT RESULTS ----------
print("\n===== Bias comparison (first 1000 samples) =====\n")

print("Method 1: raw -> bias -> scale")
print(f"Bias (LSB):  bx={bias_lsb[0]:.6f}, by={bias_lsb[1]:.6f}, bz={bias_lsb[2]:.6f}")
print(f"Bias (DPS):  bx={bias1_dps[0]:.9f}, by={bias1_dps[1]:.9f}, bz={bias1_dps[2]:.9f}\n")

print("Method 2: raw -> scale -> bias")
print(f"Bias (DPS):  bx={bias2_dps[0]:.9f}, by={bias2_dps[1]:.9f}, bz={bias2_dps[2]:.9f}\n")

print("Difference (Method2 - Method1) [DPS]:")
print(f"Δbx={bias2_dps[0]-bias1_dps[0]:.12f}")
print(f"Δby={bias2_dps[1]-bias1_dps[1]:.12f}")
print(f"Δbz={bias2_dps[2]-bias1_dps[2]:.12f}")
