# using api we get the accurate lati , longi , alti (use noaa api)
# then using the noaa api we get the magnetic field
# then it runs the mag calib and stors in a mag_calib.txt file
# then it put the magnetic filed vlaue , mag_calib.txt ,etc, etc in magnetor using api
# and get the offset of the magnetometer . (acc , gryo ka offset constant hee rahega) all the offset store in a sensor_offset.txt file. and send it later to arduino (where it will take the offset.) so need to make a new .ino file usinng the 3_full-motor-code code - where the offsets are not hardcoded but taken from the serial monitor.
# etc etc 
# and we store all this stuff in Parameter.txt file. 

# then it runs the 3-star_calib code and stores the offset value . in offset.txt file so that when pointing we can use it.



# SO we only run the _main.py file once , and baki sare code files (2_point_star_using_terminal, 3_point_star_using_stellarium, ai guide, desktop app , etc ) may we import the Parameters.txt to fetch the
# lati , longi , alti , calib ,  etc, etc .... from that .txt file   and use them for our calculations.

# so only need to run one script. ( and it does -> get data , mag calib , star calib ) and then we only select the star from the terminal or stellarium and it points to it.