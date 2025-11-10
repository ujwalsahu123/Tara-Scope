# pointing after applying the offset that we got from the 3/5 star calib. 

# select a star from terminal, caluclate its alt az (+5 seconds in future position)
#  then pass it to arudino for pointing. 

# use the 3_full-motor_control.ino code. for the arduino side which simply takes the alt az and points to it. 
# use the 0_basic_pointing.py for the calculation of alt az of a star. 
# the main game here is -> applying those star - offset before sending the alt az to arduino.
# we fetch star_offset from the .txt file to apply the offset on the alt az - which we got from 3/5 star calibration.
