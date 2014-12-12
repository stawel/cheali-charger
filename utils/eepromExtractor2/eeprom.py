#!/usr/bin/python

import sys
from ehelper import *

from v9_2_6 import *

hex = 'eeprom.bin'

if len(sys.argv) > 1:
    hex = sys.argv[1]

x = Data()
f = open(hex, 'r')
f.readinto(x)


checkVersion(x, CHEALI_CHARGER_EPPROM_VERSION_STRING)

def checkAllCRC():
    checkCRC(x, 'calibration')
    checkCRC(x, 'programData')
    checkCRC(x, 'settings')


print dump(x)
checkAllCRC();

sys.exit()

#example

x.programData[5].name = 'zosia'
restoreCRC(x,'programData');
checkAllCRC();

f = open('output.bin', 'wb')
f.write(x);
