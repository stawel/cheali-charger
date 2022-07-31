#!/bin/bash

set -x

M0517_flash_tools_PATH=../../../../../utils/M0517_flash_tools/tcl/
PROGRAMMER="interface/stlink-v2.cfg"
TARGET="target_MO517_linux.cfg"
FLASH_PGM="M0517_flash.tcl"

BIN=`ls *M0517.hex`
echo "BIN=$BIN"

[ ! -f $BIN ] && echo "file not found: $BIN" && exit 1


function exit_backup()
{
    echo "backup faild!"
    exit 1
}

if [ "$1" != "--no-backup" ]; then
    NOW=`date +"%F_%T"`
    DIR="backup/$NOW"
    echo "creating buckup: $DIR"
    mkdir -p $DIR

    openocd -s $M0517_flash_tools_PATH -f $PROGRAMMER -f $TARGET -c "init; dump_image $DIR/aprom.bin 0x0 0x10000; dump_image $DIR/flash_data.bin 0x1f000 0x1000; shutdown " || exit_backup
    openocd -s $M0517_flash_tools_PATH -f $PROGRAMMER -f $TARGET -c "init; dump_image $DIR/ldrom.bin  0x100000 0x1000; shutdown "
    openocd -s $M0517_flash_tools_PATH -f $PROGRAMMER -f $TARGET -c "init; dump_image $DIR/config.bin 0x300000 0x1000; shutdown "

fi

openocd -s $M0517_flash_tools_PATH -f $PROGRAMMER -f $TARGET -f $FLASH_PGM -c "init; FlashAprom $BIN; reset run; shutdown"

