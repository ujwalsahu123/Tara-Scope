
import serial
import keyboard
import time

arduino = serial.Serial('COM7', 9600, timeout=0.2)  # <-- change to your port
time.sleep(2)  # wait for Arduino reset

print(" Arrows = fast | w a s d = precise | SPACE = laser toggle | v = draw diamond | f = fade laser | ESC = quit")

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

    # Draw Diamond (v)
    elif keyboard.is_pressed('v'):
        arduino.write(b'v')
        while keyboard.is_pressed('v'): time.sleep(0.02)

    # --- fade laser ---
    elif keyboard.is_pressed('f'):
        arduino.write(b'f')
        while keyboard.is_pressed('f'): time.sleep(0.02)

    # exit
    if keyboard.is_pressed('esc'):
        print("Exiting...")
        break


    time.sleep(0.006) # this delay impacts the speed of arrow key movement. 0.006 is good. 