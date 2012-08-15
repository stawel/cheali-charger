#!/bin/bash

INPUT="$1"
OUTPUT="$2"
echo "INPUT=$INPUT"
echo "OUTPUT=$OUTPUT"
avr-size --format=avr "$INPUT" | tee "$OUTPUT"
