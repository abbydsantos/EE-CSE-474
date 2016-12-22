// Daisy X., Abby S., Yunie Y.
#include "pwm.h"

int main(int argc, char **argv) {
  system("echo am33xx_pwm > /sys/devices/bone_capemgr.9/slots");
  system("echo bone_pwm_P8_19 > /sys/devices/bone_capemgr.9/slots");

  duty = fopen("/sys/devices/ocp.3/pwm_test_P8_19.16/duty", "w");
  if (duty == NULL) {
    printf("failed to open duty\n");
    return -1;
  }
  FILE *pwmperiod = fopen("/sys/devices/ocp.3/pwm_test_P8_19.16/period", "w");
  if (pwmperiod == NULL) {
    printf("failed to open period\n");
    return -1;
  }
  int period = (int) ((1 / beepfreq) * pow(10.0, 9));
  fseek(pwmperiod, 0, SEEK_SET);
  fprintf(pwmperiod, "%d", period);
  fflush(pwmperiod);
  fclose(pwmperiod);
  

  while(1) {
    rpipe = fopen(pwmpipe, "r");
    if (rpipe == NULL) {
      printf("failed to open pipe\n");
      return -1;
    }
    int front;
    fscanf(rpipe, "%d", &front);
    fclose(rpipe);
    if (front < 1600) {
      continue;
    }
    front++;
    int sleep = 1000000000 / front;
    // turn duty off
    fseek(duty, 0, SEEK_SET);
    fprintf(duty, "%d", DUTY_ON);
    fflush(duty);
    // wait
    usleep(sleep);
    // turn duty on
    fseek(duty, 0, SEEK_SET);
    fprintf(duty, "%d", DUTY_OFF);
    fflush(duty);
    usleep(sleep);
  }
  //fclose(rpipe);
  fclose(duty);
  return 0;
}

