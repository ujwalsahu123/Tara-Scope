# see the ai_guide_app.py code (for how to calculate the alt az of a star.) 
# select a star from the list in terminal. 
# then voo star ka alt az calulate and give to arduino for pointing. (full-motor-code.ino use kar)

# soo we will know kya scene hai - accurate pointing or not. 
# then do 3 star calib and run point_star_using_terminal and see - accuracy improve hota hai ya nahi using 3 star calib.


# Entire flow - -----------------------------------------------------
# list print hoga terminal may - select a star.
# then voo star ka alt az calculate hoga ( +5 seconds in future position)
# then alt az arduino may bheja jaega , and then in termial it waits for user to press "s"
# and then arduino will point to the target and after reached it will point laser one time, and then contineaously point until we press "s" 
# and after pressing "s" - it will go to 0,0 position.
# and after pressing "s" - terminal will again show all the list of stars above horizon.

