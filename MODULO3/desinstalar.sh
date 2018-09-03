#!/bin/sh
modulo="tercer_driver"
dispositivo="led_SIE"
mode="664"
/sbin/rmmod ./$modulo.ko $* || exit 1
rm -f /dev/${dispositivo}
