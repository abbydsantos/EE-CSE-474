#include "game.h"

int main(int argc, char **argv) {
  char wfile[] = "/dev/game";
  int wfd = open(wfile, O_RDWR);
  char pipe[] = "mypipe";
  int rfd = open(pipe, O_RDONLY);
  if (wfd == -1) {
    printf("wfd didn't work\n");
    return 1;
  }
  if (rfd == -1) {
    printf("rfd didn't work\n");
    return 1;
  }

  initPWM();
  int playerPlace = 0;
  int place = 0;
  int characters = 0;
  int easy = 1;
  char buf[1024];
  showSpots(easy, wfd);
  strcpy(buf, "GAME START\n");
  write(wfd, buf, strlen(buf));
  usleep(5000000);
  strcpy(buf, "CLEAR");
  write(wfd, buf, strlen(buf));

  while ((place <= 3 && easy == 1) | (place <= 5 && easy == 0)) {
    //if Easy cleared
    if (place == 3 && easy == 1) {
      strcpy(buf, "CLEAR");
      write(wfd, buf, strlen(buf));
      strcpy(buf, "LVL CLEAR\n");
      write(wfd, buf, strlen(buf));
      place = 0;
      playerPlace = 0;
      easy = 0;
      usleep(5000000);
      strcpy(buf, "CLEAR");
      write(wfd, buf, strlen(buf));
      usleep(1000);
      showSpots(easy, wfd);
      write(wfd, buf, strlen(buf));
      continue;
  }
    //if Hard cleared
    if (place == 5 && easy == 0) {
      strcpy(buf, "CLEAR");
      write(wfd, buf, strlen(buf));
      strcpy(buf, "GG NO REGRETS\n");
      write(wfd, buf, strlen(buf));
      break;
  }
    //if still playing
    read(rfd, buf, 1000);
    char wbuf[100];
    if (buf[0] == 'a') {
      if (playerPlace != 0 && playerPlace != 16) {
        strcpy(wbuf, "a");
        write(wfd, wbuf, strlen(wbuf));
        playerPlace--;
      }
    }
    if (buf[0] == 'd') {
      if (playerPlace != 15 && playerPlace != 31) {
        strcpy(wbuf, "d");
        write(wfd, wbuf, strlen(wbuf));
        playerPlace++;
      }
    }
    if (buf[0] == 'w') {
      if (playerPlace > 15) {
        strcpy(wbuf, "w");
        write(wfd, wbuf, strlen(wbuf));
        playerPlace -= 16;
      }
    }
    if (buf[0] == 's') {
      if (playerPlace < 16) {
        strcpy(wbuf, "s");
        write(wfd, wbuf, strlen(wbuf));
        playerPlace += 16;
      }
    }
    if (buf[0] == 'p') {
      fseek(pwm, 0, SEEK_SET);
      fprintf(pwm, "%d", pitch[playerPlace % 16]);
	    fflush(pwm);
      fseek(duty, 0, SEEK_SET);
      fprintf(duty, "%d", DUTY_ON);
	    fflush(duty);
      usleep(1000000);
      fseek(duty, 0, SEEK_SET);
      fprintf(duty, "%d", DUTY_OFF);
	    fflush(duty);
    }
    if (buf[0] == 'y') {
      if (spots[place] == playerPlace) {
        place++;
      } else {
        strcpy(wbuf, "CLEAR");
        write(wfd, wbuf, strlen(wbuf));
        memset(wbuf, 0, strlen(wbuf));
        strcpy(wbuf, "You suck\n");
        write(wfd, wbuf, strlen(wbuf));
        break;
      }
    }
    memset(buf, 0, 1);
  }
  
  close(wfd);
  close(rfd);
  unlink(pipe);
  return 0;
}

void showSpots(int easy, int wfd) {
  int k = 0;
  srand(time(0));
  while ((k < 3 && easy == 1) | (k < 5 && easy == 0)) {
    int num = rand() % 32;
    spots[k] = num;
    k++;
  }
  int i = 0;
  int max = easy == 1 ? 3 : 5;
  char buf[100];
  for (i; i < max; i++) {
    // clear display
    strcpy(buf, "CLEAR");
    write(wfd, buf, strlen(buf));
    usleep(100);
    if (spots[i] > 15) {
      strcpy(buf, "s");
      write(wfd, buf, 1);
    }
    int j;
    int numPlace = spots[i] > 15 ? spots[i] - 16 : spots[i];
    for (j = 0; j < numPlace; j++) {
      strcpy(buf, "d");
      write(wfd, buf, 1);
    }
    strcpy(buf, "X");
    write(wfd, buf, strlen(buf));
    fseek(pwm, 0, SEEK_SET);
    fprintf(pwm, "%d", pitch[spots[i] % 16]);
	  fflush(pwm);
    fseek(duty, 0, SEEK_SET);
    fprintf(duty, "%d", DUTY_ON);
	  fflush(duty);
    usleep(3000000);
    fseek(duty, 0, SEEK_SET);
    fprintf(duty, "%d", DUTY_OFF);
	  fflush(duty);
    strcpy(buf, "CLEAR");
    write(wfd, buf, strlen(buf));
  }
  fseek(pwm, 0, SEEK_SET);
  fprintf(pwm, "%d", 0);
	fflush(pwm);
  strcpy(buf, "CLEAR");
  write(wfd, buf, strlen(buf));
}

void initPWM() {
  // set the duty
	duty = fopen("/sys/devices/ocp.3/pwm_test_P9_14.15/duty", "w");
	fprintf(duty, "%d", DUTY_OFF);
	fflush(duty);

  pwm = fopen("/sys/devices/ocp.3/pwm_test_P9_14.15/period", "w");
  double freq;
	int nsfreq;
  int i;
  for (i = 0; i < 16; i++) {
	  freq =  pow(2.0, ((double) i/12)) * 220;
	  nsfreq = (int) ((1.0/freq) * pow(10.0, 9));
    pitch[i] = nsfreq;
  }
}
