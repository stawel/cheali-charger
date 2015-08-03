#!/bin/bash


CPU=nuvoton-M0517
TARGET=imaxB6-clone
GENERIC=50W

#CPU=atmega32
#TARGET=imaxB6-clone
#GENERIC=50W

#CPU=atmega32
#TARGET=GTPowerA6-10-original
#GENERIC=200W

PYTHON=python

#http://starship.python.net/crew/theller/ctypes/old/codegen.html
#http://svn.python.org/projects/ctypes/trunk/ctypeslib/
CTYPELIB=../../../ctypeslib/ctypeslib


CORE=../../src/core/
HARDWARE=../../src/hardware/$CPU/
HCONFIG=$HARDWARE/targets/$TARGET/
CONFIG=$HARDWARE/generic/$GENERIC/
OUTPUT=eeprom_tmp

#TODO:
if [ $CPU == atmega32 ]; then
    INCLUDE_CPU="-I /usr/lib/avr/include/"
else
    INCLUDE_CPU=
fi

INCLUDE="$INCLUDE_CPU -I $CORE/strategy -I $HARDWARE/cpu -I $CORE/drivers -I $HCONFIG -I $CORE  -I $CONFIG"
VARS='
-r CHEALI_CHARGER_EEPROM.*
-r CHEALI_CHARGER_EPPROM_VERSION_STRING
-r CHEALI_CHARGER_ARCHITECTURE
-r CHEALI_CHARGER_ARCHITECTURE_CPU
-r CHEALI_CHARGER_ARCHITECTURE_CPU_STRING
-r CHEALI_CHARGER_ARCHITECTURE_GENERIC
-r CHEALI_CHARGER_ARCHITECTURE_GENERIC_STRING
-r MAX_BANANCE_CELLS
'

$PYTHON $CTYPELIB/h2xml.py `pwd`/$CORE/eeprom.h `pwd`/$CORE/Version.h $INCLUDE -o $OUTPUT.xml --cpp-symbols
$PYTHON $CTYPELIB/xml2py.py $OUTPUT.xml -s Name -s Data  $VARS > $OUTPUT.py
EEPROM_VERSION=`echo "import $OUTPUT; print  'v_a'+hex($OUTPUT.CHEALI_CHARGER_ARCHITECTURE)+'_'+$OUTPUT.CHEALI_CHARGER_EPPROM_VERSION_STRING.replace('.','_'); " | $PYTHON`

echo "eeprom version: $EEPROM_VERSION"
mv "$OUTPUT.py" "ehelper/$EEPROM_VERSION.py"