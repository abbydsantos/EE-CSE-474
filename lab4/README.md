Inside this directory, you'll find the following:

 - Makefile: the gnu Makefile to compile lab 4; to run it, just type "make"
   at the command line

 - handler.c: the implementation of the sensor module

 - handler.h: the header file for the sensor module

 - motor.c: the implementation of the motor module

 - motor.h: the header file for the motor module
 
 - pwm.c: the implementation for the beeping from the beaglebone

 - pwm.h: the header file for the motor module

After you compile, you'll have access to three executables:

 - motor: controls the motor of the beaglebone by reading from a pipe
       that handler writes to

 - handler: read from the sensor and write to a pipe

 - pwm: beeps when the beaglebone gets too close to an object

