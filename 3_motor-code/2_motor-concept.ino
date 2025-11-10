
// arduino code.txt may bhi hai ..
// star_pointing gpt chat. 

// if near then less step move , if far then more step move.
// and it contineaouly iterates .

// and its like a range - (it dosent directly point to 0 deg)
// its like we move the motor to run till it achives a range of (-0.1 to 0.1)

// parallel how motor...

// motor concept -> 

// Alt Az ayga , and the range will become 
// int range = 0.1
// ALT_LOWER =  ALT - range
// ALT_UPPER =  ALT + range
// AZ_LOWER =  AZ - range
// AZ_UPPER =  AZ + range

// so if the ALT = 45.5 then the rangle will be 45.4 to 45.6. and if the sensor pitch is beween that range then it will not move.

// and after the both the targed range is achieved , the laserfade() function will be called to turn on the laser.


// entire flow likh ------------------------------------------
# ..... read sensor ,  parallel motor move , laser point , ......

setOutputTorque(true)
Coils stay ON → motor locks position (useful when holding a laser steady) 
? idk to use this.  since we move motors as per sensors (consistently) and not just move some particular step and wait.


// for parallel motor movement - use cheapstepper ka .newmove(step) function . which is a non-blocking function. 

so  -> loop{

    read sensr value and use SF to get RPY

    if (pitch > alt_higer)
        motorA.newmove(1)
    else if (pitch < alt_lower)
        motorA.newmove(-1)
    else
        motorA.newmove(0)
    
    same for yaw.

    .run()
}

// so it reads RPY then both motors by one step. 


// so here again and again if we read the RPY after every loop then not good. 
// insted we can make another function paralley to calculate RPY. 
// idk if this is good or bad. need to think . since this above code is also looks good.  