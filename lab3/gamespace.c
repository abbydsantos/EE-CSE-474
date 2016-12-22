//Daisy X., Abby S., Yunie Y.
#include "lcd.h"
#include "lcd_uspace.h"

//static unsigned int gpio

static int __init driver_entry(void) {

  int ret = alloc_chrdev_region(&dev_num, 0, 1, "Lab3");
  if (ret < 0) {
    printk(KERN_ALERT "lab3_dev: unable to cdev to kernel\n");
    return ret;
  }

  printk(KERN_INFO "lab3_dev: major number is %d\n", MAJOR(dev_num));
  printk(KERN_INFO "Use mknod /dev/%s c %d 0 for device file\n", "Lab3", MAJOR(dev_num));

  mcdev = cdev_alloc();
  mcdev->ops = &fops;
  mcdev->owner = THIS_MODULE;

  ret = cdev_add(mcdev, dev_num, 1);
  if (ret < 0) {
    printk(KERN_ALERT "la3_dev: unable to add cdev to kernel\n");
    return ret;
  }

  playerPlace = 0;
  easy = 1;
  place = 0;
  sema_init(&ports.sem, 1);
  ports.Rclk = 47;
  ports.SRclk = 69;
  ports.SRclr = 27; //low true 
  ports.OE = 46;    //low true 
  ports.SER = 26;
  ports.RS = 61;
  ports.RW = 65;
  ports.E = 49;
  ports.db3 = 67;
	
  initializeAll();
  return 0;
}

static void initializeAll(void) {
  // initialize GPIO ports
  initGPIO();

  // set RS and R/W to 0
  setRSRW(0, 0);
  msleep(20);
  sendChar(0x30);
  msleep(10);
  sendChar(0x30);
  msleep(10);
  sendChar(0x30);

  sendChar(0x38);
  // set Display off
  sendChar(0x08);
  // clear Display
  sendChar(0x01);

  // entry mode set
  sendChar(0x06);
  // Display on
  sendChar(0x0f);
  msleep(5000);

  // blinking off
  sendChar(0x0e);
}


static void initGPIO(void) {
  // export gpio pins
  gpio_export((unsigned) ports.RS, FALSE); 
  gpio_export((unsigned) ports.RW, FALSE); 
  gpio_export((unsigned) ports.E, FALSE);
  gpio_export((unsigned) ports.OE, FALSE);
  gpio_export((unsigned) ports.SER, FALSE);
  gpio_export((unsigned) ports.Rclk, FALSE);
  gpio_export((unsigned) ports.SRclk, FALSE);
  gpio_export((unsigned) ports.SRclr, FALSE);
  gpio_export((unsigned) ports.db3, FALSE);

  // set direction to out
  gpio_direction_output((unsigned) ports.RS, TRUE);
  gpio_direction_output((unsigned) ports.RW, TRUE);
  gpio_direction_output((unsigned) ports.E, TRUE);
  gpio_direction_output((unsigned) ports.OE, TRUE);
  gpio_direction_output((unsigned) ports.SER, TRUE);
  gpio_direction_output((unsigned) ports.Rclk, TRUE);
  gpio_direction_output((unsigned) ports.SRclk, TRUE);
  gpio_direction_output((unsigned) ports.SRclr, TRUE);
  gpio_direction_output((unsigned) ports.db3, TRUE);

  // initialize value
  gpio_set_value((unsigned) ports.OE, 1);
  gpio_set_value((unsigned) ports.SRclr, 0);
  gpio_set_value((unsigned) ports.RS, 0);
  gpio_set_value((unsigned) ports.RW, 0);
  gpio_set_value((unsigned) ports.E, 0);
  gpio_set_value((unsigned) ports.SER, 0);
  gpio_set_value((unsigned) ports.SRclk, 0);
  gpio_set_value((unsigned) ports.db3, 0);
}

static void sendChar(char command) {
  gpio_set_value((unsigned) ports.SRclr, 1);
  gpio_set_value((unsigned) ports.Rclk, 0);
  for (int i = 0; i < 8; i++) {
    if (i == 3) {
      gpio_set_value((unsigned) ports.db3, ((command >> i) & 1));
    }
    functionSet((command >> i) & 1);
    // wait one clock cycle
    gpio_set_value((unsigned) ports.SRclk, 1);
    gpio_set_value((unsigned) ports.SRclk, 0);
  }
  gpio_set_value((unsigned) ports.Rclk, 1);
  // turn output enable on and off
  gpio_set_value((unsigned) ports.OE, 0);
  msleep(10);
  // turn enable on and off
  gpio_set_value((unsigned) ports.E, 1);
  msleep(1);
  gpio_set_value((unsigned) ports.E, 0);
  gpio_set_value((unsigned) ports.OE, 1);
  // push from shift register to lcd monitor
  // clear shift register
  gpio_set_value((unsigned) ports.SRclr, 0);
  gpio_set_value((unsigned) ports.SRclr, 1);
}

static void functionSet(int command) {
  gpio_set_value((unsigned) ports.SER , command);
}

static void setRSRW(int rs, int rw) {
  gpio_set_value((unsigned) ports.RS, rs);
  gpio_set_value((unsigned) ports.RW, rw);
}

int moveUp(int place) {
  
  if (place > 15) {
printk(KERN_INFO "move up\n");
    place -= 16;
    sendChar(0x80);
    int i;
    for (i = 0; i < place; i++) {
      sendChar(SHIFT_CURSOR_RIGHT);
    }
    sendChar(0x0e);
  }
  return place;
}

int moveDown(int place) {

  if (place < 16) {
  printk(KERN_INFO "move down\n");
    int i;
    sendChar(0xc0);
    for (i = 0; i < place; i++) {
      sendChar(SHIFT_CURSOR_RIGHT);
    }
    sendChar(0x0e);
    place = place + 16;
  }
  return place;
}

int moveLeft(int place) {

  if (place != 16 && place != 0) {
  printk(KERN_INFO "move left\n");
    sendChar(SHIFT_CURSOR_LEFT);
    sendChar(0x0e);
    return place - 1;
  }
  return place;
}

int moveRight(int place) {

  if (place != 15 && place != 31) {
  printk(KERN_INFO "move right\n");
    sendChar(SHIFT_CURSOR_RIGHT);
    sendChar(0x0e);
    return place + 1;
  }
  return place;
}

// called up exit.
// unregisters the device and all associated gpios with it.
static void __exit driver_exit(void) {
  cdev_del(mcdev);
  unregister_chrdev_region(dev_num, 1);
  printk(KERN_ALERT "lcd: successfully unloaded\n");
}

// Called on device file open
// inode reference to file on disk, struct file represents an abstract
// checks to see if file is already open (semaphore is in use)
// prints error message if device is busy.
int device_open(struct inode *inode, struct file* filp) {
  if (down_interruptible(&ports.sem) != 0) {
    printk(KERN_ALERT "lcd: could not lock device during open\n");
    return -1;
  }
  printk(KERN_INFO "lcd: device opened\n");
  return 0;
}

// Called upon close
// closes device and returns access to semaphore.
int device_close(struct inode* inode, struct  file *filp) {
  up(&ports.sem);
  printk(KERN_INFO "lcd, closing device\n");
  return 0;	
}

// Called when user wants to get info from device file
ssize_t device_read(struct file* filp, char* bufStoreData, size_t bufCount, loff_t* curOffset) {
  printk(KERN_INFO "lcd: Reading from device...\n");
  return copy_to_user(bufStoreData, &ports.SER, bufCount);
}

// Called when user wants to send info to device
ssize_t device_write(struct file* filp, const char* bufSource, size_t bufCount, loff_t* curOffset) {
  printk(KERN_INFO "lcd: writing to device...\n");
  if (strcmp("CLEAR", bufSource) == 0) {
    setRSRW(0, 0);
    sendChar(0x01);
    sendChar(0x0e);
    playerPlace = 0;
    *curOffset += 6;
    return 6;
  } else {
    if (strcmp("a", bufSource) == 0) {
      playerPlace = moveLeft(playerPlace);
    } else if (strcmp("d", bufSource) == 0) {
      playerPlace = moveRight(playerPlace);
    } else if (strcmp("w", bufSource) == 0) {
      playerPlace = moveUp(playerPlace);
    } else if (strcmp("s", bufSource) == 0) {
      playerPlace = moveDown(playerPlace);
    } else if (strcmp("X", bufSource) == 0) {
      setRSRW(1, 0);
      sendChar('X');
      setRSRW(0, 0);
      sendChar(0x0c);
    } else {
      int i;
      setRSRW(1, 0);
      int limit = strlen(bufSource) - 1;
      for (i = 0; i < limit; i++) {
        sendChar(bufSource[i]);
      }
      setRSRW(0, 0);
      sendChar(0x0c);
      *curOffset += limit + 1;
      return limit + 1;
    }
    *curOffset += 2;

    return 2;
  }
  return 0;
}


MODULE_LICENSE("GPL"); // module license: required to use some functionalities.
module_init(driver_entry); // declares which function runs on init.
module_exit(driver_exit);  // declares which function runs on exit.
