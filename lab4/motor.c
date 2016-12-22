// Daisy X., Yunie Y., Abby S.
#include "motor.h"
#include "pwm.h"

int main(int argc, char **argv) {
  int err = setUp();
  if (err == -1) {
    printf("Initialization error\n");
    return 1;
  }
  FILE *pwma_period = fopen("/sys/devices/ocp.3/pwm_test_P9_14.17/period", "w");
  FILE *pwmb_period = fopen("/sys/devices/ocp.3/pwm_test_P9_16.18/period", "w");
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
  fscanf(fd, "%d %d %d %d", &front, &back, &left, &right);
  fclose(fd);
  nextStep(front, back, left, right);
}

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

  system("echo bone_pwm_P9_14 > /sys/devices/bone_capemgr.9/slots");
  system("echo bone_pwm_P9_16 > /sys/devices/bone_capemgr.9/slots");
  usleep(500000);
  // open the files
  ain2 = fopen("/sys/class/gpio/gpio48/value", "w");
  ain1 = fopen("/sys/class/gpio/gpio49/value", "w");
  stby = fopen("/sys/class/gpio/gpio112/value", "w");
  bin1 = fopen("/sys/class/gpio/gpio115/value", "w");
  bin2 = fopen("/sys/class/gpio/gpio20/value", "w");
  pwma_duty = fopen("/sys/devices/ocp.3/pwm_test_P9_14.17/duty", "w");
  pwmb_duty = fopen("/sys/devices/ocp.3/pwm_test_P9_16.18/duty", "w");

  test = fopen("/sys/class/gpio/gpio60/value", "w");
	digitalWrite(test, 0);

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

void digitalWrite(FILE *file, int value) {
  fseek(file, 0, SEEK_SET);
  fprintf(file, "%d", value);
  fflush(file);
}

// do we need to change duty cycles?
void pwmaWrite(int speed) {
  digitalWrite(pwma_duty, speed);
}

void pwmbWrite(int speed) {
  digitalWrite(pwmb_duty, speed);
}

//forward:  direction == 1, backword: direction == 0

void drive(int direction, int speed) {
  if (direction) {
    digitalWrite(ain2, 0);
    digitalWrite(ain1, 1);

    digitalWrite(bin2, 1);
    digitalWrite(bin1, 0);
  } else {
    digitalWrite(ain2, 1);
    digitalWrite(ain1, 0);

    digitalWrite(bin2, 0);
    digitalWrite(bin1, 1);
  }
  pwmaWrite(speed);
  pwmbWrite(speed);
  digitalWrite(stby, 1);
}

void stop() {
  digitalWrite(ain2, 0);
  digitalWrite(ain1, 0);
  digitalWrite(bin2, 0);
  digitalWrite(bin1, 0);
}

//beaglebone at the head 
//channel a is left motor
//channel b is right motor
void rotate(int direction, int speed) {
  pwmaWrite(speed);
  pwmbWrite(speed);
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

void nextStep(int front, int back, int left, int right) {
  // if front is free, move forward
  if (front < thresholdFront) {
    drive(1, 300);
  } else if (right< thresholdRight) {
    // rotate clockwise, move forward
    rotate(0, 300);
    //drive(1, 30);
  } else if (left < thresholdLeft) {
    // rotate counterclockwise, move forward
    rotate(1, 300);
    //drive(1, 30);
  } else if (back < thresholdBack) {
    // move backward
    drive(0, 300);
  } else {
    stop();
  }
}


