# Tara Scope Motor Control (arrow keys , w a s d) , laser (spacbar)

# works fine

# // so i will know ki how to run the motor and if the min step angle is enough to point a star . or need to do gearing 1:10

# // and this code and 1.2 code will also help in the - 3_star_calibration - code






#########################################################################################################   


# 1) Motor control using Arrow Keys and 1/2/3/4 keys        (works fine)

#--------------------------------------------------------------------------

# INFO:

# ↑ ↓ ← →  = Fast movement
# w a s d = Precise 1 step move
# esc = quit program

# you can contineous press arrow keys for fast movement
# and single press w a s d keys for precise movement
# set the RPM , Precise_step and Precise_delay in the arduino code as per your need. 

#-------------------------------------------------------------------------------------
# // Arduino code


# #include <CheapStepper.h>

# // Motor A (Azimuth) → ULN2003 IN1–IN4
# CheapStepper motorA(8, 9, 10, 11);
# // Motor B (Altitude) → ULN2003 IN1–IN4
# CheapStepper motorB(4, 5, 6, 7);

# const long STEPS_PER_REV = 4076;

# const int RPM = 10;    // set the RPM.


# // ===== PRECISE CONTROL SETTINGS w a s d =================================

# const int PRECISE_DELAY = 100;   // delay (ms) after each precise step // 100 is fine
# const int PRECISE_STEP  = 1;     // steps per precise key press. 
# // Note: 1 works, so try to keep this 1 . but it not able to work (since 1 step is a very small movement - half-step) then Change to 2, 3. 



# void setup() {
#   Serial.begin(9600);

#   motorA.setTotalSteps(STEPS_PER_REV);
#   motorB.setTotalSteps(STEPS_PER_REV);
#   motorA.setRpm(RPM);
#   motorB.setRpm(RPM);

#   Serial.println("System ready:");
#   Serial.println("↑ ↓ ← →  = Fast movement");
#   Serial.println("w a s d  = Precise move");
#   Serial.print("Precision Settings -> Steps: ");
#   Serial.print(PRECISE_STEP);
#   Serial.print(", Delay: ");
#   Serial.print(PRECISE_DELAY);
#   Serial.println(" ms");
# }

# void loop() {
#   if (Serial.available()) {
#     char c = Serial.read();

#     switch (c) {

#       // ===== FAST MOTION (Arrow Keys) =====
#       case 'U':   // Up arrow → Altitude CCW
#         motorB.move(false, 1);
#         break;

#       case 'D':   // Down arrow → Altitude CW
#         motorB.move(true, 1);
#         break;

#       case 'R':   // Right arrow → Azimuth CW
#         motorA.move(true, 1);
#         break;

#       case 'L':   // Left arrow → Azimuth CCW
#         motorA.move(false, 1);
#         break;


#       // ===== PRECISE MOTION (Keys 1–4) =====
#       case 'a':   // Azimuth CCW
#         delay(PRECISE_DELAY);
#         motorA.move(false, PRECISE_STEP);
#         delay(PRECISE_DELAY);
#         break;

#       case 'd':   // Azimuth CW
#         delay(PRECISE_DELAY);
#         motorA.move(true, PRECISE_STEP);
#         delay(PRECISE_DELAY);
#         break;

#       case 'w':   // Altitude CCW
#         delay(PRECISE_DELAY);
#         motorB.move(false, PRECISE_STEP);
#         delay(PRECISE_DELAY);
#         break;

#       case 's':   // Altitude CW
#         delay(PRECISE_DELAY);
#         motorB.move(true, PRECISE_STEP);
#         delay(PRECISE_DELAY);
#         break;

#       default:
#         break;
#     }
#   } 
# }


# -------------------------------------------------------------------------------------
# Python code


# import serial
# import keyboard
# import time

# # --- Configure your serial port ---
# arduino = serial.Serial('COM7', 9600, timeout=0.1)  # <-- change this to your port
# time.sleep(2)  # wait for Arduino to reset

# while True:
#     # ----- Motor B (Altitude) -----
#     if keyboard.is_pressed('up'):
#         arduino.write(b'U')

#     elif keyboard.is_pressed('down'):
#         arduino.write(b'D')

#     # ----- Motor A (Azimuth) -----
#     elif keyboard.is_pressed('right'):
#         arduino.write(b'R')

#     elif keyboard.is_pressed('left'):
#         arduino.write(b'L')

#     # ----- Precise movement -----
#     elif keyboard.is_pressed('a'):
#         arduino.write(b'a')
#         # wait until key released (debounce)
#         while keyboard.is_pressed('a'):
#             time.sleep(0.02)

#     elif keyboard.is_pressed('d'):
#         arduino.write(b'd')
#         while keyboard.is_pressed('d'):
#             time.sleep(0.02)

#     elif keyboard.is_pressed('w'):
#         arduino.write(b'w')
#         while keyboard.is_pressed('w'):
#             time.sleep(0.02)

#     elif keyboard.is_pressed('s'):
#         arduino.write(b's')
#         while keyboard.is_pressed('s'):
#             time.sleep(0.02)


#     # ----- Read Arduino feedback -----
#     if arduino.in_waiting > 0:
#         try:
#             msg = arduino.readline().decode().strip()
#             if msg:
#                 print(msg)
#         except Exception:
#             pass

#     # ----- Exit -----
#     if keyboard.is_pressed('esc'):
#         print("Exiting...")
#         break
    
#     time.sleep(0.006)  # small delay , this implacts the speed of arrow key movement. 0.006 is good.

#################################################################################################################

