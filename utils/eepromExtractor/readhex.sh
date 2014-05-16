#!/bin/bash

INFILE=$1
OUTFILE=eeprom_hex.bin
VALUES=./dump.txt

avr-objcopy -I ihex -O binary $INFILE $OUTFILE
./eepromExtractor $OUTFILE > $VALUES
./createDefaultCalibration
