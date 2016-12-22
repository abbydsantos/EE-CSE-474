#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>

#define DUTY_ON 15000
#define DUTY_OFF 0

int spots[5];

int pitch[16];

FILE *pwm;
FILE *duty;

void showSpots(int, int);

void initPWM();
