# Here you can scale raw LSB data to DPS (multiply)    or    DPS to LSB data (divide). 
# as per the particular range ka scaling factor 125dps(0.004375) 250dps(0.00875) , etc..)






# Here We Scale gyro LSB data to dps and save to a new file (use proper scaling factor as per the LSB_range your data is)
INPUT_FILE  = "Analysis_big_data/data_250hz_nofilter_125dps.txt"        # input file (LSB)
OUTPUT_FILE = "Analysis_big_data/250hz_nofilter_125dps.txt"        # output file (dps)
SENSITIVITY_DPS_PER_LSB = 0.004375  # ---- GYRO SENSITIVITY  ----   ##########################################
# For reference:
# ±125 dps → 0.004375
# ±250 dps → 0.00875
# ±500 dps → 0.0175
# ±1000 dps → 0.035
# -------------------------------------

def main():
    with open(INPUT_FILE, "r") as fin, open(OUTPUT_FILE, "w") as fout:

        header = fin.readline().strip()
        fout.write("t_ms,gx_dps,gy_dps,gz_dps\n")

        for line in fin:
            line = line.strip()
            if not line:
                continue

            parts = line.split(",")
            if len(parts) != 4:
                continue

            t_ms = parts[0]
            gx_lsb = float(parts[1])
            gy_lsb = float(parts[2])
            gz_lsb = float(parts[3])

            gx_dps = gx_lsb * SENSITIVITY_DPS_PER_LSB
            gy_dps = gy_lsb * SENSITIVITY_DPS_PER_LSB
            gz_dps = gz_lsb * SENSITIVITY_DPS_PER_LSB

            fout.write(f"{t_ms},{gx_dps:.7f},{gy_dps:.7f},{gz_dps:.7f}\n")  # 7 decimals are enough and the values dont go beyon 6 decimals . so 7 is fine. 

    print("Scaling complete.")
    print(f"Input : {INPUT_FILE}")
    print(f"Output: {OUTPUT_FILE}")


if __name__ == "__main__":
    main()






