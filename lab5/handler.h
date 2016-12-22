  // Daisy X., Yunie Y., Abby S.
#ifndef HANDLER_H
#define HANDLER_H

#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DUTY_ON 15000
#define DUTY_OFF 0

// pipe name
char *myfifo = "lab4pipe";

// main parameters
char **ain;

// array that holds the previous 300 readings from the adc
int avgLeft[300], avgBack[300], avgRight[300], avgFront[300];

// check if any of adc port average past the threshold
// if did, tell the master program by changing pastThreshold to 1
int pastFront = 0;
int pastLeft = 0;
int pastRight = 0;
int pastBack = 0;
// count of how many time read from adc
int count = 0;
// index where next reading will be put in
int i = 0;

// handler stuff
struct sigaction sa;
struct itimerval timer;

// reading the data from sensor and outputting to the pipe
void adc_handler(int);

// set the timer for the interrupt
void timer_Init();

// read data from sensor and return it
int readADC(unsigned int);

#endif

