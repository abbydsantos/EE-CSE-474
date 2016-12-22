Inside this directory, you'll find the following:

 - Makefile: the gnu Makefile to compile lab 5; to run it, just type "make"
   at the command line

 - handler.c: the implementation of the sensor module

 - handler.h: the header file for the sensor module

 - motor.c: the implementation of the motor module

 - motor.h: the header file for the motor module

 - reader.py: the python file to set the baudrate for the bluetooth

 - start.sh: the bash file for the startup code for the BeagleBone. The bash script compiles
   the motor and handler module and read them and the python file

 - MainActivity.java: the java file for the UI thread for the app that controls the Beaglebone

 - WriteTask.java: the java class that writes to the bluetooth in the app

After you compile, you'll have access to two executables:

 - motor: controls the motor of the beaglebone by reading from a pipe
       that handler writes to

 - handler: read from the sensor and write to a pipe

