#!/bin/bash


PYTHON=python

#http://starship.python.net/crew/theller/ctypes/old/codegen.html
#http://svn.python.org/projects/ctypes/trunk/ctypeslib/
CTYPELIB=../../../ctypeslib/ctypeslib

CORE=../../src/core/
HARDWARE=../../src/hardware/nuvoton-M0517/
HCONFIG=$HARDWARE/targets/imaxB6-clone/
CONFIG=$HARDWARE/generic/50W/
OUTPUT=eeprom_tmp

INCLUDE="-I $CORE/strategy -I $HARDWARE/cpu -I $CORE/drivers -I $HCONFIG -I $CORE  -I $CONFIG"


$PYTHON $CTYPELIB/h2xml.py `pwd`/$CORE/eeprom.h `pwd`/$CORE/Version.h $INCLUDE -o $OUTPUT.xml --cpp-symbols
$PYTHON $CTYPELIB/xml2py.py $OUTPUT.xml -s Name -s Data -r CHEALI_CHARGER_EEPROM.* -r CHEALI_CHARGER_EPPROM_VERSION_STRING > $OUTPUT.py
EEPROM_VERSION=v`echo "import $OUTPUT; print $OUTPUT.CHEALI_CHARGER_EPPROM_VERSION_STRING.replace('.','_'); " | $PYTHON`

echo "eeprom version: $EEPROM_VERSION"
mv "$OUTPUT.py" "$EEPROM_VERSION.py"