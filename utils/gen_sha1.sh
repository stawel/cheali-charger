#!/bin/bash


OUTPUT=$1

[ -d $OUTPUT ] && cd $OUTPUT

for FILE in *.hex *.bin
do
    echo "file: $FILE"
    SHA1_FILE=$FILE.sha1
    echo "sha1 file $SHA1_FILE"
    sha1sum $FILE > $SHA1_FILE
    cat $SHA1_FILE
done

[ -d $OUTPUT ] && cd -
