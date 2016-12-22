#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include "lcd.h"
#include "game.h"

int main(int argc, char** argv) {
  int err;
  // initialize GPIO ports
  err = initializeAll();
  if (err == -1) {
    return 1;
  }

  char pipe[] = "mypipe";
  int fd = open(pipe, O_RDONLY);
  usleep(50000);
  int easy = 1;
  showSpots(easy);
  
  // clear display and set cursor on
  functionSet(0x01);
  usleep(1000);
  functionSet(0x0e);
  usleep(1000);
  functionSet(0x80);
  int playerPlace = 0;
  int place = 0;
  int characters = 0;
  char buf[1000];
  while ((place <= 3 && easy == 1) | (place <= 5 && easy == 0)) {
    //if Easy cleared
    if (place == 3 && easy == 1) {
      functionSet(0x01);
      usleep(1000);
      setRSRW(1, 0);
      functionSet('L');
      functionSet('V');
      functionSet('L');
      functionSet(' ');
      functionSet('D');
      functionSet('O');
      functionSet('N');
      functionSet('E');
      place = 0;
      playerPlace = 0;
      easy = 0;
      usleep(5000000);
      functionSet(0x01);
      usleep(1000);
      showSpots(easy);
      functionSet(0x01);
      usleep(1000);
      functionSet(0x0e);
      usleep(1000);
      functionSet(0x80);
      continue;
  }
    //if Hard cleared
    if (place == 5 && easy == 0) {
      functionSet(0x01);
      usleep(1000);
      setRSRW(1, 0);
      functionSet('G');
      functionSet('G');
      functionSet(' ');
      functionSet('B');
      functionSet('R');
      functionSet('U');
      functionSet('H');
      break;
  }
    //if still playing
    read(fd, buf, 1000);
    if (buf[0] == 'a') {
      playerPlace = moveLeft(playerPlace);
    }
    if (buf[0] == 'd') {
      playerPlace = moveRight(playerPlace);
    }
    if (buf[0] == 'w') {
      playerPlace = moveUp(playerPlace);
    }
    if (buf[0] == 's') {
      playerPlace = moveDown(playerPlace);
    }
    if (buf[0] == 'y') {
      if (spots[place] == playerPlace) {
        place++;
      } else {
        functionSet(0x01);
        usleep(1000);
        setRSRW(1, 0);
        functionSet('Y');
        functionSet('o');
        functionSet('u');
        functionSet(' ');
        functionSet('s');
        functionSet('u');
        functionSet('c');
        functionSet('k');
        break;
      }
    }
    memset(buf, 0, 1);
  }
  
  close(fd);
  closePort();
  unlink(pipe);
  return 0;
}

void showSpots(int easy) {
  int k = 0;
  srand(time(0));
  while ((k < 3 && easy == 1) | (k < 5 && easy == 0)) {
    int num = rand() % 56;
    if (num < 16 || (num >= 40 && num < 56)) {
      spots[k] = num;
      k++;
    }
  }
  int i = 0;
  setRSRW(0, 0);
  int max = 0;
  if (easy == 1)
    max = 3;
  else
    max = 5;
  for (i; i < max; i++) {
    // clear display
    functionSet(0x01);
    usleep(100);
    int j;
    for (j = 0; j < spots[i]; j++) {
      functionSet(SHIFT_CURSOR_RIGHT);
    }
    setRSRW(0, 0);
    // blinking on
    functionSet(0x0f);

    usleep(3000000);
    // blinking off
    functionSet(0x0e);
  }
}

int initializeAll() {
  usleep(5000);  
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

int moveUp(int place) {
  if (place - 40 >= 0) {
    int i;
    for (i = 0; i < 40; i++) {
      functionSet(SHIFT_CURSOR_LEFT);
    }
    usleep(1000);
    return place - 40;
  }
  return place;
}
int moveDown(int place) {
  if (place + 40 < 80) {
    int i;
    for (i = 0; i < 40; i++) {
      functionSet(SHIFT_CURSOR_RIGHT);
    }
    usleep(1000);
    place = place + 40;
  }
  return place;
}

int moveLeft(int place) {
  if (place != 40 && place != 0) {
    functionSet(SHIFT_CURSOR_LEFT);
    usleep(1000);
    return place - 1;
  }
  return place;
}

int moveRight(int place) {
  if (place != 15 && place != 55) {
    functionSet(SHIFT_CURSOR_RIGHT);
    usleep(1000);
    return place + 1;
  }
  return place;
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

