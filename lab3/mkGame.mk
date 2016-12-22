#Daisy X., Yunie Y., Abby S.
# default target
game: game.c game.h lcd.h
	gcc -o game game.c -lm
pipe: pipe.c
	gcc -o pipe pipe.c

# make all target
all:
	gcc -o game game.c -lm
	gcc -o pipe pipe.c

# phone target - delete built files (including OS X debug files)
clean:
	rm -rf game pipe *.o *~ game.dSYM
