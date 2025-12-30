# xtra code for scaling. not imp.


# reads from one .txt file (raw lsb) and makes a new file with scaled value in another .txt file
# data_g.txt may raw (lsb) data hai. - and its in float since avg kia hai of 25 readings so thats why in float. 
# and using that raw lsb data we can scale to g , mg .
# make sure that - Lsb_ka_g and scaling_ka_g are same .  ex- 2g_lsb * 2g_scaling(4/2^16)

# data_g_2g_100filter.txt -> matlab data is scaled to g as per 2g , and filter is 100
# data_lsb_2g_100filter.txt -> matlab data is in lsb as per 2g , and filter is 100.


INPUT_FILE  = "data_lsb.txt"
OUTPUT_FILE = "scaled_mg.txt"

SCALE = 0.06103515625  # mg per LSB for ±2g

with open(INPUT_FILE, "r") as fin, open(OUTPUT_FILE, "w") as fout:
    for line in fin:
        line = line.strip()
        if not line:
            continue

        x_lsb, y_lsb, z_lsb = map(float, line.split())

        x_mg = x_lsb * SCALE
        y_mg = y_lsb * SCALE
        z_mg = z_lsb * SCALE

        fout.write(f"{x_mg:.10f}\t{y_mg:.10f}\t{z_mg:.10f}\n")

print("Done. Scaled data written to", OUTPUT_FILE)
