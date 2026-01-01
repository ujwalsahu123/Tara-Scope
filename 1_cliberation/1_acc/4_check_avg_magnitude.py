# at stable positioning - the magnitude value must me same as the g value . or closest to g. (1 , 0.978....)
# and using this code you can see the avg magnitude of the data , and by that you can 
# know if you offset values are good or bad .
# also can use 5_ardu_calib code to see magnitude on Live data.


import numpy as np

# -------------------------------
# Pretty print helpers
# -------------------------------
def print_matrix(name, M, precision=6):
    fmt = f"{{:.{precision}f}}"
    print(f"{name} = np.array([")
    for i, row in enumerate(M):
        row_str = ", ".join(fmt.format(v) for v in row)
        if i < len(M) - 1:
            print(f"    [{row_str}],")
        else:
            print(f"    [{row_str}]")
    print("])")

def print_vector(name, v, precision=6):
    fmt = f"{{:.{precision}f}}"
    vec_str = ", ".join(fmt.format(x) for x in v)
    print(f"{name} = np.array([{vec_str}])")

# -------------------------------
# Magneto-style accel calibration
# -------------------------------
def calibrate_accelerometer(data, g=1.0):
    """
    Robust Magneto-style accelerometer calibration.

    data : Nx3 numpy array (scaled to g units)
    g    : target gravity (1.0 or local g)

    Returns:
        A : 3x3 scale / misalignment matrix
        b : 3x1 bias vector
    """

    x, y, z = data[:, 0], data[:, 1], data[:, 2]

    # Design matrix
    D = np.column_stack([
        x*x, y*y, z*z,
        2*x*y, 2*x*z, 2*y*z,
        2*x, 2*y, 2*z,
        np.ones_like(x)
    ])

    # Solve ellipsoid via SVD
    _, _, Vt = np.linalg.svd(D, full_matrices=False)
    beta = Vt[-1, :]

    # Quadratic form
    Q = np.array([
        [beta[0], beta[3], beta[4]],
        [beta[3], beta[1], beta[5]],
        [beta[4], beta[5], beta[2]]
    ])

    p = beta[6:9]
    c = beta[9]

    # Bias
    b = -np.linalg.inv(Q) @ p

    # Normalize to gravity
    k = (b @ Q @ b - c)
    Qn = Q / k * (g * g)

    # Safe eigen decomposition
    eigvals, eigvecs = np.linalg.eigh(Qn)
    A = eigvecs @ np.diag(np.sqrt(eigvals)) @ eigvecs.T

    return A, b

# -------------------------------
# Apply calibration + magnitude
# -------------------------------
def calibrated_avg_magnitude(data, A, b):
    mags = []
    for v in data:
        v_cal = A @ (v - b)
        mags.append(np.linalg.norm(v_cal))
    return np.mean(mags)

# -------------------------------
# MAIN
# -------------------------------
if __name__ == "__main__":

##################################################################################
    file_name = "using_motor/data_g_2g_100filter.txt"  # give it scaled data in g . not lsb.
    data = np.loadtxt(file_name)

    g = float(input("Enter target g (use 1.0 or local g): "))

    A, b = calibrate_accelerometer(data, g)

    avg_mag = calibrated_avg_magnitude(data, A, b)

    # print("\n=== ACCEL CALIBRATION RESULTS ===\n")

# -------------------- PRINT FORMATTING --------------------
    np.set_printoptions(suppress=True, precision=6, floatmode='fixed')

    print(f"\nAverage calibrated magnitude: {avg_mag:.10f}")
    Error = avg_mag - g
    print(f"Error: between Magnitude and g :  {Error} " )

    print("\n# A = np.array([")
    for row in A:
        print(f"#   [{row[0]:.8f}, {row[1]:.8f}, {row[2]:.8f}],")
    print("# ])")

    print("\n# b = np.array(["
          f"{b[0]:.8f}, {b[1]:.8f}, {b[2]:.8f}"
          "])\n")


