Inside this directory, you'll find the following:

 - Makefile: the gnu Makefile to compile the kernel files; to run it, just type
   "make" at the command line

 - mkGame.mk: the gnu Makefile to compile the game files to run the userspace

 - game.h: the public header for the game module.
   This header contains all of the definitions, typedefs, and function
   prototypes upon which customers depend.

 - game.c: the implementation of the game module.

 - lcd.h, lcd2.c: basis kernel space code to test writing from terminal

 - lcd_uspace.h, gamespace.c: similar to the lcd files, but for the game
   kernel space

 - pipe.c: the pipe module used to talk to game.c

After you compile, you'll have access to two executables:

 - game:  starts the game and plays through it

 - pipe: create a pipe to write from the console to talk to game

After you compile, you'll have two kernel files:

 - gamespace.ko: kernel for the game module

 - lcd2.ko: basis kernel to test writing from terminal
