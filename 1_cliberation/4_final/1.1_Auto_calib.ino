//// read the below writting and make it proper...




// This code Prints Scaled & Caliberated ACC, GYRO, MAG data 
// Acc is already calib, So It does Mag and Gyro calib after startup
// use the Manual_calib code in sf and Auto_calib code in 3_full_motor_code.


// need to write a single script for calculating gyro & mag offset/bias - where it uses motor to store the readings and then calculates the offsets, etc. 
// so what we are doing currently  is -> manually running the calib code and using py to store the data and then using Magnetor to find offsets and storing the offsets in a code file then use that offset to do gyro and calib .  , and this that....nd 
// but why to do all this much - things needs to be easy and automated. 
// for example -> At every stargazing session - the tarascope device needs to do mag & gyro calib at startup. 
// So we have these 2 scripts for gyro and mag (since they need to be calib at each startup/stargazing session) 
// So we need to write a code file -> Auto_calib.ino  which does both -> gyro and mag calib and store there bias / offset in varialbes and then raw->scale->calib_apply and print the calib. 
// so this Auto_calib.ino script will be a Live saver and we dont need to do calib manually by this that code files and storing the offset here and there.
// So when ever we connect the hardware and place it properly and Give command "calib" from python to the ard.ino code then that arudino code ka Auto_calib() function will run when we calib and store the offsets in in a global variable , and we use those variables to get raw->scale->calib->sf.
// So here we did Caliberation Abstraction - and we simply plub the device / or connect with wifi and give one single command , and it does calib by self. // this is awesome - since later at the time of 3_motor_code or 4_star_pointing then we dont have to hop on this that code to do calib and we can quicky calib and start to work on top.


// PROCESS:-
// use Auto_calib_gyro.ino & Auto_calib_mag.ino code files. 
// so basically -> call the Auto_calib() function in the setup() function / or wait wait from cmd from terminal (later, in full_motor_code.ino)
// And there is a Auto_calib() function :
// it calls these functions -> Calc_gyro() , Calc_mag().  (use the Auto_calib_mag.ino, use the Auto_calib_mag.ino)
// and store the offset values in a const array,etc . 
// SO -> acc ka offset/bias is hardcoded in a const array,etc .  and mag,gyro ka we just now found out and stored.
// and then in the main loop -> it reads Lsb values then scales it and use the const_array,etc to print the calib -> acc , gyro, mag. 
// Systematic -> Write parameters, variables in the top: like:- Scaling_factor_Acc , Sacling_factor_Gyro , Sacling_factor_Mag , const Magnetic_field, const acc_offset array , etc.... , long acc_x , long acc_y , .... etc...

// code kar ........



// after Above Code Done : UPDATE this : - 

// 1)
// we will use this Auto_calib.ino code in the 3_full_motor_code.ino (where it just takes command from the termainl using py file) so -> we can calculate the offset/bias at startup by calling the Auto_calib() function in setup() or we can give a cmd from termianl to initiale caliberation. (i think we should do both -> startup up par caliberate first, and then whenever we wanna calib we give cmd)
// for cmd initialisation -> update this part in this code -> use a if()cmd and run only when cmd = claib) (also if it asks for magnetic filed - then terminal may proper show hona chaheay, and we write in terminal then it should passes to ardu.)
// dont put this updates in this code file-(2_Auto_calib.ino) make a new code file and usme put these updates (3_Auto_&_initilaize_calib.ino)

// 2)
// first go and figure out NOAA_api.py , location_api.py seen
// we dont want to manually put the magnetic field value so lets use Api. 
// so we will put this Auto_calib.ino code in 3_full_motor_code and it works as per py cmds so 
// so what happens is - the Calc_mag() function in the 3_full_motor_code.ino/Auto_calib()_fun -> ask for magnetic field, so terminal may it should show -> and the py file see that its asking for magnetic filed - then the py code calls the NOAA api for magnetifc field data and pass it.
// and if it is not able to get then it shows -> failed to fetch magentic filed from NOAA, please enter manually:- and then we enter manually in the terminal, and then it passes that value to the arduino. 
// so lets write a small py code in (4_pass_MF.py) - which does this stuff. and run the 2_Auto_calib.ino code and check if it works or not... (if you need to update the arudino code also - then dont update here make new code file (4_get_MF.py)

// use open meteo elevation - api for getting height . and lati logi using other api . then put the lati , longi , heigh in NOAA to get the magnetic filed. (noaa ka height/elvation is wrong, use open_meteo_elevation ka api for height.)

////////// one IMP update -> dont do gyro and mag calib at startup use hardcoded values print calib values , and only when given cmd "j" then it does gyro calib , when press "h" then it does mag calib ,adn when pressed "l" it does acc calib (for now write a empty function for acc calib ) and update the hardcoded_offset and bias values and also print the new offset/ bias .  
// in 3_full_motor_code what we can do is make offset/bias.txt file in python folder and when the ardu does the new calib and finds the new offset and bias - then it gives to python and python updates it in the offset.txt file.
// and when the ard code starts then it ask python for the offset & bias values and it gives it those values form those offse.txt file -> so that we always have latest offsets stored in .txt files and can use them -> instead of using purana bad hardcoded offset bias value , and instead of again and again calibrating - (if we lost the new offset/bias values - because of arduino turned off or connection lost , etc ...)
// so all calib is done using cmd , and startup par nahi hoga koi , and uses hardcoded values to give calib value. okkkkk.....
// 


// later also integrate Acc-collectData-calcOffset-calib in this in end to end device (when selling the product time par. not now )

// ###############################################################################################
// Use the 1) ACC_Gyro_calib.ino code and 2) Auto_calib_mag.ino  code ....   and comment the hardcode_offset and bias ... and after startup it will Auto_calib gyro and mag. 
// and then we use this code in 3_full_motor_code - but over there instead of calib in the start -> it will use hardcoded offset/bias values for calibration - and only after the cmd from .py file comes then only it performes the calib of gyro and mag one by one and updates thoses hardcoded values with the new calculated offset and bias. 
// and when we press the cmd "j" the py files send to ard and the ard then only runs the Auto_calib() func and in that fun the Auto_gyro_calib() and Auto_mag_calib() functions are called one by one. and jaise hee py send calib cmd then it makes api call for getting magnetic field and stores the value. and when the arudino ask for magnetic field the it gives that value. and if it api call fails and the ard is asking for magnetic value then it propmt in terminal and ask user for manually entring the magnetic field.
// we can also later integrate the Auto_acc_calib() function -> but in that kya hoga ki there will be a seperate cmd for acc calib -> "k" . and jab man hoga tab hee acc karange. verna use hardcoded value same concept was as we done for mag and gyro hardcoded value (and update when calib done.)






/// i think we can use this cody only for sf also . just comment out the part -> where it ask python for offsets . and also dont press the j k l - when its running.  so hoga kya ki it will use the hardcoded wala.

/// use the good - hardcoded A,b vector values 




















