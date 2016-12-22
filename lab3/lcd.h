#ifndef _LCD_H
#define _LCD_H

#define SHIFT_CURSOR 0x14

#define TRUE 1
#define FALSE 0

 
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/random.h>

//Daisy X., Abby S., Yunie Y.
// gpio pin num for RS, R/W, E, data bits, clock and clear
/*const unsigned int RS = 61;
const unsigned int RW = 65;
const unsigned int E = 49;
const unsigned int dataAB = 48;
const unsigned int clk = 47;
const unsigned int clr = 27;*/

int pos;


struct lcd_ports {
	int Rclk;
	int SRclk;
	int SRclr; //low true 
	int OE;    //low true 
	int SER;
	int RS;
	int RW;
	int E;
        int db3;

	struct semaphore sem;
} ports;

// stores info about this char device.
static struct cdev* mcdev;

// holds major and minor number granted by the kernel
static dev_t dev_num;

/* function prototypes */
// file operations
static int __init driver_entry(void);
static void __exit driver_exit(void);
static int  device_open(struct inode*, struct file*);
static int device_close(struct inode*, struct file *);
static ssize_t device_read(struct file*, char*, size_t, loff_t*);
static ssize_t device_write(struct file*, const char*, size_t, loff_t*);

// initalize GPIO pins and LCD display
// return 1 if successful, -1 if not
static void initializeAll(void);

// initialize GPIO pins
static void initGPIO(void);

// closes all GPIO ports in lcdPort
//static void closePort(void);

// set instruction code
static void functionSet(int);

// send char
static void sendChar(char);

// set RS and R/W
static void setRSRW(int, int);


/* operations usable by this file. */
static struct file_operations fops = {
   .owner = THIS_MODULE,
   .read = device_read,
   .write = device_write,
   .open = device_open,
   .release = device_close
};
#endif
