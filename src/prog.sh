#!/bin/bash

set -x

HEX="cheali-charger.hex"
echo "HEX=$HEX"

avrdude -patmega32 -cavrisp -P/dev/ttyUSB0 -b38400 -Uflash:w:$HEX:a

#-C/home/stawel/aaa/arduino/arduino-1.0/hardware/tools/avrdude.conf 
