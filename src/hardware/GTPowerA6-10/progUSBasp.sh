#!/bin/bash

set -x

#maybe you should change this:
TTY=/dev/ttyUSB0
PROGRAMMER=usbasp
#---

PARTNO=atmega32

HEX=`ls cheali-charger*.hex`
echo "HEX=$HEX"
echo "TTY=$TTY"

if [ "$1" != "--no-backup" ]; then
    NOW=`date +"%F_%T"`
    DIR="backup/$NOW"
    echo "creating buckup: $DIR"
    mkdir -p $DIR
    cd $DIR
    avrdude -p$PARTNO -c$PROGRAMMER -P$TTY -Uflash:r:flash.hex:i -Ulfuse:r:lfuse.hex:i -Uhfuse:r:hfuse.hex:i -Ueeprom:r:eeprom.hex:i
    cd -
fi


avrdude -p$PARTNO -c$PROGRAMMER -P$TTY -Uflash:w:$HEX:a
