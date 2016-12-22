#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define DUTY_ON 15000
#define DUTY_OFF 0

char *pwmpipe = "pwmpipe";
double beepfreq = 440;
FILE *duty;
FILE *rpipe;

