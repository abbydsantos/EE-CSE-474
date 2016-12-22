#!/bin/bash

sleep 5s
cd ~

echo cape-bone-iio > /sys/devices/bone_capemgr.9/slots
echo am33xx_pwm > /sys/devices/bone_capemgr.9/slots
echo bone_pwm_P9_14 > /sys/devices/bone_capemgr.9/slots
echo bone_pwm_P9_16 > /sys/devices/bone_capemgr.9/slots
echo BB-UART4 > /sys/devices/bone_capemgr.9/slots

DUTY1=$`ls /sys/devices/ocp.3 | grep pwm_test_P9_14.*`
DUTY1=${DUTY1:1}
PATH1=$"/sys/devices/ocp.3/$DUTY1/duty"
PER1=$"/sys/devices/ocp.3/$DUTY1/period"
echo $PATH1


DUTY2=$`ls /sys/devices/ocp.3 | grep pwm_test_P9_16.*`
DUTY2=${DUTY2:1}
PATH2=$"/sys/devices/ocp.3/$DUTY2/duty"
PER2=$"/sys/devices/ocp.3/$DUTY2/period"
echo $PATH2

PATH3=$`ls /sys/devices/ocp.3 | grep helper.*`
PATH3=${PATH3:1}
AIN4=$"/sys/devices/ocp.3/$PATH3/AIN4"
AIN6=$"/sys/devices/ocp.3/$PATH3/AIN6"
AIN2=$"/sys/devices/ocp.3/$PATH3/AIN2"
AIN0=$"/sys/devices/ocp.3/$PATH3/AIN0"
echo $PATH3
echo $AIN4
echo $AIN6
echo $AIN2
echo $AIN0

sleep 1s

#make
gcc -Wall -g -o motor motor.c -lm
gcc -Wall -g -o handler handler.c -lm

./handler  $AIN4 $AIN6 $AIN2 $AIN0 &
./motor $PATH1 $PATH2 $PER1 $PER2

