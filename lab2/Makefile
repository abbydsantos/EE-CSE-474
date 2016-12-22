# default target
lcd: lcd.c lcd.h
	gcc -o lcd lcd.c

pipe: pipe.c
	gcc -o pipe pipe.c

game: game.c game.h lcd.h
	gcc -o game game.c

osc: osc.c lcd.h
	gcc -o osc osc.c

# make all target
all:
	gcc -o lcd lcd.c
	gcc -o pipe pipe.c
	gcc -o game game.c
	gcc -o osc osc.c

# phone target - delete built files (including OS X debug files)
clean:
	rm -rf lcd pipe game osc *.o *~ lcd.dSYM pipe.dSYM game.dSYM osc.dSYM
