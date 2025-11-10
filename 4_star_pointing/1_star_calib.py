#  using buttons point the laser to a selected star 
#  then calculate the difference between the ALT AZ of the star and the Pitch Yaw of the sensor.
#  the store the averge difference in a offset.txt file

#  in this code: there is a loop and it runs 3/5 times ( thus 3/5 star calib)
#  in the loop : we need to first select a star from the list - using terminal
# then voo star ka data fetch hoga. 
#  then using buttons we will point the laser to the star.
# and side by side RPY is calculated real time.
# then we press enter and jaise he enter press hoga - 
# vaise he Pitch Yaw ka value ek variable may store hoga. and ALT AZ calculate hoga voo selected star ka. 
# (caution : calculating ALT AZ takes time of around 5 second . and calculaing Ptich yaw also takes time.
# so because of that - star ka alt az ka value point.xx change hojata hai)




# use the ai_guide_app.py code to understand how to caluclate ALT AZ of a star.
# but instead of finding the top 20 brightest stars,
# we will make a list of top 200 most brightest stars. and a list of all planets.
# then among them - find all the present star above horizon will be printed in terminal
# then we will select a star  which will be stored in a variable.
# then using that star's name - we will fetch its RA DEC and caluclate its ALT AZ.


# and differnce1 , difference2, difference3 which were calculated in 3 iterations will be averaged and stored appended in parameters.txt (offset_pitch = .. , offset_yaw = ... )



# to make this work i will need both - .py and .ino code.

# THINK of the entire flow of the code -
# in .py code : this this ... will be done.
# in .ino code this this will be done . 

# ex: ek aisa arudino.ino code jo continuously RPY calculate karta rahega. & button say motors move hum kar sake . & when we want - the RPY value is printed. 
# and .py code - may sare list hoga stars ka in the terminal - we will select a star . then using the temrinal we will move the motors, then we press enter - the RPY value is read from arduino - alt az is quickly calculated  - difference is calculated - stored in a list - then next iteration starts.