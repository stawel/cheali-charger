#!/bin/bash

set -x

PROGRAMMER=interface/stlink-v2.cfg
TARGET=target/m051.cfg


BIN=`ls *.bin`
echo "BIN=$BIN"

openocd -f $PROGRAMMER  -f $TARGET -c "program $BIN verify reset 0x1f000"
