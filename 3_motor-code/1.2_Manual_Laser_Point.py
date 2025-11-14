# Arrow keys = fast movement
# w a s d = precise movement
# SPACE = laser toggle 
# f = fade laser 
# v = draw diamond
# c = draw circle 
# b = draw box / square
# ESC = quit

# (not tested the laser part yet, need to check after doing 3_laser part)

# after laser code done, update here also. 

# Test out the radius and speed(delay) in arduino code. - and see if proper circeling out the star. 
# keep the values as mentioned for radius and delay . try to lower the radius and increase the delay to see how it impacts the circle shape.

# circle is not properly coming with smaller radius, you have to update stepDeg = 1 - 5 accordingly. (jitna bada Radius unta chota stepDeg, and vice versa)

# // place alt motor on the left side (facing right) of of the az mount , and laser will point forward .


##############################################################################################

# // Arduino code 

# #include <CheapStepper.h>

# // AZ - left (false) right (true)
# // Alt - up (false) down (true)


# // -------- Motors --------
# CheapStepper motorA(8, 9, 10, 11);   // Azimuth (ULN2003 IN1–IN4)
# CheapStepper motorB(4, 5, 6, 7);     // Altitude (ULN2003 IN1–IN4)

# const long STEPS_PER_REV = 4076;
# const int RPM = 10;  // base RPM for normal moves

# // ===== PRECISE CONTROL SETTINGS (w a s d) =====
# const int PRECISE_STEP  = 1;     // steps per precise press  // keep it 1


# // ===== Shape settings #################################################################################
# const int RADIUS_IN_STEPS = 20;      // <- radius (steps) // keep 20 . below 10 dosent work
# const int DELAY_MS = 50;       // <- speed (ms per iteration) // keep 50 . 
# // both are inversely proportional - so if you increase the Radius then decrease the delay ex - 200 Radius , 5 delay.
# // and if you want to decrease the Radius then decrease the delay. ex - 15 Radius , 60 delay.
# // ######################################################################################################

# // -------- Laser - spacebar - on/off --------
# #define LASER_PIN 3   // PWM pin we can use this pin for both - digitalWrite and analogWrite
# bool laserOn = false; 

# void setLaser(bool on) {
#   laserOn = on;
#   digitalWrite(LASER_PIN, on ? HIGH : LOW);
# }

# // -------- Laser fade - --------
# const int FADE_RATE_MS = 10;      // delay between fade steps
# const int LASER_ON_TIME_MS = 1000; // how long laser stays ON at full brightness

# void fadeLaser(int fadeRate, int laserOnTime) {
#   Serial.println("Laser fade: starting");

#   // 1) Fade In
#   for (int brightness = 0; brightness <= 255; brightness++) {
#     analogWrite(LASER_PIN, brightness);
#     delay(fadeRate);
#   }

#   // 2) Hold full brightness
#   delay(laserOnTime);

#   // 3) Fade Out
#   for (int brightness = 255; brightness >= 0; brightness--) {
#     analogWrite(LASER_PIN, brightness);
#     delay(fadeRate);
#   }

#   analogWrite(LASER_PIN, 0);
#   Serial.println("Laser fade: done");
# }

# void setup() {
#   pinMode(LASER_PIN, OUTPUT);
#   setLaser(false); // start OFF

#   Serial.begin(9600);

#   motorA.setTotalSteps(STEPS_PER_REV);
#   motorB.setTotalSteps(STEPS_PER_REV);

#   motorA.setRpm(RPM);
#   motorB.setRpm(RPM);

# }

# // ------------------------ Shapes ------------------------

# inline void stepBoth(bool azDir, bool altDir) {
#   motorA.move(azDir, 2);
#   motorB.move(altDir, 1);
#   delay(DELAY_MS);
# }

# void drawDiamond() {

#   Serial.println("Drawing Diamond..."); 

#   // long alt0 = motorB.getStep();  // no need of this, since we are not using moveTo() to return to the original posi. // but still keep the code.
#   // long az0  = motorA.getStep();


#   // 1) go to bottom (ALT down by R)
#   for(int i=0;i<RADIUS_IN_STEPS;i++) {
#     motorB.move(true, 1);
#     delay(DELAY_MS);
#   }  
#   delay(500);

#   // 2) laser ON
#   setLaser(true);

#   // 3) four loops 

#   // Loop1: AZ left R, ALT up R
#   for(int i=0;i<RADIUS_IN_STEPS;i++) stepBoth(false, false);

#   // Loop2: AZ right R, ALT up R
#   for(int i=0;i<RADIUS_IN_STEPS;i++) stepBoth(true,  false);

#   // Loop3: AZ right R, ALT down R
#   for(int i=0;i<RADIUS_IN_STEPS;i++) stepBoth(true,  true);

#   // Loop4: AZ left R, ALT down R
#   for(int i=0;i<RADIUS_IN_STEPS;i++) stepBoth(false, true);

#   // 4) laser OFF
#   setLaser(false);

#   delay(500);

#   // 5) return to original pointing

#     for(int i=0;i<RADIUS_IN_STEPS;i++) {
#     motorB.move(false, 1);
#     delay(DELAY_MS);
#   }  

#   // #   motorB.moveTo(false, alt0); // up 
#   // // ---- Smart Azimuth return ---- we cannot simply do motorA.moveTo(az0) because it may take the long way around
#   // long currentAz = motorA.getStep();
#   // long diff = az0 - currentAz;
#   // if (diff > 0)      motorA.moveTo(true,  az0);
#   // else if (diff < 0) motorA.moveTo(false, az0);

#   delay(200);
#   Serial.println("Diamond done");
# }

# //-------------------------------------------------
# void drawSquare() {
#   Serial.println("Drawing Square...");

#   // long alt0 = motorB.getStep();
#   // long az0  = motorA.getStep();

#   for(int i=0;i<RADIUS_IN_STEPS;i++) {
#     motorB.move(true, 1);
#     delay(DELAY_MS);
#   } 
#     delay(500);

#   setLaser(true);

# // 3) Left by R/2
#   for (int i = 0; i < RADIUS_IN_STEPS ; i++) {
#     motorA.move(false, 1);   // left
#     delay(DELAY_MS);
#   }

#   // 4) Up by R
#   for (int i = 0; i < RADIUS_IN_STEPS * 2; i++) {
#     motorB.move(false, 1);   // up
#     delay(DELAY_MS);
#   }

#   // 5) Right by R
#   for (int i = 0; i < RADIUS_IN_STEPS * 2; i++) {
#     motorA.move(true, 1);    // right
#     delay(DELAY_MS);
#   }

#   // 6) Down by R
#   for (int i = 0; i < RADIUS_IN_STEPS * 2; i++) {
#     motorB.move(true, 1);    // down
#     delay(DELAY_MS);
#   }

#   // 7) Left by R/2 
#   for (int i = 0; i < RADIUS_IN_STEPS ; i++) {
#     motorA.move(false, 1);   // left
#     delay(DELAY_MS);
#   }

#   setLaser(false);
#   delay(500);

#   // return to original center
  
#   for(int i=0;i<RADIUS_IN_STEPS;i++) {
#     motorB.move(false, 1);
#     delay(DELAY_MS);
#   }  

#   // #   motorB.moveTo(false, alt0); // up 
#   // long currentAz = motorA.getStep();
#   // long diff = az0 - currentAz;
#   // if (diff > 0)      motorA.moveTo(true,  az0);
#   // else if (diff < 0) motorA.moveTo(false, az0);

#   delay(200);

#   Serial.println("Square done");
# }

# //-------------------------------------------------

# void drawCircle() {

#   Serial.println("Drawing Circle...");
  
#   // long alt0 = motorB.getStep();
#   // long az0  = motorA.getStep();

#   const float stepDeg = 2; // smaller = smoother // jitna small Radius utna hee bada stepDeg rakhna padega.

#   // move to bottom of circle
#   for(int i=0;i<RADIUS_IN_STEPS;i++) {
#     motorB.move(true, 1);
#     delay(DELAY_MS);
#   } 
#   delay(500);

#   setLaser(true);

#   float prevAz = 0;
#   float prevAlt = -RADIUS_IN_STEPS; // start at bottom

#   for (float deg = -90 + stepDeg; deg <= 270; deg += stepDeg) {
#     float rad = deg * 3.14159 / 180.0;
#     float az = RADIUS_IN_STEPS * cos(rad);
#     float alt = RADIUS_IN_STEPS * sin(rad);

#     int dAz = round(az - prevAz);
#     int dAlt = round(alt - prevAlt);

#     if (dAz > 0) motorA.move(true, dAz);
#     else if (dAz < 0) motorA.move(false, -dAz);

#     if (dAlt > 0) motorB.move(false, dAlt);
#     else if (dAlt < 0) motorB.move(true, -dAlt);

#     prevAz = az;
#     prevAlt = alt;

#     delay(DELAY_MS);
#   }

#   setLaser(false);
#   delay(200);

#   for(int i=0;i<RADIUS_IN_STEPS;i++) {
#     motorB.move(false, 1);
#     delay(DELAY_MS);
#   }  

#   // #   motorB.moveTo(false, alt0); // up 
#   // long currentAz = motorA.getStep();
#   // long diff = az0 - currentAz;
#   // if (diff > 0)      motorA.moveTo(true,  az0);
#   // else if (diff < 0) motorA.moveTo(false, az0);

#   delay(200);

#   Serial.println("Circle done");
# }


# // ------------------------

# void loop() {
#   if (Serial.available()) {
#     char c = Serial.read();

#     switch (c) {
#       // ===== FAST (arrows) =====
#       case 'U': motorB.move(false, 1); break;  // Up  -> ALT down (your mapping)
#       case 'D': motorB.move(true,  1); break;  // Down-> ALT up
#       case 'R': motorA.move(true,  1); break;  // Right-> AZ CW
#       case 'L': motorA.move(false, 1); break;  // Left -> AZ CCW

#       // ===== precise (w a s d) =====
#       case 'a': motorA.move(false, PRECISE_STEP); break;
#       case 'd': motorA.move(true,  PRECISE_STEP); break;
#       case 'w': motorB.move(false, PRECISE_STEP); break;
#       case 's': motorB.move(true,  PRECISE_STEP); break;

#       // ===== laser toggle (space) =====
#       case 'T': setLaser(!laserOn);Serial.println(laserOn ? "LASER ON" : "LASER OFF"); break;

#       // ===== fade laser (f) =====
#       case 'f': fadeLaser(FADE_RATE_MS, LASER_ON_TIME_MS); break;

#       // shapes
#       case 'v': drawDiamond(); break;
#       case 'b': drawSquare(); break;
#       case 'c': drawCircle(); break;

#       default: break;
#     }
#   }
# }








# -----------------------------------------------------------------------------------------
# python code


import serial
import keyboard
import time

arduino = serial.Serial('COM7', 9600, timeout=0.2)  # <-- change to your port
time.sleep(2)  # wait for Arduino reset

print(" Arrows = fast | w a s d = precise | SPACE = laser toggle | f = fade laser | v = draw diamond | b = draw square | c = draw circle | ESC = quit")

while True:

    # --- read serial ---
    if arduino.in_waiting > 0:
        try:
            msg = arduino.readline().decode(errors='ignore').strip()
            if msg:
                print(msg)
        except:
            pass

    # fast (arrows)
    if   keyboard.is_pressed('up'):    arduino.write(b'U') # here we didnt write debounce - since we want continuous movement while we hold a arrow key.
    elif keyboard.is_pressed('down'):  arduino.write(b'D')
    elif keyboard.is_pressed('right'): arduino.write(b'R')
    elif keyboard.is_pressed('left'):  arduino.write(b'L')

    # precise (w a s d)
    elif keyboard.is_pressed('a'):
        arduino.write(b'a')
        while keyboard.is_pressed('a'): time.sleep(0.02)
    elif keyboard.is_pressed('d'):
        arduino.write(b'd')
        while keyboard.is_pressed('d'): time.sleep(0.02)
    elif keyboard.is_pressed('w'):
        arduino.write(b'w')
        while keyboard.is_pressed('w'): time.sleep(0.02)
    elif keyboard.is_pressed('s'):
        arduino.write(b's')
        while keyboard.is_pressed('s'): time.sleep(0.02)

    # laser toggle (space)
    elif keyboard.is_pressed('space'):
        arduino.write(b'T')
        while keyboard.is_pressed('space'): time.sleep(0.02)

    # --- fade laser ---
    elif keyboard.is_pressed('f'):
        arduino.write(b'f')
        while keyboard.is_pressed('f'): time.sleep(0.02)

    # Draw shapes
    elif keyboard.is_pressed('v'):
        arduino.write(b'v')
        while keyboard.is_pressed('v'): time.sleep(0.02)

    elif keyboard.is_pressed('b'):
        arduino.write(b'b')  # Square
        while keyboard.is_pressed('b'): time.sleep(0.02)

    elif keyboard.is_pressed('c'):
        arduino.write(b'c')  # Circle
        while keyboard.is_pressed('c'): time.sleep(0.02)


    # exit
    if keyboard.is_pressed('esc'):
        print("Exiting...")
        break


    time.sleep(0.006) # this delay impacts the speed of arrow key movement. 0.006 is good. 