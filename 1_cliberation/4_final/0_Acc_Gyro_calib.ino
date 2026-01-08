


////////  first update the 5_ardu_calib_acc code 6_Auto_calib_gyro code  as per the 1_basic.ino
//// then mix both of that code here.  
// keep all the comments of 1_basic.ino 
// keep all the comments of 6_Auto_calib_gyro and 8_auto_calib_acc


// after code is ready -> put the code in 1.1_Auto_calib.ino   and  delete this file.
// and Quickly start working on -> the 1.2_Auto_calib.py -> but just for acc and gyro calib (since 1.1 may bhi sift acc and gyro hee hai) . 
// (later mix Mag_auto_calib.ino in 1.1 and 1.2)
// to get local_g use the api.....


// here also add the bmp wala part in the acc_calib function -> where - like it send the acc_data then it also send the Height/altitude (in meter) to python ...
// use the height.ino code.

// also add the Grav_magfiled() code here when "h" is pressed. <- in the pyton code 1.2_Auto_calib


// First read both data -acc , gyro then one by one each calib. 
// Don't do read&calib acc then read&calib gyro