#!/bin/sh

./clean
./bootstrap
make

rm hex/not_tested/*hex
rm hex/not_tested/idnus/*hex
mv src/hardware/*/*idnus*.hex hex/not_tested/idnus
mv src/hardware/*/*hex hex/not_tested


./clean
