#!/bin/sh
modulo="tercer_driver"
dispositivo="led_SIE"
mode="664"
/sbin/insmod ./$modulo.ko $* || exit 1
mayor=$(awk '{if($2=="led") print $1 ; else print No_existe}' /proc/devices)
mknod /dev/${dispositivo} c $mayor 0
