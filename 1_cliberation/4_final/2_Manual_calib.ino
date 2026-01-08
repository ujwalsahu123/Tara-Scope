/////////////////////////////////////////
// HERE use the -> 1.1_Auto_calib.ino code and comment all the calib part and it should give calib_valeus only as per the hardcoded one. 
// so that we use this code in SF 
// ( so that quickly we can test and debugg without any Auto_calc_offset...part )
// and keep nice offset for acc,gyro,mag - since we will not auto_calib here.
// after we figured out SF then we will mix the 1_Auto_calib wala with sf in -> sf_with_auto_calib code file.
/////////////////////////////////////////








// use this manually code for SF . since sf may cannot do calib every time. 


// xtra - i was thinking that the calib takes time and if we first calib acc then gyro then both the values will be of diff time. 
// so sould we do parallelism here ? like non-blocking code .. and call both  ?   i dont ask gpt ?  
// but how will we know if when the calib is done ? and if code run faster than they calc then sf may old data jaygaa. 
// for ex - parallel you run acc and gyro calib fun , and then uskay bad sf we give data then sf gets old data if it takes more time for calib...
// gpt ask ...



