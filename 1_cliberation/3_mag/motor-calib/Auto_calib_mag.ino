///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

// so what we are doing currently  is -> using non_blocking or blocking and using py to store the data and then using Magnetor to find offsets and then use that offset to do mag calib . 

// so here we write the code - which -> moves motor and stores scaled_data points in a array,etc  and then there are parameters/ variables like - Earth_mag_field, etc what magnetor used to take. 
// and we call that Calc_mag_func() which reads these scaled data points, and parameters and gives offset and bias (or stores it in a 3x3 , 3x1 array). and then we use these values ( raw -> Scale -> apply calib using these varialbes) 
// Magneto also uses these mathematical function to calculate the offset, bias.. so why not write in code. (and we can -write this function to calc the offset of a .txt file data. and then we can check the offset of our fucntion and magnetor and see if both are same or not for same dataset.)


// Process:- 
// so basically -> call the calc_mag() function in the setup() function --- (same way like we did for 6_Auto_gyro_calib.ino - checkout)
// in that calc_mag() function it first ask the magnetic_filed value and stores it in a const_var. // it ask for magnetic filed , dont do api call stuff here. , write proper code here for user input
// and then the motor moves and stores the scaled_data in a temp_array(flush this array after used) and then finds the offset / bias using the data and magnetic field. and stored it in a const array,etc . 
// and then in the main loop -> it does raw->scale->calib-> print
// Write parameters, variables in the top: like:-  Sacling_factor_Mag , const Magnetic_field, etc....  (see the 6_Auto_gyro_calib.ino for structure)


// later - after code ready. (movement -> fast and smooth . should not take more than 1 minute. increase the yaw degree 360/8 = 47) 

// ------------- to do ----------------
// so 1st need to write a code file -> Calc_Mag_offset.cpp which takes a readings.txt file and user input-Earth_mag_field and calcultes the offset and bias of the Magnetomter . (basically replica of what Magneto does, and we can then compare the offset with magneto -using the same reading.txt file and magnetic filed - to see if our works correctly. optioinal-(and also plot and see if works and accurate)
// use the Calib_Algo folder to know about the Algorithm for calculating the offset and bias.
// then 2nd need to write a code file -> Auto_Calib_Mag.cpp in which we only give the parameter Earth_mag_field and then it moves motor and collect data points and then uses that Calc_Mag() function to get the offset and bias and then stores it in a array. and then read raw -> scale -> calib and print the calib values.

///// update -> Startup par calib nahi hoga . it uses hardcoded values for printing calib_values , and only after pressed a cmd then only it will calib and update the hardcoded values and then print it.


// same way do as 8_Auto_calib_acc.ino