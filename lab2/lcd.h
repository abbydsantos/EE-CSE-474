#ifndef _LCD_H
#define _LCD_H

#define SHIFT_CURSOR 0x14

// initalize GPIO pins and LCD display
// return 1 if successful, -1 if not
int initializeAll();

// initialize GPIO pins
void initGPIO();

// closes all GPIO ports in lcdPort
void closePort();

// check if all GPIO port got initialized correctly
// return 1 if initialized correctly, -1 if not
int checkGPIO();

// set instruction code
void functionSet(char);

// set RS and R/W
void setRSRW(int, int);

// zero out all the GPIO ports
void zeroOut();

// array of GPIO pins used
const int pinNum[11] = {61, 65, 49, 47, 45, 69, 66, 20, 112, 60, 48};

//Pin 117 indicative of potential suckage

// array of file pointers
/* 0 = RS
   1 = RW
   2 = E
   3 = DB7
   4 = DB6
   5 = DB5
   6 = DB4
   7 = DB3
   8 = DB2
   9 = DB1
   10 = DB0
*/
FILE *lcdPort[11];


#endif // _LCD_H_
