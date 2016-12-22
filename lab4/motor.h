// Daisy X., Yunie Y., Abby S.
#ifndef MOTOR_H
#define MOTOR_H

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

#define SIG_USER 20

void timer_Init();
void sig_handler(int);
int pid;

// threshold for each size
int thresholdFront = 1500;
int thresholdRight = 1500;
int thresholdLeft = 1500;
int thresholdBack = 1800;

// set up GPIO pins
int setUp();
void digitalWrite(FILE*, int);
void pwmWrite(int);

// Motor functions
void drive(int, int);
void stop();
void rotate(int, int);
void nextStep(int, int, int, int);

// handler stuff
struct sigaction sa;
struct itimerval timer;

// pipe name
char *myfifo = "lab4pipe";

// gpio pins
int gpin[5] = {48, 49, 112, 115, 20};

int frequency = 1000; 

// file numbers for files
FILE *ain2, *ain1, *stby, *bin1, *bin2, *pwmb_duty, *pwma_duty, *test;


#endif

