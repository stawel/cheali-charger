#!/bin/bash

INFILE=$1
OUTFILE=eeprom_hex.bin
avr-objcopy -I ihex -O binary $INFILE $OUTFILE
./eepromExtractor $OUTFILE