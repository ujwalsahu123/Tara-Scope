# using api we get the accurate lati , longi , alti (use noaa api)
# then using the noaa api we get the magnetic field
# then the mag calib runs and stors in a mag_calib.txt file
# then we put the magnetic filed vlaue , mag_calib.txt ,etc, etc in magnetor using api
# and get the offset of the magnetometer . (acc , gryo ka offset constant hee rahega)
# etc etc 
# and we store all this stuff in Parameter.txt file. 

# then it runs the 3-star_calib code and stores the calib value . in parameter.txt file so that when pointing we can use it.



# SO we only run the _main.py file once , and baki sare code files (2_point_star_using_terminal, 3_point_star_using_stellarium, ai guide, desktop app , etc ) may we import the Parameters.txt to get the
# lati , longi , alti , calib ,  etc, etc ....   and use them for our calculations.