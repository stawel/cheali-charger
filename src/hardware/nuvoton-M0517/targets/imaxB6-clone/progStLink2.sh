#!/bin/bash

set -x

PROGRAMMER="interface/stlink-v2.cfg"
TARGET="target_MO517_linux.cfg"
FLASH_PGM="M0517_flash.tcl"

BIN=`ls *M0517.bin`
echo "BIN=$BIN"

CFDIR=`pwd`

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
    cd $DIR

    openocd -s $CFDIR -f $PROGRAMMER -f $TARGET -c "init; dump_image aprom.bin 0x0 0x10000; dump_image flash_data.bin 0x1f000 0x1000; shutdown " || exit_backup
    openocd -s $CFDIR -f $PROGRAMMER -f $TARGET -c "init; dump_image ldrom.bin  0x100000 0x1000; shutdown "
    openocd -s $CFDIR -f $PROGRAMMER -f $TARGET -c "init; dump_image config.bin 0x300000 0x1000; shutdown "
    cd -
fi

openocd -s `pwd` -f $PROGRAMMER -f $TARGET -f $FLASH_PGM -c "init; FlashAprom $BIN; reset run"
