#!/bin/bash

INPUT="$1"
OUTPUT="$2"
echo "INPUT=$INPUT"
echo "OUTPUT=$OUTPUT"

SIZE_PARAMETER="--format=avr --mcu=atmega32"
#test if avr-size is patched to support --format=avr and mcu parameter
avr-size --format=avr --mcu=atmega32 "$INPUT" > /dev/null 2>/dev/null
if [ $? -ne 0 ]; then
	SIZE_PARAMETER="--format=sysv"
fi

avr-gcc -v 2>&1 | tee "$OUTPUT"
avr-size $SIZE_PARAMETER "$INPUT" | tee -a "$OUTPUT"

true

