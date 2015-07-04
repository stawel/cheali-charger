#!/bin/bash


IN=$1

avr-objcopy -I ihex -O binary $IN $IN.bin