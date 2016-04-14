#!/bin/bash


OUTPUT=$1
FIND_DIR=../src/hardware


mkdir $OUTPUT
[ ! -d $OUTPUT ] && echo "OUTPUT: $OUTPUT is not a directory"

git rm  $OUTPUT/cheali-charger*


cd ..
git clean -dxf
./bootstrap-arm
make
cd -

mkdir $OUTPUT
./copy_hex.sh $OUTPUT

git add $OUTPUT/cheali-charger*

cd ..
git clean -dxf
./bootstrap-avr
make
cd -

./copy_hex.sh $OUTPUT
git add $OUTPUT/cheali-charger*


