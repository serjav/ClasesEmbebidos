#!/bin/sh
modulo="segundo_driver" #Nombre del modulo
dispositivo="led_SIE" #Nombre del dispositivo
/sbin/rmmod ./$modulo.ko $* || exit 1
rm -f /dev/${dispositivo}
