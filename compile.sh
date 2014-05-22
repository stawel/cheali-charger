#!/bin/sh

./clean
./bootstrap
make

rm hex/not_tested/*hex
mv src/hardware/*/*hex hex/not_tested


./clean
