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

def checkVersion(d, str_version):
    if not checkHeader(d):
        error('checkHeader error!')
    v =  '' + str(d.calibrationVersion) + '.'  + str(d.programDataVersion) + '.' + str(d. settingVersion)
    print 'version:', v
    if str_version != v:
        error('checkVersion: ' + v +' != ' + str_version)

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


def checkCRC(d, field):
    data = getattr(d, field)
    dataCRC = getattr(d, field + 'CRC')
    if CRC(data) != dataCRC:
        error(field + ' CRC error: ' + str(CRC(data)) + ' != ' + str(dataCRC))

def restoreCRC(d, field):
    data = getattr(d, field)
    setattr(d, field + 'CRC', CRC(data))
