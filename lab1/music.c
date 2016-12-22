#include <stdio.h>
#include <math.h>
#include <unistd.h>

//btdubs using duty of 25000

int main(int argc, char** argv) {
	printf("start");
	int count;
	int note;
	double freq;
	int duty = 25000;
	int nsfreq;

	//initialize gpio ports 1, 2, and 3, and the pwm port

	FILE *port1 = fopen("/sys/class/gpio/export", "w");
	fprintf(port1, "%d", 20);
	fflush(port1);
	fclose(port1);

	FILE *port2 = fopen("/sys/class/gpio/export", "w");
	fprintf(port2, "%d", 48);
	fflush(port2);
	fclose(port2);

	FILE *port3 = fopen("/sys/class/gpio/export", "w");
	fprintf(port3, "%d", 49);
	fflush(port3);
	fclose(port3);

        // set the duty
	FILE *duty1A = fopen("/sys/devices/ocp.3/pwm_test_P9_14.15/duty", "w");
	fprintf(duty1A, "%d", duty);
	fflush(duty1A);
	fclose(duty1A);


	while(1){
		usleep(500000);
		count++;

	//if we want port 1 on

	if(count & 4){
		FILE *port1val = fopen("/sys/class/gpio/gpio20/value", "w");
		fprintf(port1val, "%d", 1);
		fflush(port1val);
		fclose(port1val);

		FILE *port1dir = fopen("/sys/class/gpio/gpio20/direction", "w");
		fprintf(port1dir, "%s", "out");
		fflush(port1dir);
		fclose(port1dir);
	}

	//if we want port 2 on
	if(count & 2){
		FILE *port2val = fopen("/sys/class/gpio/gpio48/value", "w");
		fprintf(port2val, "%d", 1);
		fflush(port2val);
		fclose(port2val);

		FILE *port2dir = fopen("/sys/class/gpio/gpio48/direction", "w");
		fprintf(port2dir, "%s", "out");
		fflush(port2dir);
		fclose(port2dir);
	}

	//if we want port 3 on
	if(count & 1){
		FILE *port3val = fopen("/sys/class/gpio/gpio49/value", "w");
		fprintf(port3val, "%d", 1);
		fflush(port3val);
		fclose(port3val);

		FILE *port3dir = fopen("/sys/class/gpio/gpio49/direction", "w");
		fprintf(port3dir, "%s", "out");
		fflush(port3dir);
		fclose(port3dir);
	}

        // set the period and plays the note

	note = count & 7;
	freq =  pow(2.0, ((double) note/12)) * 440;
	nsfreq = (int) ((1.0/freq) * pow(10.0, 9));

	FILE *period1A = fopen("/sys/devices/ocp.3/pwm_test_P9_14.15/period", "w");
	fprintf(period1A, "%d", nsfreq);
	fflush(period1A);
	fclose(period1A);

	}
	return 0;
}
