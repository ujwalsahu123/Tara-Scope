// so basically - an alt az values is given from terminal, and then it calculates the rpy using sensors and S.F. (use the sf_code for this part)
// then it moves the motors to make pitch = alt and yaw = az .  (use the 2_Pointing_concept for the pointing part)
// and then when reached the target alt az , it does some laser action - or just simply points to it. and then we can do laser toggle uins spacebar or draw shape, etc or move motors , etc... ( use the 1.2_manual_laser_code for this part)
// i know that it will not point accurately since we are not using by lets try . and this code will be very usefull for star_Pointing codes . 
// before starting this code - first full figure out the sf code , and 2_pointing_concept code (like parallel move motors , or what , and etc concepts....)
// so the arduino calculates the rpy using sf code. and it Always waits for inputs - such as 
// if P is pressed in vs code terminal then it asks for alt az values and after we press enter - it send it to arduino and the arduino code points to that alt az.  and it now contineously points to that alt az until a new raading is given
// if O is pressed in vs code terminal then it goes to default mode of pointing 0,0 alt az .
// and if I is pressed then it shows the current rpy values in terminal. (avg of 10 readings, etc)
// and baki sab 1.2_manual_laser_code functions -> fast slow move, laser toggle , draw shapes , etc can be done after reaching the target alt az.
// and the alt az we will manually type it from sterellium mobile app for now. and in the  4_star_poinint we will use own libary or sterellium desktop app simply. and send that alt az values to arudino)

// so first we give a targeted ALT AZ - hardcode (alt = 0 , az = 0) 
// then A) the rpy is calculated using sensors and S.F. (use the arduino_rpy_read code)
// then B) the motor move some steps to make the pitch and yaw 0 . ( use the motor_concept)
// and this works in a loop -> A)read sensor -> calc rpy -> B)move motor by this much steps -> A) ... do this again . and thats how reach a target posi.
// and move motors like -> parallel move // or first yaw then pitch // or alternative pitch the yaw then pitch the yaw // ... think on this
// so, default it will point to 0 , 0 degrees alt az .

// Also the arduino scripts always reads the serial monitor for inputs. if there is no input then the above A)B)A)B)... 
//  but if input aya using -> injesting a Target ALT AZ value using serial monitor from a .py script.
// then the motor move to make the Pitch equal to Alt and/ yaw equal to Az. 
// and it does that in a loop A)B)A)B) and when reached the target then laser will on. 
// and it will keep pointing to that target alt az until a Particular key is not pressed "0" (default)  in the terminal. 
// and after '0' is pressed , it will again go to default mode of pointing 0,0 alt az.
// then again if a new target alt az is given from terminal , it will point to that. 

// need to write differnt functions for - 0,0 default pointing , and target alt az pointing. (since in target alt az pointing - after reaching the target - laser function is called on etc ...)


// combine Motor_concept.ino code and RPY code . 


// its not easy , since the sensor fetch data and takes time and motor movement will also take time, 
// and it's all wirtten inloop so gonna be a mess . 
//serial.begin seen etcc ....

// we can then also do like - laser toggle , or draw shapes , fade , etc. and after that action is done, the motor auto comes to the initial position using moveTo(starting) (which is basically the target alt az approx) and after that funciton is done - then the main code runs which moves the motors to the target alt az, that we gave it.





// Entire flow likh : -----------------------------------------------