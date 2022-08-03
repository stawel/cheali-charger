#!/bin/bash

set -x

M0517_flash_tools_PATH=../../../../../utils/M0517_flash_tools/tcl/
PROGRAMMER="interface/stlink-v2.cfg"
TARGET="target_MO517_linux.cfg"
FLASH_PGM="M0517_flash.tcl"

openocd -s $M0517_flash_tools_PATH -f $PROGRAMMER -f $TARGET -f $FLASH_PGM -c "init; reset run; shutdown"

