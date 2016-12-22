# Daisy X., Yunie Y., Abby S.
# default target
handler: handler.c handler.h
	gcc -Wall -g -o handler handler.c -lm
motor: motor.c motor.h
	gcc -Wall -g -o motor motor.c -lm

# make all target
all:
	gcc -Wall -g -o handler handler.c -lm
	gcc -Wall -g -o motor motor.c -lm

# phone target - delete built files (including OS X debug files)
clean:
	rm -rf handler motor *.o *~ handler.dSYM motor.dSYM
