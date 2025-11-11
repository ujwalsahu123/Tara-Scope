# when pressed spacebar, the laser turns on / off .  (full intensity , no fade in/out)
# press esc to quit



# test out if works, if changes hai here , then also change in the 1.2_Maual_laser code.




# ---------------------------------------------------------------------------------


#arduino code

# #define LASER_PIN 7        // D4184 PWM/SIG input
# bool laserOn = false;

# void setLaser(bool on) {
#   laserOn = on;
#   digitalWrite(LASER_PIN, on ? HIGH : LOW);
# }

# void setup() {
#   pinMode(LASER_PIN, OUTPUT);
#   setLaser(false);         // start OFF
#   Serial.begin(115200);    // must match Python
#   // while (!Serial) { }   // optional for Leonardo/Micro; not needed for UNO
# }

# void loop() {
#   if (Serial.available()) {
#     char c = (char)Serial.read();
#     if (c == 'T' || c == 't') {
#       setLaser(!laserOn);        // toggle
#       Serial.println(laserOn ? "ON" : "OFF");  // optional feedback
#     }
#   }
# }


# ------------------------------------------

# Python code

import time
import serial
import keyboard  # if this is troublesome on Linux, I can give a pynput version

PORT = "COM3"      # <-- change this (e.g., "COM3", "/dev/ttyACM0", "/dev/ttyUSB0")
BAUD = 115200

ser = serial.Serial(PORT, BAUD, timeout=0.2)
time.sleep(2.0)  # give Arduino time to reset after opening serial

print("Space = toggle laser, Esc = quit. (Laser starts OFF)")

try:
  while True:
    if keyboard.is_pressed('esc'):
      break

    if keyboard.is_pressed('space'):
      ser.write(b'T')  # send toggle
      # optional: read ack from Arduino
      try:
        resp = ser.readline().decode(errors='ignore').strip()
        if resp:
          print("Arduino:", resp)
      except:
        pass

      # debounce: wait until space is released
      while keyboard.is_pressed('space'):
        time.sleep(0.02)

    time.sleep(0.01)

finally:
  ser.close()
  print("Serial closed.")