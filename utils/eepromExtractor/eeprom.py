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


#remove this line for "edit" example
sys.exit()

#example, edit battery: 6

x.battery[5].type = module.NiMH
x.battery[5].Ic = 1111
x.battery[5].capacity = 2222
x.battery[5].cells = 3
x.battery[5].enable_deltaV = 1
x.battery[5].deltaV = 8


restoreCRC(x,'battery');
checkAllCRC(x);
f = open('output.bin', 'wb')
f.write(x);
