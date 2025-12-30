# write code so that i sends cmd -> j k l -> to arduino -> for calib gyro , mag , acc.  (single tab - not like press karke rakha hai to 10-20 times it send "j" .... cmd ....use the laser wala code how implmented the single tab functionality)

# 1) arduino asks for offsets and bias from the python and python sees that code and gives the offsets from offset.txt
# arduino always asks for offsets -> so write this part in setup() code. and if it fails to get or etc -> then it uses the hardcoded one -> 
# so write hardcoded offsets and bias and if it gets from python then it updates those arrays ... and uses that new one. and if it dosent get then it simply uses those hardcoded one.
# if it not able to get the data then send - not_available. and when ard sees not available it uses just prints not able to fetch offset.txt and simply dosent updates the hardcoded one - and uses that only.

# 2) it calculates the offset/ bias and updates the hardcode/offset.txt say joo mila.  and prints it.

# 3) the python wait and gets the new printed offset , bias and then stores it in offset. (and it knows ki konsa offset bias update krna hai - acc or gyro or mag kaa since we press that specific key "k" mag, "j" gyro... ) 
# if ard fails to calc the offset/bias then it prints - cant calculate , and py sees it and thus it dosent update the offse.txt and moves onn in the code .

# ard code -> will be like -> in loop() function it checks for any cmd like -> j k l . and using a switch cmd it calls that calib function....  and below that switch cmd -> the timer will be there for 104hz - and it calculates the calib values and prints it, and the py file shows that printed calib values in terminal
# 4) 

# 5) so now what happens is ardu get the latest offsets from py . and stores the latest once, and if not calc the offset then no update in offset.txt file. and if not able to get offset from offset.txt file then also no worries and the arduino uses the hardcoded once.










#  Accel calc Algo ---------------------------------------------------------------------

# def calibrate_accelerometer(data, g=1.0):
#     """
#     Robust Magneto-style accelerometer calibration.

#     data : Nx3 numpy array (scaled to g units)
#     g    : target gravity (1.0 or local g)

#     Returns:
#         A : 3x3 scale / misalignment matrix
#         b : 3x1 bias vector
#     """

#     x = data[:, 0]
#     y = data[:, 1]
#     z = data[:, 2]

#     # Design matrix
#     D = np.column_stack([
#         x*x, y*y, z*z,
#         2*x*y, 2*x*z, 2*y*z,
#         2*x, 2*y, 2*z,
#         np.ones_like(x)
#     ])

#     # Solve least squares
#     _, _, Vt = np.linalg.svd(D, full_matrices=False)
#     beta = Vt[-1, :]  # smallest singular vector

#     # Quadratic form
#     Q = np.array([
#         [beta[0], beta[3], beta[4]],
#         [beta[3], beta[1], beta[5]],
#         [beta[4], beta[5], beta[2]]
#     ])

#     p = beta[6:9]
#     c = beta[9]

#     # Bias (center of ellipsoid)
#     b = -np.linalg.inv(Q) @ p

#     # Normalize so surface maps to g
#     k = (b @ Q @ b - c)
#     Qn = Q / k * (g * g)

#     # Eigen-decomposition (SAFE)
#     eigvals, eigvecs = np.linalg.eigh(Qn)

#     # Build correction matrix
#     A = eigvecs @ np.diag(np.sqrt(eigvals)) @ eigvecs.T

#     return A, b
