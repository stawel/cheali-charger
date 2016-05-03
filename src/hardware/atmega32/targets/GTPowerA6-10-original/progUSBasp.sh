#!/bin/bash

set -x

#maybe you should change this:
#TTY=-P/dev/ttyUSB0
TTY=
PROGRAMMER=usbasp
#---

PARTNO=atmega32

HEX=`ls *.hex | head -1`
echo "HEX=$HEX"
echo "TTY=$TTY"

if [ "$1" != "--no-backup" ]; then
    NOW=`date +"%F_%T"`
    DIR="backup/$NOW"
    echo "creating buckup: $DIR"
    mkdir -p $DIR
    cd $DIR
    avrdude -p$PARTNO -c$PROGRAMMER $TTY -Uflash:r:flash.bin:r -Ulfuse:r:lfuse.bin:r -Uhfuse:r:hfuse.bin:r -Ueeprom:r:eeprom.bin:r
    cd -
fi

if [ -f "$HEX" ]; then
    avrdude -p$PARTNO -c$PROGRAMMER $TTY -Uflash:w:$HEX:a
fi
