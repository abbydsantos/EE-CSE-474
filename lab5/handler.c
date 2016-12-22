// Daisy X., Yunie Y., Abby S.
#include "handler.h"

int main(int argc, char **argv) {
  ain = argv;
  timer_Init();
  while(1);

	return 0;
}

void adc_handler(int whatever) {
  // read from each adc port
  int val = readADC(1);
  if (val == -1) {
    exit(0);
  }
  avgLeft[i] = val;

  val = readADC(2);
  if (val == -1) {
    exit(0);
  }
  avgBack[i] = val;

  val = readADC(3);
  if (val == -1) {
    exit(0);
  }
  avgRight[i] = val;

  val = readADC(4);
  if (val == -1) {
    exit(0);
  }
  avgFront[i] = val;

  i = (i + 1) % 300;
  count++;

  if (count < 300) {
    return;
  }

  // get mean for each adc port
  int meanLeft = 0, meanBack = 0, meanRight = 0, meanFront = 0;
  int j;
  for (j = 0; j < 300; j++) {
    meanLeft += avgLeft[j];
    meanBack += avgBack[j];
    meanRight += avgRight[j];
    meanFront += avgFront[j];
  }

  meanLeft /= 300;
  meanBack /= 300;
  meanRight /= 300;
  meanFront /= 300;

  // connect to pipe and write
  int fd;
  mkfifo(myfifo, 0666);
  fd = open(myfifo, O_WRONLY);
  if (fd == -1) {
    printf("failed to open lab4pipe\n");
    exit(0);
  }
  // write to motor
  char buf[1000];
  sprintf(buf, "%d %d %d %d", meanFront, meanBack, meanLeft, meanRight);
  write(fd, buf, sizeof(buf));
  close(fd);
  memset(buf, 0, 1000);
  }
 
void timer_Init() {
  memset (&sa, 0, sizeof (sa));
  sa.sa_handler = &adc_handler;
  sigaction (SIGVTALRM, &sa, NULL);

  /* Configure the timer to expire after 3 msec... */
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = 3000;
  /* ... and every 3 msec after that. */
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 3000;
  /* Start a virtual timer. It counts down whenever this process is
    executing. */
  setitimer (ITIMER_VIRTUAL, &timer, NULL);
}

int readADC(unsigned int pin) {
  int fd;
  char buf[4];
  // open the correct ADC port
  if (pin == 1) {
    fd = open(ain[1], O_RDONLY);
  } else if (pin == 2) {
    fd = open(ain[2], O_RDONLY);
  } else if (pin == 3) {
    fd = open(ain[3], O_RDONLY);
  } else {
    fd = open(ain[4], O_RDONLY);
  }
  // if couldn't open, return -1
  if (fd < 0) {
    perror("adc port not open");
    close(fd);
    return -1;
  }
  // read from ADC port
  int rd = read(fd, &buf, 4);
  // if didn't read, return -1
  if (rd == 0) {
    perror("can't read\n");
    close(fd);
    return -1;
  }
  // close port
  close(fd);
  return atoi(buf);
}


