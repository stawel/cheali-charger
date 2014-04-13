#!/bin/bash


for FILE in *.hex
do
    echo "file: $FILE"
    SHA1_FILE=`basename $FILE .hex`.sha1
    echo "sha1 file $SHA1_FILE"
    sha1sum $FILE > $SHA1_FILE
    cat $SHA1_FILE
done