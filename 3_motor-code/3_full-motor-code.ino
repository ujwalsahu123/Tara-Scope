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



// Entire flow likh : -----------------------------------------------