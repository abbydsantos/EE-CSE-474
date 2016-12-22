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

// main parameters

char **g_argv;

// threshold for each size
int thresholdFront = 1350;
int thresholdRight = 1350;
int thresholdLeft = 1350;
int thresholdBack = 1600;

// set up GPIO pins
int setUp();
void digitalWrite(FILE*, int);
void pwmaWrite(int);
void pwmbWrite(int);

// Motor functions

// make the machine go forward or backward
void drive(int, int);

// stops the machine
void stop();

// rotate clockwise or counterclockwise
void rotate(int, int);

// determine if the machine should go forward, backward,
// or rotate clockwise or counterclockwise
void nextStep(int, int, int, int, char*);

// handler stuff
struct sigaction sa;
struct itimerval timer;

// pipe for reading the sensor data
char *myfifo = "lab4pipe";

// pipe for reading the commands through bluetooth
char *bluetooth = "/dev/ttyO4";

// gpio pins
int gpin[5] = {48, 49, 112, 115, 20};

int frequency = 1000;

// file numbers for files
FILE *ain2, *ain1, *stby, *bin1, *bin2, *pwmb_duty, *pwma_duty;

// file to read from for bluetooth
FILE *bt;


#endif


