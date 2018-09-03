#!/bin/sh
modulo="segundo_driver" #Nombre del modulo
dispositivo="led_SIE" #Nombre del dispositivo
mode="664" #Numero del nodo
/sbin/insmod ./$modulo.ko $* || exit 1
mayor=$(awk '{if($2=="led") print $1 ; else print No_existe}' /proc/devices)
mknod /dev/${dispositivo} c $mayor 0
