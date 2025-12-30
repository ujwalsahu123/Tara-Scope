

# here in this code it simply runs the calc_acc_algo() function which takes scaled_acc.txt values and local_g from user input from terminal. 
# and that function calculates the offset and bias and prints it. 
# then we can compare the magnetor ka offset and bias on that same_data and local_g with our fuction to see proper working.

# conclusion -> it is same / better than magneto offset and bias.  


import numpy as np

def calibrate_accelerometer(data, g=1.0):
    """
    Robust Magneto-style accelerometer calibration.

    data : Nx3 numpy array (scaled to g units)
    g    : target gravity (1.0 or local g)

    Returns:
        A : 3x3 scale / misalignment matrix
        b : 3x1 bias vector
    """

    x = data[:, 0]
    y = data[:, 1]
    z = data[:, 2]

    # Design matrix
    D = np.column_stack([
        x*x, y*y, z*z,
        2*x*y, 2*x*z, 2*y*z,
        2*x, 2*y, 2*z,
        np.ones_like(x)
    ])

    # Solve least squares
    _, _, Vt = np.linalg.svd(D, full_matrices=False)
    beta = Vt[-1, :]  # smallest singular vector

    # Quadratic form
    Q = np.array([
        [beta[0], beta[3], beta[4]],
        [beta[3], beta[1], beta[5]],
        [beta[4], beta[5], beta[2]]
    ])

    p = beta[6:9]
    c = beta[9]

    # Bias (center of ellipsoid)
    b = -np.linalg.inv(Q) @ p

    # Normalize so surface maps to g
    k = (b @ Q @ b - c)
    Qn = Q / k * (g * g)

    # Eigen-decomposition (SAFE)
    eigvals, eigvecs = np.linalg.eigh(Qn)

    # Build correction matrix
    A = eigvecs @ np.diag(np.sqrt(eigvals)) @ eigvecs.T

    return A, b


data = np.loadtxt("data_lsb_2g_100filter.txt")  # Nx3, already in g
g = float(input("Enter target g (use 1.0 or local g): "))

A, b = calibrate_accelerometer(data, g)

print("\n=== ACCEL CALIBRATION RESULTS ===")
print("Bias (offset):")
print(b)

print("\nScale / misalignment matrix:")
print(A)

print("\nApply like:")
print("acc_cal = A @ (acc_raw - b)")
