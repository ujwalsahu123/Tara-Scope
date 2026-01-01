# here in this code it simply runs the calc_acc_algo() function which takes scaled_acc.txt values and local_g from user input from terminal. 
# and that function calculates the offset and bias and prints it. 
# then we can compare the magnetor ka offset and bias on that same_data and local_g with our fuction to see proper working.

# conclusion -> it is same / better than magneto offset and bias.  

# magneto calculated offset and its magnitude 
# A = np.array([[0.996560, 0.000095, -0.000602], 
#               [0.000095, 1.002121, 0.000542],
#               [-0.000602, 0.000542, 1.003814]])

# b = np.array([0.005001, -0.015234, 0.011616])
# ->0.9999997686 
 
   
# own calculated offset ka magnitude 
# A = np.array([[0.996560244, 0.0000954675174, -0.000602107644], 
#               [0.000095475174, 1.00212137, 0.000542080239],
#               [-0.000602107644, 0.000542080239, 1.00381400]])

# b = np.array([ 0.00500114, -0.01523379 , 0.01161619])
# -> 0.9999999695

# ours is better - but it dosent matter (since the difference between them is super small 10^-7)


import numpy as np

def calibrate_accelerometer(data, g=1.0):
    x = data[:, 0]
    y = data[:, 1]
    z = data[:, 2]

    D = np.column_stack([
        x*x, y*y, z*z,
        2*x*y, 2*x*z, 2*y*z,
        2*x, 2*y, 2*z,
        np.ones_like(x)
    ])

    _, _, Vt = np.linalg.svd(D, full_matrices=False)
    beta = Vt[-1, :]

    Q = np.array([
        [beta[0], beta[3], beta[4]],
        [beta[3], beta[1], beta[5]],
        [beta[4], beta[5], beta[2]]
    ])

    p = beta[6:9]
    c = beta[9]

    b = -np.linalg.inv(Q) @ p

    k = (b @ Q @ b - c)
    Qn = Q / k * (g * g)

    eigvals, eigvecs = np.linalg.eigh(Qn)
    A = eigvecs @ np.diag(np.sqrt(eigvals)) @ eigvecs.T

    return A, b


# -------------------- LOAD DATA --------------------
data = np.loadtxt("using_motor/data_g_2g_45filter.txt")
g = float(input("Enter target g (use 1.0 or local g): "))

A, b = calibrate_accelerometer(data, g)

# -------------------- PRINT FORMATTING --------------------
np.set_printoptions(suppress=True, precision=6, floatmode='fixed')

print("\n# A = np.array([")
for row in A:
    print(f"#   [{row[0]:.6f}, {row[1]:.6f}, {row[2]:.6f}],")
print("# ])")

print("\n# b = np.array(["
      f"{b[0]:.6f}, {b[1]:.6f}, {b[2]:.6f}"
      "])")

print("\n# Apply like:")
print("# acc_cal = A @ (acc_raw - b)")
