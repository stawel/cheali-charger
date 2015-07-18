#!/bin/bash


OUTPUT=$1
FIND_DIR=../src/hardware

[ ! -d $OUTPUT ] && echo "OUTPUT: $OUTPUT is not a directory"

for FILE in `find $FIND_DIR -iname '*.hex'`
do
    echo "file: $FILE"
    cp $FILE $OUTPUT
done

for FILE in `find $FIND_DIR -iname '*.bin'`
do
    echo "file: $FILE"
    cp $FILE $OUTPUT
done

./gen_sha1.sh $OUTPUT
