#!/bin/bash

set -x

#maybe you should change this:
#TTY=-P/dev/ttyUSB0
TTY=
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
    avrdude -p$PARTNO -c$PROGRAMMER $TTY -Uflash:r:flash.hex:r -Ulfuse:r:lfuse.hex:r -Uhfuse:r:hfuse.hex:r -Ueeprom:r:eeprom.hex:r
    cd -
fi


avrdude -p$PARTNO -c$PROGRAMMER $TTY -Uflash:w:$HEX:a
