// Daisy X., Yunie Y., Abby S.
#include "motor.h"

int main(int argc, char **argv) {
  g_argv = argv;

  int err = setUp();
  if (err == -1) {
    printf("Initialization error\n");
    return 1;
  }
  // open the pwm period files
  FILE *pwma_period = fopen(g_argv[3], "w");
  FILE *pwmb_period = fopen(g_argv[4], "w");

  // set period for the pwm
  digitalWrite(pwma_period, frequency);
  digitalWrite(pwmb_period, frequency);
  fclose(pwma_period);
  fclose(pwmb_period);
 
  timer_Init();
  while(1);

	return 0;
}

void timer_Init() {
  memset (&sa, 0, sizeof (sa));
  sa.sa_handler = &sig_handler;
  sigaction (SIGVTALRM, &sa, NULL);

  /* Configure the timer to expire after 1 sec... */
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = 3000;
  /* ... and every 3 msec after that. */
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 3000;
  /* Start a virtual timer. It counts down whenever this process is
    executing. */
  setitimer (ITIMER_VIRTUAL, &timer, NULL);
}

void sig_handler(int sig) {
  int front, back, left, right;
  FILE *fd = fopen(myfifo, "r");
  // read sensor data from the pipe
  fscanf(fd, "%d %d %d %d", &front, &back, &left, &right);
  fclose(fd);
  // read the command from bluetooth
  bt = fopen("/dev/ttyO4", "r");
  char command[128];
  fread(command, 1, 1, bt);
  fclose(bt);
  nextStep(front, back, left, right, command);
}

// set up the pins for the motor and the pwm
int setUp() {
  int i;

  // export gpio pins
  char export[strlen("echo ") + 4 + strlen(" > /sys/class/gpio/export")];
  for (i = 0; i < 5; i++) {
    sprintf(export, "echo %d > /sys/class/gpio/export", gpin[i]);
    system(export);
  }

	sprintf(export, "echo %d > /sys/class/gpio/export", 60);
	system(export);

  // set direction to out
  char direct[strlen("echo out > /sys/class/gpio/gpio") + 4 + strlen("/direction")];
  for (i = 0; i < 5; i++) {
    sprintf(direct, "echo out > /sys/class/gpio/gpio%d/direction", gpin[i]);
    system(direct);
  }

  sprintf(direct, "echo out > /sys/class/gpio/gpio%d/direction", 60);
  system(direct);

  system("echo am33xx_pwm > /sys/devices/bone_capemgr.9/slots");
  system("echo bone_pwm_P9_14 > /sys/devices/bone_capemgr.9/slots");
  system("echo bone_pwm_P9_16 > /sys/devices/bone_capemgr.9/slots");
  usleep(500000);
  // open the files
  ain2 = fopen("/sys/class/gpio/gpio48/value", "w");
  ain1 = fopen("/sys/class/gpio/gpio49/value", "w");
  stby = fopen("/sys/class/gpio/gpio112/value", "w");
  bin1 = fopen("/sys/class/gpio/gpio115/value", "w");
  bin2 = fopen("/sys/class/gpio/gpio20/value", "w");

  pwma_duty = fopen(g_argv[1], "w");
  pwmb_duty = fopen(g_argv[2], "w");

  // check if open correctly
  // return -1 if don't
  if (ain2 == NULL) {
    printf("Error: GPIO for AIN2 failed to initialized\n");
    return -1;
  }
  if (ain1 == NULL) {
    printf("Error: GPIO for AIN1 failed to initialized\n");
    return -1;
  }
  if (stby == NULL) {
    printf("Error: GPIO for STBY failed to initialized\n");
    return -1;
  }
  if (bin1 == NULL) {
    printf("Error: GPIO for BIN1 failed to initialized\n");
    return -1;
  }
  if (bin2 == NULL) {
    printf("Error: GPIO for BIN2 failed to initialized\n");
    return -1;
  }
  if (pwma_duty == NULL) {
    printf("Error: PWM for PWMA_DUTY failed to initialized\n");
    return -1;
  }
  if (pwmb_duty == NULL) {
    printf("Error: PWM for PWMB_DUTY failed to initialized\n");
    return -1;
  }


  return 0;
}

// write the commands to the file
void digitalWrite(FILE *file, int value) {
  fseek(file, 0, SEEK_SET);
  fprintf(file, "%d", value);
  fflush(file);
}

void pwmaWrite(int speed) {
  // set the speed of the wheel
  digitalWrite(pwma_duty, speed);
}

void pwmbWrite(int speed) {
  // set the speed of the wheel
  digitalWrite(pwmb_duty, speed);
}

//forward:  direction == 1, backword: direction == 0

void drive(int direction, int speed) {
  if (direction) {
    // go forward if direction is 1
    digitalWrite(ain2, 0);
    digitalWrite(ain1, 1);

    digitalWrite(bin2, 1);
    digitalWrite(bin1, 0);
  } else {
    // else go backward
    digitalWrite(ain2, 1);
    digitalWrite(ain1, 0);

    digitalWrite(bin2, 0);
    digitalWrite(bin1, 1);
  }
  // set speed for the 2 wheels
  pwmaWrite(1.3 * speed);
  pwmbWrite(1 * speed);
  digitalWrite(stby, 1);
}

void stop() {
  // stop the wheels
  digitalWrite(ain2, 0);
  digitalWrite(ain1, 0);
  digitalWrite(bin2, 0);
  digitalWrite(bin1, 0);
}

//beaglebone at the head 
//channel a is left motor
//channel b is right motor
void rotate(int direction, int speed) {
  pwmaWrite(1.3 * speed);
  pwmbWrite(1 * speed);
  if (direction == 1) {
    //rotates counterclockwise
    //channel a = ccw
    //channel b = cw
    digitalWrite(ain2, 1);
    digitalWrite(ain1, 0);

    digitalWrite(bin2, 1);
    digitalWrite(bin1, 0);
  }
  else {
    //rotates clockwise
    //channel a = cw
    //channel b = ccw
    digitalWrite(ain2, 0);
    digitalWrite(ain1, 1);  

    digitalWrite(bin2, 0);
    digitalWrite(bin1, 1);
  }
  digitalWrite(stby, 1);
}

void nextStep(int front, int back, int left, int right, char *command) {
  if (strcmp(command, "5") == 0) {
    // if front is free, move forward
    if (front < thresholdFront) {
      drive(1, 300);
    } else if (right< thresholdRight) {
      // rotate clockwise
      rotate(0, 300);
    } else if (left < thresholdLeft) {
      // rotate counterclockwise
      rotate(1, 300);
    } else if (back < thresholdBack) {
      // move backward
      drive(0, 300);
    } else {
      stop();
    }
  } else {
    // if command is 1, go forward
    if (strcmp(command, "1") == 0) {
      drive(1, 300);
    } else if (strcmp(command, "2") == 0) {
      // else if command is 2, go backward
      drive(0, 300);
    } else if (strcmp(command, "3") == 0) {
      // if command is 3, rotate clockwise
      rotate(0, 300);
    } else if (strcmp(command, "4") == 0) {
      // if command is 4, rotate counterclockwise
      rotate(1, 300);
    } else {
      // else command is 0 and stop the machine
      stop();
    }
  }
}




