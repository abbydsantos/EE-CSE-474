#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "lcd.h"

int main(int argc, char** argv) {
	initializeAll(); 
  while (1) {

	fseek(lcdPort[2], 0, SEEK_SET);
        fprintf(lcdPort[2], "%d", 1);
  	fflush(lcdPort[2]);
	usleep(300);
	fseek(lcdPort[2], 0, SEEK_SET);
        fprintf(lcdPort[2], "%d", 0);
  	fflush(lcdPort[2]);
	usleep(300); 
  }

  return 0;
}

int initializeAll() {
  usleep(500000);  
// initialize GPIO ports
  initGPIO();
  // check if GPIO ports got initialized correctly
  int err = checkGPIO();
  if (err == -1) {
    return err;
  }
  
  zeroOut();
  usleep(20000);
  // set RS and R/W to 0
  setRSRW(0, 0);
  // usleep(1);
  functionSet(0x30);

  //0000 1100
  //0123 4567
  usleep(41000);
  functionSet(0x30);
  usleep(1000);
  functionSet(0x30);
  usleep(10000);
  functionSet(0x38);
  // set Display off
  functionSet(0x08);
  // clear Display
  functionSet(0x01);

  // entry mode set
  functionSet(0x06);
  // Display on
  functionSet(0x0f);

  usleep(500000);
  // blinking off
  functionSet(0x0e);
  return 1;
}


void initGPIO() {
  // initialize GPIO pins
  int i;
  FILE *export = fopen("/sys/class/gpio/export", "w");
  for (i = 0; i < 11; i++) {
    fseek(export, 0, SEEK_SET);
    fprintf(export, "%d", pinNum[i]);
    fflush(export);
  }
  fclose(export);

  // set GPIO to output
  char set[strlen("/sys/class/gpio/gpio") + strlen("/direction") + 4];
  for (i = 0; i < 11; i++) {
    sprintf(set, "/sys/class/gpio/gpio%d", pinNum[i]);
    strcat(set, "/direction");
    FILE *gpioDir = fopen(set, "w");
    fprintf(gpioDir, "%s", "out");
    fflush(gpioDir);
    fclose(gpioDir);
  }

  char value[strlen("/sys/class/gpio/gpio") + strlen("/value") + 4];
  // open the gpio value files, stores in lcdPort array
  for (i = 0; i < 11; i++) {
    sprintf(value, "/sys/class/gpio/gpio%d", pinNum[i]);
    strcat(value, "/value");
    FILE *file = fopen(value, "w");
    lcdPort[i] = file;
  }
}

int checkGPIO() {
  int i;
  for (i = 0; i < 11; i++) {
    if (lcdPort[i] == NULL) {
      return -1;
    }
  }
  return 1;
}

void closePort() {
  int i;
  for (i = 0; i < 11; i++) {
    fclose(lcdPort[i]);
  }
}

void functionSet(char command) {
  int i;
  // sets DB7-DB0
  for (i = 3; i < 11; i++) {
    // get bit for correct corresponding value
    int value = (command >> (10 - i)) & 1;
    // send the value to the correct GPIO pin port
    fseek(lcdPort[i], 0, SEEK_SET);
    fprintf(lcdPort[i], "%d", value);
    fflush(lcdPort[i]);
  }
  // set enable on
  fseek(lcdPort[2], 0, SEEK_SET);
  fprintf(lcdPort[2], "%d", 1);
  fflush(lcdPort[2]);
  // delay
  usleep(1000);
  // set enable off
  fseek(lcdPort[2], 0, SEEK_SET);
  fprintf(lcdPort[2], "%d", 0);
  fflush(lcdPort[2]);
}

void setRSRW(int rs, int rw) {
  fseek(lcdPort[0], 0, SEEK_SET);
  fprintf(lcdPort[0], "%d", rs);
  fflush(lcdPort[0]);
  fseek(lcdPort[1], 0, SEEK_SET);
  fprintf(lcdPort[1], "%d", rw);
  fflush(lcdPort[1]);
}

void zeroOut() {
  // zero out commands
  int i;
  for (i = 0; i < 11; i++) {
    fseek(lcdPort[i], 0, SEEK_SET);
    fprintf(lcdPort[i], "%d", 0);
    fflush(lcdPort[i]);
  }
}
