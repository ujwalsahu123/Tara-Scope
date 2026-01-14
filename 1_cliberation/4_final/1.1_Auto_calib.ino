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






//// mix 0_Acc_gyro_code and Auto_calib_mag code ... but also refer the 1_calib/0_sensor_basic/full_code.ino since usme format is there how to use ISM and MMC together using sensor hub , etc...























































// ok now what i wanna do is -> i wanna write a py code file -> from which we send commands 
// "j" gyro calib , "l" accel calib , "h" for calculating local_g , magnetic filed. 
// ok so in the start setup() function the arduino asks python for gyro_bias value , accel_offset and bias value , and then python gives it those values from the offset.txt where it is stored. 
// and if python is able to send those values then arudino and arduino catches it and updates the hardcoded values - and also prints "using offset.txt values" . and if python is not able to get the data from offset.txt then it sends - couldnt found - and then arduino checks this and then prints "using hardcoded values" and dosent updates the hardcoded values.
// if stores all the offset/bias in a single txt file is hard then we can make seperate txt files for each accel , gyro..


// ok so when i press j then its does gyro calib and then form the gyro_calib function it sends the bias vector to the python and then python updates it in a offsets.txt file .

// when i press h then it ask the user for lati , longi , height . then uses the formula and api to get the local_g and magnetic field and then stores that calculated values in offsets.txt  ### give it code file

// when i press l then the accel calib happens - and then arduino prints values , and python reads those values and scales it and uses the local_g from the offsets.txt and then uses the function to calc the bias and offset using the collected value and local_g . and then updates the offset/bias in offset.txt and also sends it to arudino and then arudino catches it updates the hardcoded values..












// GYRO INFO ----------------------------------------------------------------------------------------------

// Gyro calib is required after every startup. since the bias is dependent on temp, etc 

// so we write a calc_gyro_bias() function which collects data points and find bias - and then we can use that bias to get calib_output. 
// so we can either directly call this calc_gyro_bias() function in the setup() so as it startups - it calculates. 
// or we can call it when the user gives cmd. 
// both are fine - but using user cmd is better - so that user when ever wants can calib .... 

// process: 
// so the code prints the calib values contineously . 
// & since calib is as per user cmd - so jabtak user calib naa kar tab tak - bekar unclib values print nahi hone chaheay ->
// so initally we use the hardcoded bias values (not accurate but okayish) for getting the calib_value. 
// and when we press "j" then it calls the calc_gyro_bias() function - and updates that hardcoded with the new one ...and print the new one...
// in calc_gyro_bias() function it first Reads raw values for 1000 readings 
// and then Calculates the bias(lsb) that biass and updates that hardcodecd bias array
// then in main loop we do raw_lsb => calib using bias_lsb (hardcoded/updated) => scale to dps
 
// Code Process : 
// call the calc_gyro() function when cmd "j" is pressed
// in that function read data - calculate the bias - and update the hardcoded bias.
// then in main loop simply do calib and print.  raw->calib->scale.

// We can do any  // raw->scale->calib // or // raw->calib->scale // both are same but caliberating first (-bias_lsb) is better since less binary_representation noise.
// if you want scale first -> the code is also there down below. *******code_2_scale_first******
// but use code_1. its Better.


// --------------
// at rest the values should be 0 for all.
// 1000 samples read (dosent matter how many hz you keep)
// keep the sensor STILL at calib time.

// ------Result--------
// after code run ->  calib is done fine- so the vales are closer to 0. 
// but since the gyro is very noisy thats why values seems to see shaky - but they are very small noise 
// ex:- 
// x,y,z
// 0.006952, 0.054626, 0.047337
// -0.023673, 0.032751, 0.012337
// so values are closer to 0, but super noisy, but the noise is very small +-0.0x , so max 0.1 to 0.02 degree error at Rest position.


// i tested this works and it uses hardcoded bias intially and updated bias later after calibration.


// As we know that bias drift as per time, so if we calculate the bias at differnt time -> startup time , 20 seconds after the startup then we will get differnt bias. 
// so if we calc the bias after some time of startup -> thus we get drifted bias ...
// but sensor fusion takes care of the bias drift (using mag, acc) , and we do gyro bias calculation at the startup or later is fine .
// and a stable calibration at whatever time is fine, (but you will think that latest calib will give latest bias and it will be more accurate ... 
// so yes, ✅ Calibrating slightly later (after warm-up) usually gives slightly better / more stable bias (after heat up, it stays at stable heat)
// and we know that Bias drift never fully stops -> so we do NOT need to recalibrate gyro bias frequently., since sf takes cares of drift.
// so Thus -> 1-stable calib of bias after 1 miniute of statup is enough.. and after that sf will take care of drift. 
// & for more precise you can do after every 15 min... but not sure if it imporves.. or its unecessay (very small improvement)
// ex:-
// Startup bias:
// bx = 58.77
// by = -184.35
// bz = 62.86
// After 1 min:
// bx = 56.72   (-2.05)
// by = -183.29 (+1.06)
// bz = 63.93   (+1.07)


// so do calib 30 sec after the startup , just calib for 1000 values not more. , do calib every 10 min...(later figure out time interval)
// use code_1 its better.

// Collect_data_lsb => find Bias_lsb
// Raw_lsb - bias_lsb => calib_lsb
// calib_lsb * scaling_factor => calib_dps





// Accel INFO ----------------------------------------------------------------------------------------------

// we can do Accel Auto Calculate the offset/bias using data points & local_g (instead of using magento) 
// so that no need to Run 3 differnt files and use magneto then store offset/bias here and there. 
// We just simply press a Cmd "L" and then it uses the 2 motors to move the sensor and collect the data at different orientaitons
// and then send that data to python where it uses the collected data and local_g to calculate the offset/bias and it also stores it in offset.txt file
// and then send that offset/bias to arduino , so that the arduino gives calibrated values as per the new offset/bias we calculated.
// put the device in sleeping position before calib (so that sensors can move 3 axis rotation)
// then stand up the device and do gyro calib , mag calib - and then dont touch the device.

// NOTES:
// -> at differnt gravity /place its better to calcualte new data and then use that data with the local_g of that place.
// (No need to do Accel Calib at every startup but if the device is in at new location than previously acc_calib location ) 
// Also later think on this -> Calib karna too accha hai , but need to think ki improvement hoga issay ya nahi (need to check this) since noise is way bigger than these small accuracy jugad we do -(calib as per local g.)
// Also later think -> if the main thing we are doing is calib as per the local_g then can we use the previous data collected and just calcuate the offset/bias as per that previously_collected data and the current location ka local_g.
// we are not doing calc offset/bias here since its very heavy computation and it needs numpy library.


// PROCESS : 
// apply caliberation on live data - and see the xyz and Magnitude . 
// by seeing the magnitude you can judge if the offset are good or bad as per (magnitude close to 1).
// so there is hardcoded offset prewritten and it also ask for the latest offsets and bias from python form offset.txt
// also auto calib is there when presses "l"
// here we use motor to print the data.
// it prints - average of 25 values (we are doing averaging here only in arduino, and not in py)
// and then in py - it just reads and scales the value and then calculates the offset and bias over there and then store it in offset.txt and also send it to arduino 
// and arduino updates the offset and bias and give calib_values as per that new only.


// # For Accel dont do Raw->calib->Scale , do Raw->scale-> calib. (since when you do Raw-> calib-> scale then precision is lost since the lsb values are super big, so offset are like 0.000061)












#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"
#include <CheapStepper.h>

SparkFun_ISM330DHCX myISM;
sfe_ism_raw_data_t rawAccel;
sfe_ism_raw_data_t rawGyro;

CheapStepper motorAlt(4, 5, 6, 7);
CheapStepper motorAz(8, 9, 10, 11);

// IMP - so that loop run as per the data rate of sensor and not try to run as fast as possible 
unsigned int DataRate_HZ;	
unsigned long PERIOD_US;          
// time tracker for loops -> mainLoop(), while() , for()     
unsigned long lastRead = 0; 
unsigned long now = 0;

// sending n decimal data to python   // UNO float can only handle till 6 decimals (more than this has representation error) 
int n_gyro_bias = 6; // do 6 in uno, 8 in esp
int n_acc_raw = 4; // do 2-4 in uno , 4-6 in esp  (32343.123...)
int n_mag_raw ; // later set...
// for accel , mag ka offset/bias ka decimal -> go in the python and update from there -> because python hee calc offset/bias and then it stores in offset.txt


// #######################   CONFIG GYRO   ####################### 
#define GYRO_Scale_DPS   0.004375f      // scaling to 125 dps
#define GYRO_CALIB_SAMPLES  1000         // number of samples to collect for bias calculation
double gyro_bias[3] = { 44.295409, -169.449102, 72.213573}; // // Hardcode Gyro bias_lsb, not accurate
// ##########################################################



// #######################   CONFIG ACCEL  ####################### 
#define ACC_LSB_TO_G 0.00006103515625f   // 2g range
#define AVG_MEAS  25 // take Avg of N readings to get 1 stable reading /per orientation.

const int STEPS_PER_REV = 4096;
// const int N_orientations = 64 ;

const int N_orientations = 4 ;

//SET THIS //(value must be -> 2^N). [VALUES -> (1 = 360Deg) (2 = 180Deg) (4 = 90Deg) (8 = 45Deg) (16 = 22.5Deg) (32 = 11.25Deg) (64 = 5.625Deg) (128 = 2.8125Deg) (256 = 1.4Deg)]
const int STEPS_to_move_per_orientation = (STEPS_PER_REV / N_orientations) ;
// So if 4096/1_orientation => so 4096_steps_per_orientation to complete 1 full rotation. if 4096/2_orientaitons => so 2048_step)per_orientation and it will take 2 orientatins to complete 1 full rotation.      

// Hardcoded offset and bias - (g_2g_Nofilter_0.97859) 
// change it as per filter you use.
// 1 time Calib when in new Location.

double A_acc[3][3] = 
{{1.0, 1.0, 1.0},
 {1.0, 1.0, 1.0},
 {1.0, 1.0, 1.0}};
double b_acc[3] = { 1.0, 1.0, 1.0}; 

// double A_acc[3][3] = 
// {{0.97522389, 0.00009342, -0.00058922},
//  {0.00009342, 0.98066595, 0.00053047},
//  {-0.00058922, 0.00053047, 0.98232234}};
// double b_acc[3] = { 0.00500114, -0.01523379, 0.01161619}; 
// ################################################################



double rawAcc[3] =  {0.0, 0.0, 0.0};
double calibAccel[3] = {0.0, 0.0,0.0};
double calibGyro[3] = {0.0,0.0,0.0} ;
long count = 0; // iterator for Average readings



// =======================================================
// CALCULATE GYRO BIAS 
// =======================================================
long gX = 0, gY = 0, gZ = 0; // for storing Average readings

void calc_gyro_bias()
{

  delay(100); // # 100 milliseconds dealy IMP, keep 100 only.
  // Nothing printing here for handshake,since no need - we gonna directly send the data to python.

  // calculate the bias - by readings 1000 values and doing avg of that.
  gX = 0; gY = 0; gZ = 0;
  count = 0;
  lastRead = 0;

  Serial.println("Keep Sensor STILL for Gyro calibration ......");
  delay(1000); // wait 

  // Run loop for to collect 1000 samples
  while (count < GYRO_CALIB_SAMPLES) 
  {
    // Run loop as per Sensor Hz and not Run as fast as possible.
    // Dont worry it will Collect all 1000 samples, since the condition is as per count variable, and count variable only increments at each reading
    now = micros();
    if (now - lastRead >= PERIOD_US)
    {
      lastRead = now;

      if (myISM.getRawGyro(&rawGyro))
      {
        gX += rawGyro.xData;
        gY += rawGyro.yData;
        gZ += rawGyro.zData;
        count++;
      }
    }
  }

  // retset after use.
  lastRead = 0;


  // calculated the bias(lsb) and Update the Hardcoded gyro_bias[]
  gyro_bias[0] = (float)gX / count;
  gyro_bias[1] = (float)gY / count;
  gyro_bias[2] = (float)gZ / count;


    // ---- SEND GYRO BIAS TO PYTHON ----
    Serial.print("GYRO_BIAS,");                             
    Serial.print(gyro_bias[0], n_gyro_bias); Serial.print(",");     // sending n_gyro_bias decimals to python
    Serial.print(gyro_bias[1], n_gyro_bias); Serial.print(",");
    Serial.println(gyro_bias[2], n_gyro_bias);


  delay(500); // not necessary but good.

}


// =======================================================
// CALCULATE ACCEL OFFSET/BIAS USING MOTOR
// =======================================================
double ax = 0, ay = 0, az = 0; // for storing Average readings

// function to calculte the avg readings and print it.
void Read_Accel_data()
{
    // take 25 readings ka AVG - for single orientation.
    lastRead = 0;
    count = 0; // make iterator 0 before staring.
    ax = 0; ay = 0; az = 0; // Make Avg calculation 0 - before storing new avg calculations.
    while (count < AVG_MEAS)  // take Multiple readings ka AVG - for single orientation - at particular Hz.
    {
        now = micros();
        if (now - lastRead >= PERIOD_US)
        {
            lastRead = now;
            if(myISM.getRawAccel(&rawAccel))
            {
                ax += rawAccel.xData;
                ay += rawAccel.yData;
                az += rawAccel.zData;
                count++;
            }
        }
    }
    
    lastRead = 0;


    // find AVG
    ax /= AVG_MEAS;
    ay /= AVG_MEAS;
    az /= AVG_MEAS;

    // Send AVG RAW LSB ONLY , and in python it will scale
    Serial.print(ax, n_acc_raw); Serial.print(",");   // sending n decimals 
    Serial.print(ay, n_acc_raw); Serial.print(",");
    Serial.println(az, n_acc_raw);

    }

// =======================================================
// Function which uses 2_motors to collecte the Acc data at differnt orientations
// =======================================================

void calc_acc_offset()
{ 
  // cannot do calculation of ACC offset and bias over here .
  // so we simply print the raw values and python collects these datapoint and scales it then calculate the offset and bias and stores it in offset.txt and also sends it here to update the harcoded values.
 
  delay(100); // # 100 milliseconds dealy IMP, keep 100 only.
  Serial.println("ACC_RAW_DATA_SHARING_START"); // for handshake - to tell python that collect the data
  
  delay(1000); // for stabilization // keep

      //////////////////////////// MOVEMENT 1 -> Alt motor full rotation - from Initial orientation
      for (int i = 0; i < N_orientations ; i++)  // take reading for full circle - 
      { 
        if(i == 0)
        {
          delay(2000); // keep
          Read_Accel_data(); // initail read before moving the motor (since it moves the motor first and then reads)
        }

        // first Motor-move and then take readings.
        delay(200); // keep
        motorAlt.move(true, STEPS_to_move_per_orientation); // move to next orientation of current movement.
        delay(2000); // 2 sec delay to stabalize the vibration, after motor has moved to the new position.
        Read_Accel_data();
      }

      delay(1000);
      motorAlt.move(false, STEPS_PER_REV ); // return to inital position of Movement 1


      ////////////////////////// Movement 2 -> AZ motor full rotation - From initial orientation. 
      for (int i = 0; i < N_orientations ; i++)  
      { 
        if(i == 0)
        {
          delay(2000); // keep
          Read_Accel_data(); 
        }
        
        delay(200); // keep
        motorAz.move(false, STEPS_to_move_per_orientation); // move to next orientation of current movement. using AZ motor .
        delay(2000); // 2 sec delay to stabalize the vibration, after motor has moved to the new position.
        Read_Accel_data();
      }

      // no need to return to initial posi now, we first go to movement3 starting position.
      delay(1000);
      motorAlt.move(true, (STEPS_PER_REV/4)); // Rotate alt motor +90 deg.

      ////////////////////////////////// Movement 3 -> Rotate alt motor by 90 deg or 4096/4, then AZ motor full rotation in opposite direction this time.
      for (int i = 0; i < N_orientations ; i++) 
      { 
        if(i == 0)
        {
          delay(2000); // keep
          Read_Accel_data(); 
        }

        delay(200); // keep
        motorAz.move(true, STEPS_to_move_per_orientation); // move to next orientation of current movement. Using AZ motor (opposite this time)
        delay(2000); 
        Read_Accel_data();
      }
      delay(1000); // keep
      motorAlt.move(false, (STEPS_PER_REV/4)); // move the alt motor back to -90 degree (back to inital orientation)
      // So now we are OG back to OG inital orientation.
      delay(100); // keep
      Serial.println("ACC_RAW_DATA_SHARING_DONE"); // finished sending raw accel data to Python
  



      // ---- HANDSHAKE ----
      while(!Serial.available())         // wait for python to send the msg that it is sending updated offset/bias values.
      { 
        dealyt(100); // keep // let python write the data first before arduino reads.

        // read single line from python
        String msg = Serial.readStringUntil('\n');
        msg.trim();
        
        if(msg == "REQUEST_FOR_SENDING_UPDATED_VALUES")
        { 
          Serial.println("READY_FOR_UPDATED_VALUES"); 
          break; // exit the while loop
        }
        else if(msg == "ACCEL_CALIB_FAILED")
        {
          Serial.println("ERROR_GETTING_UPDATED_VALUES"); // something went wrong
          return; // exit the function - without updating the hardcoded values.
        }
        else{
          Serial.println(msg); // for debugging
          return; // exit the function - without updating the hardcoded values.
        }
      }

// ---------------------- get new Accel offset/bias from python and update the  hardcoded once --------------------------
      
      // ---- ACC BIAS ----
      delay(100); // Imp, let python first read that data sharing is done, and let it store the values in offset.txt then it sends the values ,, and after that only arduino should read it..
      String line = Serial.readStringUntil('\n');
      line.trim();  // "ACC_B,0.0045,-0.0148,0.0123"

      if (line.startsWith("ACC_B"))
      {
        int p1 = line.indexOf(',');
        int p2 = line.indexOf(',', p1 + 1);
        int p3 = line.indexOf(',', p2 + 1);
  
        b_acc[0] = line.substring(p1 + 1, p2).toFloat();
        b_acc[1] = line.substring(p2 + 1, p3).toFloat();
        b_acc[2] = line.substring(p3 + 1).toFloat();
      }

      // ---- ACC MATRIX ----
      Serial.readStringUntil('\n'); // consumes "ACC_A"

      for (int i = 0; i < 3; i++)
      {
        line = Serial.readStringUntil('\n');
        line.trim();
  
        int p1 = line.indexOf(',');
        int p2 = line.indexOf(',', p1 + 1);
  
        A_acc[i][0] = line.substring(0, p1).toFloat();
        A_acc[i][1] = line.substring(p1 + 1, p2).toFloat();
        A_acc[i][2] = line.substring(p2 + 1).toFloat();
      }


  Serial.println(b_acc[2],6 );
  Serial.println(A_acc[0][0],6 );
  Serial.println(A_acc[0][1],6 );
  Serial.println(A_acc[0][2],6 );
  Serial.println(A_acc[1][0],6 );
  Serial.println(A_acc[1][1],6 );
  Serial.println(A_acc[1][2],6 );
  Serial.println(A_acc[2][0],6 );
  Serial.println(A_acc[2][1],6 );
  Serial.println(A_acc[2][2],6 );
      


      // ---- END ----
      line = Serial.readStringUntil('\n');
      line.trim();

      if(line == "END")
      {
        Serial.println("ACC_UPDATED"); // for handshake -> that we got the offset/bias from python and updated the 
      }
      else{
      Serial.println("Error");  // Python dosent print END after sending values... or etc error
    }

  delay(500); // keep

}


// =======================================================
// GET INITIAL OFFSET/BIAS FROM PYTHON
// =======================================================


void requestOffsetsFromPython()
{
  delay(100); // # 100 milliseconds dealy IMP, keep 100 only.
  Serial.println("READY_FOR_OFFSETS");
  
  unsigned long t0 = millis();
  while (!Serial.available())
  {
    if (millis() - t0 > 5000) // wait for 5 second for python to give the offsets...
    {
      Serial.println("Using hardcoded values (timeout)");
      return;
    }
  }

//  can put a handshake here ... ...
// then delay

delay(500); // imp, let python send the values fully before the arduino reads.. (other wise arduino will read first before python sending)

// ---- GYRO ----
String line = Serial.readStringUntil('\n');
line.trim();  // "GYRO,28.58,-157.54,78.52"

if (line.startsWith("GYRO"))
{
  int p1 = line.indexOf(',');
  int p2 = line.indexOf(',', p1 + 1);
  int p3 = line.indexOf(',', p2 + 1);

  gyro_bias[0] = line.substring(p1 + 1, p2).toFloat();
  gyro_bias[1] = line.substring(p2 + 1, p3).toFloat();
  gyro_bias[2] = line.substring(p3 + 1).toFloat();
}

// ---- ACC BIAS ----
line = Serial.readStringUntil('\n');
line.trim();  // "ACC_B,0.0045,-0.0148,0.0123"

if (line.startsWith("ACC_B"))
{
  int p1 = line.indexOf(',');
  int p2 = line.indexOf(',', p1 + 1);
  int p3 = line.indexOf(',', p2 + 1);
  
  b_acc[0] = line.substring(p1 + 1, p2).toFloat();
  b_acc[1] = line.substring(p2 + 1, p3).toFloat();
  b_acc[2] = line.substring(p3 + 1).toFloat();
}

// ---- ACC MATRIX ----
Serial.readStringUntil('\n'); // consumes "ACC_A"

for (int i = 0; i < 3; i++)
{
  line = Serial.readStringUntil('\n');
  line.trim();
  
  int p1 = line.indexOf(',');
  int p2 = line.indexOf(',', p1 + 1);
  
  A_acc[i][0] = line.substring(0, p1).toFloat();
  A_acc[i][1] = line.substring(p1 + 1, p2).toFloat();
  A_acc[i][2] = line.substring(p2 + 1).toFloat();
}


line = Serial.readStringUntil('\n');   // reads "END" // for safety
line.trim();
 

// // I tested , it actally updates the offsets as per the offset.txt values
Serial.println(gyro_bias[0],6);
Serial.println(gyro_bias[1],6);
Serial.println(gyro_bias[2],6);
Serial.println(b_acc[0],6 );
Serial.println(b_acc[1],6 );
Serial.println(b_acc[2],6 );
Serial.println(A_acc[0][0],6 );
Serial.println(A_acc[0][1],6 );
Serial.println(A_acc[0][2],6 );
Serial.println(A_acc[1][0],6 );
Serial.println(A_acc[1][1],6 );
Serial.println(A_acc[1][2],6 );
Serial.println(A_acc[2][0],6 );
Serial.println(A_acc[2][1],6 );
Serial.println(A_acc[2][2],6 );
  
  
  if(line == "END")
  {
    Serial.println("Updated all offset/bias values");
  }
  else{
    Serial.println("Error");  // Python dosent print END after sending values... or etc error
  }
  
  delay(500); // keep
}



// =======================================================
// PRINT CALIBRATED DATA
// =======================================================
void Calib_Output_Print()
{
  if (myISM.getRawGyro(&rawGyro) && myISM.getRawAccel(&rawAccel))
    {
      
        // ----------------------------GYRO---------------------------------------------
        // first Calib with offset/bias_lsb then Scale it
        
        // -------- Raw_LSB -> CALIB using bias_Lsb -> Calib_Lsb --------
        // (Calib_Lsb = Raw_LSB - Bias_Lsb)
        calibGyro[0] = rawGyro.xData - gyro_bias[0];
        calibGyro[1] = rawGyro.yData - gyro_bias[1];
        calibGyro[2] = rawGyro.zData - gyro_bias[2];

        // -------- Calib_Lsb -> Scale to -> Calib_Dps --------
        calibGyro[0] *= GYRO_Scale_DPS;
        calibGyro[1] *= GYRO_Scale_DPS;
        calibGyro[2] *= GYRO_Scale_DPS;


        // -------------------------------------Accel----------------------------------------
        // first scale then calib with scaled_offset/bias.
        
        // -------- LSB -> SCALE TO g  --------
        // (we are not doing 0.061 since that's mg not g. and the sf wants g . dont do 0.000061, since that has binary representation noise)
        rawAcc[0] = (rawAccel.xData) * ACC_LSB_TO_G ;
        rawAcc[1] = (rawAccel.yData) * ACC_LSB_TO_G;
        rawAcc[2] = (rawAccel.zData) * ACC_LSB_TO_G;

        // -------- SCALED -> CALIB --------
        // -------- Raw_g -> CALIB using offset/bias_g -> Calib_g  --------
        calibAccel[0] = A_acc[0][0] * (rawAcc[0] - b_acc[0]) + A_acc[0][1] * (rawAcc[1] - b_acc[1]) + A_acc[0][2] * (rawAcc[2] - b_acc[2]);
        calibAccel[1] = A_acc[1][0] * (rawAcc[0] - b_acc[0]) + A_acc[1][1] * (rawAcc[1] - b_acc[1]) + A_acc[1][2] * (rawAcc[2] - b_acc[2]);
        calibAccel[2] = A_acc[2][0] * (rawAcc[0] - b_acc[0]) + A_acc[2][1] * (rawAcc[1] - b_acc[1]) + A_acc[2][2] * (rawAcc[2] - b_acc[2]);


        // ------------------------------------------PRINT---------------------------------------------------------
        ///// in sf - instead of prining we give it to sf. 
        Serial.print("Gyro calib :  ");
        Serial.print(calibGyro[0], 6); Serial.print(", ");
        Serial.print(calibGyro[1], 6); Serial.print(", ");
        Serial.print(calibGyro[2], 6);
        
        
        // ------- Calculate magnitude of the calibrated vector
        // at static position the magnitude should be same as g value
        double magnitude = sqrt(calibAccel[0] * calibAccel[0] + calibAccel[1] * calibAccel[1] + calibAccel[2] * calibAccel[2]);  // no need to do in sf.
        
        // Print calibrated data (X, Y, Z) and the magnitude // in sf we will not print it , but give it to the sf. 
        Serial.print("     Accel calib :  ");
        Serial.print(calibAccel[0], 5); Serial.print(", ");
        Serial.print(calibAccel[1], 5); Serial.print(", ");
        Serial.print(calibAccel[2], 5);
        Serial.print(" | Magnitude: ");
        Serial.println(magnitude, 5);
       } 
      else 
       {
        Serial.println("Failed to read Sensor data.");
       }
}



// =======================================================
// SETUP
// =======================================================

void setup() {
	Wire.begin();
    Serial.begin(115200);
	
	
    if (!myISM.begin()) {
		Serial.println("Could not initialize ISM330DHCX. Check connections.");
        while (1);
    }
	
    // Reset the device to default settings
    myISM.deviceReset();
    while (!myISM.getDeviceReset()) {
		delay(1);
    }
	Serial.println("Reset complete.");
	Serial.println("Applying settings.");
	delay(100);
	
	
    // Apply device settings
    myISM.setDeviceConfig();
    myISM.setBlockDataUpdate(); 
	
	// Wire.setClock(400000); // uncomment it if using 416hz or higher data rate , other wise comment it.
	

	// DataRate -> 104Hz is good (fast & low noise) // can also try 208 or more in SF since it needs frequent values.
	myISM.setAccelDataRate(ISM_XL_ODR_104Hz); // data rate (best -> 104, 208, 416) (for faster use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12Hz5, 1Hz6 )
	myISM.setGyroDataRate(ISM_GY_ODR_104Hz); // data rate (best -> 104, 208, 416) (for fast use -> 833, 1666, 3332, 6667) (for slower use -> 52, 26, 12 )
	DataRate_HZ = 104; // keep same as sensor_Hz
    PERIOD_US = (1000000UL / DataRate_HZ); // using this we Control the iteration time.


	// Range -> always need to set, even if scaling maunally
	myISM.setAccelFullScale(ISM_2g); 
	myISM.setGyroFullScale(ISM_125dps); 
	// test: in Motor_code if motor moves faster than 2g or 125dps then the sensor values can break - so in that senario we can do 4g or 250dps, but that can lower the precision. other jugad we can do is -> move motor slowly or etc.
	// if you wanna update the range of acc or gyro then - not just you have to change the scaling factor , but again calib (as per that_range_lsb_data * that_range_scaling_factor) and get new offset.
	

	// Filter ->
	myISM.setAccelFilterLP2(false); // can keep it ON also, // later, TRY : ON in SensorFusion and see the results. 
    // myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_20); // can keep 20, 45 (not 10) // (if you keep the filter then based on the filter -> update the hardcoded offset/bias as per that)
	myISM.setGyroFilterLP1(false); // strictly off
	

	// fifo config (not much to do here, can try stream mode in SF, see ISM/1_full_info.ino ka final setting section for more info)
	myISM.setFifoMode(ISM_BYPASS_MODE); // fifo off (default)
	
     // Set motor RPM (10 is good)
  motorAlt.setTotalSteps(STEPS_PER_REV);
  motorAlt.setRpm(10); // 6 to 24
  motorAz.setTotalSteps(STEPS_PER_REV);
  motorAz.setRpm(10); // 6 to 24
    

	delay(100);
  Serial.println("Settings applied.");

//   Serial.println(" press 'l' for Accel recalibrate // press 'j' for Gyro recalibrate.");
  delay(500);

}

// =======================================================
// LOOP
// =======================================================
bool streamEnabled = true;

void loop()
{
  if (Serial.available())
  {
    char cmd = Serial.read();
     
    // Clear the buffer - (newline, extra keys, etc) 
    // so that during the calibration is being done - No key press are recoded. 
    // (other wise while calib you pressed j j j again then after calib it will again do calib 3 more times, since its in the buffer)
    while (Serial.available()) Serial.read();

    switch (cmd)
    {
      case 'g':
      // Serial.println("\n--- Offset Load Triggered ---");
      streamEnabled = false;
      requestOffsetsFromPython(); // get latest offset/bias from the offset.txt using python.
      streamEnabled = true;
      break;

      case 'j':
        // Serial.println("\n--- Gyro Calibration Triggered ---");
        streamEnabled = false;
        calc_gyro_bias();
        streamEnabled = true;
        break;

      case 'l':
        // Serial.println("\n--- Accel Calibration Triggered ---");
        streamEnabled = false;
        calc_acc_offset();
        streamEnabled = true;
        break;

      case 'p':
        // print all offset/bais values , good for debugging and see if the offset.txt values or calib values actually updated or not.
        streamEnabled = false;
        delay(100);// imp
        Serial.print("GYRO: ");
        Serial.print(gyro_bias[0],6);Serial.print(", ");
        Serial.print(gyro_bias[1],6);Serial.print(", ");
        Serial.println(gyro_bias[2],6);
        Serial.print("B_ACC: ");
        Serial.print(b_acc[0],6 );Serial.print(", ");
        Serial.print(b_acc[1],6 );Serial.print(", ");
        Serial.println(b_acc[2],6 );
        Serial.print("A_ACC: ");
        Serial.print(A_acc[0][0],6 );Serial.print(", ");
        Serial.print(A_acc[0][1],6 );Serial.print(", ");
        Serial.print(A_acc[0][2],6 );Serial.print(", ");
        Serial.print(A_acc[1][0],6 );Serial.print(", ");
        Serial.print(A_acc[1][1],6 );Serial.print(", ");
        Serial.print(A_acc[1][2],6 );Serial.print(", ");
        Serial.print(A_acc[2][0],6 );Serial.print(", ");
        Serial.print(A_acc[2][1],6 );Serial.print(", ");
        Serial.println(A_acc[2][2],6 );
        Serial.println("DONE");
        streamEnabled = true;
        break;
      
      case 'y':
        // to display 10 calib_output values in terminal. (no need , just for degubbing)
        streamEnabled = false;
        delay(100);
        for(int i = 0 ; i < 10 ; i++) 
        { Calib_Output_Print(); }
        Serial.println("DONE");
        streamEnabled = true;
        break;
         

      default:
        break;
    }
  }

  now = micros();
  if (now - lastRead >= PERIOD_US)
  {

    lastRead = now;
    if(streamEnabled)
    {
    // Calib_Output_Print(); // in sf -> in this function we will not print the calib values , but do SF ... 
    }
  }

}


