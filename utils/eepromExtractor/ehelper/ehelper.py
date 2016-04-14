#!/bin/python

from ctypes import *
import crcmod
from io import BytesIO
import sys

def dump(s, tabs = 0, tab= '    '):
    retu = ''
    if isinstance(s, Structure):
        retu += s.__class__.__name__ + '(\n';
        tabs = tabs+1
        for field_name, field_type in s._fields_:
            retu += tab*tabs + str(field_name) + ' = ' + dump(getattr(s, field_name), tabs) + ',\n'
        retu += tab*(tabs-1) + ')'
    elif isinstance(s, Array):
        tabs = tabs+1
        retu += '(\n';
        for v in s:
            retu += tab*tabs + dump(v, tabs) + ',\n'
        retu +=  tab*(tabs-1) + ')'
    elif isinstance(s, str):
        retu += '\'' + str(s) + '\''
    else:
        retu += str(s)
    return retu

def checkHeader(d):
    s = ''.join([chr(x) for x in  d.magicString])
    print 'magic: ', s
    return s == 'chli'

def error(s):
    print s
    sys.exit()

def getVersion(filename):
    import v9_2_6
    d = v9_2_6.Data()
    f = open(filename, 'r')
    f.readinto(d)
    if not checkHeader(d):
        error('checkHeader error!')

    if(d.calibrationVersion < 10):
        return 'v' + str(d.calibrationVersion) + '_'  + str(d.programDataVersion) + '_' + str(d. settingVersion)
    else:
        import v_a0x4001_9_3_10
        d = v_a0x4001_9_3_10.Data()
        f = open(filename, 'r')
        f.readinto(d)
        if d.calibrationVersion == 9:
            return 'v_a' + hex(d.architecture) + '_' + str(d.calibrationVersion) + '_' + str(d.programDataVersion) + '_' + str(d. settingVersion)
        else:
            import v_a0x1002_i0x6_10_3_11
            d = v_a0x1002_i0x6_10_3_11.Data()
            f = open(filename, 'r')
            f.readinto(d)
            return 'v_a' + hex(d.architecture) + '_i'+ hex(d.architectureInfo) + '_' + str(d.calibrationVersion) + '_' + str(d.programDataVersion) + '_' + str(d. settingVersion)


def printModuleInfo(module):
    if hasattr(module, 'CHEALI_CHARGER_ARCHITECTURE'):
        print 'architecture: '+hex(module.CHEALI_CHARGER_ARCHITECTURE)
        print 'cpu: ', module.CHEALI_CHARGER_ARCHITECTURE_CPU_STRING
        print 'generic: ', module.CHEALI_CHARGER_ARCHITECTURE_GENERIC_STRING
    else:
        print 'no module info'

def getCellNumber(module):
    if hasattr(module, 'CHEALI_CHARGER_ARCHITECTURE_INFO'):
        return module.CHEALI_CHARGER_ARCHITECTURE_INFO;
    else:
        return 6


def dumpRaw(d):
    s = BytesIO()
    s.write(d)
    s.seek(0)
    return s.read()

def CRC(d):
#    crc16 = crcmod.mkCrcFun(0x18005, initCrc=0xffff, rev=True)
    crc16 = crcmod.predefined.mkCrcFun('modbus')
    crc =  crc16(dumpRaw(d))
    return crc;

def checkAllCRC(x):
    checkCRC(x, 'calibration')
    if hasattr(x, 'programData'):
        checkCRC(x, 'programData')
    else:
        checkCRC(x, 'battery')
    checkCRC(x, 'settings')


def checkCRC(d, field):
    data = getattr(d, field)
    dataCRC = getattr(d, field + 'CRC')
    if CRC(data) != dataCRC:
        error(field + ' CRC error: ' + str(CRC(data)) + ' != ' + str(dataCRC))

def restoreCRC(d, field):
    data = getattr(d, field)
    setattr(d, field + 'CRC', CRC(data))
