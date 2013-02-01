#!/bin/bash

set -x

HEX=`ls cheali-charger*.hex`
TTY=/dev/ttyUSB0

echo "HEX=$HEX"
echo "TTY=$TTY"
avrdude -patmega32 -cusbasp -P$TTY -Uflash:w:$HEX:a $*

#-C/home/stawel/aaa/arduino/arduino-1.0/hardware/tools/avrdude.conf 
