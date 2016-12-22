# default target
music: music.o
	gcc -o music music.o -lm

# individual source files
music.o: music.c
	gcc -c music.c -lm

# clean built files
clean:
	rm -rf music *.o *~ music.dSYM
