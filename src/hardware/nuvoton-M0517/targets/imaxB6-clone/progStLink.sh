#!/bin/bash

set -x

PROGRAMMER=interface/stlink-v2.cfg
TARGET=target/m051.cfg


HEX=`ls *.hex`
echo "HEX=$HEX"

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
    openocd -f $PROGRAMMER  -f $TARGET -c "init; dump_image aprom.bin 0x0 0x10000; dump_image flash_data.bin 0x1f000 0x1000; shutdown " || exit_backup
    openocd -f $PROGRAMMER  -f $TARGET -c "init; dump_image ldrom.bin  0x100000 0x1000; shutdown "
    openocd -f $PROGRAMMER  -f $TARGET -c "init; dump_image config.bin 0x300000 0x1000; shutdown "
    cd -
fi

openocd -f $PROGRAMMER  -f $TARGET -c "program $HEX verify reset"
