# see the ai_guide_app.py code (for how to calculate the alt az of a star.) - full study how that code works. - which libaraies do we use etc and how does it calculate ... etc all
# also take a day out to learn about - libraries using which the alt az are calculated - py libraries , js libaries , etc ... 


# select a star from the list in terminal. 
# then voo star ka alt az calulate and give to arduino for pointing. (3_full-motor-code.ino use kar)
# we can also use the sterellium desktop app to give the alt az of the star to the arduino. ... 
# both ways ka code likh yaha par. in 2 seperate code files -> 0_library_to_point.py and 0_stellarium_to_point.py
# its easy - just see the ai_guide_app.py and the stellarium_pointing.py code.
# then arudino code will be same - 3_full-motor-code.ino use karna hai.

# soo we will know kya scene hai - accurate pointing or not. 
# then do 3 star calib and run point_star_using_terminal and see - accuracy improve hota hai ya nahi using 3 star calib.


# Entire flow - -----------------------------------------------------
# list print hoga terminal may - select a star.
# then voo star ka alt az calculate hoga ( +5 seconds in future position)
# then alt az arduino may bheja jaega , and then in termial it waits for user to press "s"
# and then arduino will point to the target and after reached it will point laser one time, and then contineaously point until we press "s" 
# and after pressing "s" - it will go to 0,0 position.
# and after pressing "s" - terminal will again show all the list of stars above horizon.

# see the sheet image , in this folder flow.jpg  for the flow .  ( and look at the suntracker sheets i made.)