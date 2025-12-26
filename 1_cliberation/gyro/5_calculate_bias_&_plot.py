#  code for calculating bias and then plot -> scaled uncalib data and calib data side by side.

#  can i do calib on 6hr data , or do i need to write ardu code for getting 10 sec,etc data ? 
# and after getting the data - i need to first scale it and then find calib offsets ...

# how to caliberate the gryo ?   use magnetor or what ??    first go and study this. 


# gryo_callib = gryo_raw - bias 

# How to Calculate bias ???????????? (use scaled.txt or raw.txt and find bias using that)
# find bias using raw or scaled data ?????? (we can find the bias using scaled data , or we can find the bias using lsb and then scale the bias ? )  
# how to apply calib (gryo_callib = gryo_raw - bias ) ?????
# store bias in offset.txt

# also is there a way to test if the calib is good ? like there was Magnitude in acc (close to 1) like that can we do ,
#  and plot the data points before and after the calib...

# here we can plot also the uncalib and calib data. (scalud.txt - uncalib plot hogg and then apply calib on that scaled.txt and plot the calib data side by side)



# we caliberate after every startup gyro after every startup ?????  
# // (The gyro is normally calibrated at every startup. With the sensor still, collect a few hundred readings, and average them to get the offsets. The offsets are subtracted from later readings, while in normal operation.)