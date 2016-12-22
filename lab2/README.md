Inside this directory, you'll find the following:

 - Makefile: the gnu Makefile to compile the kernel files; to run it, just type
   "make" at the command line

 - game.h: the public header for the game module.
   This header contains all of the definitions, typedefs, and function
   prototypes upon which customers depend.

 - game.c: the implementation of the game module.

 - lcd.h, lcd.c: similar to game.h and game.c, but for writing to the LCD monitor

 - pipe.c: the pipe module used to talk to game.c and lcd.c

 - osc.c: the module that verifies the enable signal in the LCD

After you compile, you'll have access to two executables:

 - game:  starts the game and plays through it

 - pipe: create a pipe to write from the console to talk to game

 - osc: verifies the enable signal
