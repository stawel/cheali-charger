#!/usr/bin/python

import sys
#sys.path.append('helper/')

from ehelper.ehelper import *
from dformatter import *
import importlib

binFile = 'eeprom.bin'

if len(sys.argv) > 1:
    binFile = sys.argv[1]


version = getVersion(binFile)
print 'version: ', version

module = importlib.import_module('ehelper.'+version);
printModuleInfo(module)

x = module.Data()
f = open(binFile, 'r')
f.readinto(x)

print dump(x)
checkAllCRC(x);


#save defaultCalibration.cpp file
defaultCalibrationFile = 'defaultCalibration.cpp'

fmt = DFormatter()
f = open(defaultCalibrationFile + '.template' + str(getCellNumber(module)), 'r')
template = f.read()
out = fmt.vformat(template, [],x)
f = open(defaultCalibrationFile, 'w')
f.write(out)


sys.exit()

#example deprecated

x.programData[5].name = 'zosia'
restoreCRC(x,'programData');
checkAllCRC();
f = open('output.bin', 'wb')
f.write(x);
