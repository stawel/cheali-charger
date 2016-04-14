from ctypes import *

STRING = c_char_p



# values for enumeration 'Name'
Vout_plus_pin = 0
Vout_minus_pin = 1
Ismps = 2
Idischarge = 3
VoutMux = 4
Tintern = 5
Vin = 6
Textern = 7
Vb0_pin = 8
Vb1_pin = 9
Vb2_pin = 10
Vb3_pin = 11
Vb4_pin = 12
Vb5_pin = 13
Vb6_pin = 14
Vb7_pin = 15
Vb8_pin = 16
IsmpsSet = 17
IdischargeSet = 18
VirtualInputs = 19
Vout = 20
Vbalancer = 21
VoutBalancer = 22
VobInfo = 23
VbalanceInfo = 24
Iout = 25
Pout = 26
Cout = 27
Eout = 28
deltaVout = 29
deltaVoutMax = 30
deltaTextern = 31
deltaLastCount = 32
Vb1 = 33
Vb2 = 34
Vb3 = 35
Vb4 = 36
Vb5 = 37
Vb6 = 38
Vb7 = 39
Vb8 = 40
LastInput = 41
Name = c_int # enum
class Data(Structure):
    pass
uint8_t = c_uint8
uint16_t = c_uint16
class Calibration(Structure):
    pass
class CalibrationPoint(Structure):
    pass
ValueType = uint16_t
CalibrationPoint._pack_ = 1
CalibrationPoint._fields_ = [
    ('x', ValueType),
    ('y', ValueType),
]
Calibration._fields_ = [
    ('p', CalibrationPoint * 2),
]
class Battery(Structure):
    pass
class N11ProgramData7Battery3DOT_0E(Union):
    pass
class N11ProgramData7Battery3DOT_03DOT_1E(Structure):
    pass
N11ProgramData7Battery3DOT_03DOT_1E._fields_ = [
    ('Vs_per_cell', uint16_t),
    ('balancerError', uint16_t),
]
class N11ProgramData7Battery3DOT_03DOT_2E(Structure):
    pass
int16_t = c_int16
N11ProgramData7Battery3DOT_03DOT_2E._fields_ = [
    ('enable_deltaV', uint16_t),
    ('deltaV', int16_t),
    ('deltaVIgnoreTime', uint16_t),
    ('deltaT', uint16_t),
    ('DCcycles', uint16_t),
]
N11ProgramData7Battery3DOT_0E._anonymous_ = ['_0', '_1']
N11ProgramData7Battery3DOT_0E._fields_ = [
    ('_0', N11ProgramData7Battery3DOT_03DOT_1E),
    ('_1', N11ProgramData7Battery3DOT_03DOT_2E),
]
Battery._pack_ = 1
Battery._anonymous_ = ['_0']
Battery._fields_ = [
    ('type', uint16_t),
    ('capacity', uint16_t),
    ('cells', uint16_t),
    ('Ic', uint16_t),
    ('Id', uint16_t),
    ('Vc_per_cell', uint16_t),
    ('Vd_per_cell', uint16_t),
    ('minIc', uint16_t),
    ('minId', uint16_t),
    ('time', uint16_t),
    ('enable_externT', uint16_t),
    ('externTCO', ValueType),
    ('enable_adaptiveDischarge', uint16_t),
    ('DCRestTime', uint16_t),
    ('capCutoff', uint16_t),
    ('_0', N11ProgramData7Battery3DOT_0E),
]
class Settings(Structure):
    pass
uint32_t = c_uint32
Settings._fields_ = [
    ('backlight', uint16_t),
    ('fanOn', uint16_t),
    ('fanTempOn', ValueType),
    ('dischargeTempOff', ValueType),
    ('audioBeep', uint16_t),
    ('minIc', ValueType),
    ('maxIc', ValueType),
    ('minId', ValueType),
    ('maxId', ValueType),
    ('inputVoltageLow', ValueType),
    ('adcNoise', uint16_t),
    ('UART', uint16_t),
    ('UARTspeed', uint16_t),
    ('UARToutput', uint16_t),
    ('menuType', uint16_t),
]
Data._pack_ = 1
Data._fields_ = [
    ('magicString', uint8_t * 4),
    ('architecture', uint16_t),
    ('architectureInfo', uint16_t),
    ('calibrationVersion', uint16_t),
    ('programDataVersion', uint16_t),
    ('settingVersion', uint16_t),
    ('calibration', Calibration * 19),
    ('calibrationCRC', uint16_t),
    ('battery', Battery * 20),
    ('batteryCRC', uint16_t),
    ('settings', Settings),
    ('settingsCRC', uint16_t),
]
CHEALI_CHARGER_ARCHITECTURE_INFO = 8 # Variable c_int '8'
CHEALI_CHARGER_ARCHITECTURE = 4098 # Variable c_int '4098'
CHEALI_CHARGER_EPPROM_VERSION_STRING = '10.3.11' # Variable STRING '(const char*)"10.3.11"'
CHEALI_CHARGER_ARCHITECTURE_GENERIC = 2 # Variable c_int '2'
CHEALI_CHARGER_ARCHITECTURE_CPU_STRING = 'atmega32' # Variable STRING '(const char*)"atmega32"'
CHEALI_CHARGER_ARCHITECTURE_GENERIC_STRING = '200W' # Variable STRING '(const char*)"200W"'
CHEALI_CHARGER_EEPROM_CALIBRATION_VERSION = 10 # Variable c_int '10'
CHEALI_CHARGER_ARCHITECTURE_CPU = 4096 # Variable c_int '4096'
MAX_BANANCE_CELLS = 8 # Variable c_int '8'
CHEALI_CHARGER_EEPROM_SETTINGS_VERSION = 11 # Variable c_int '11'
CHEALI_CHARGER_EEPROM_PROGRAMDATA_VERSION = 3 # Variable c_int '3'
__all__ = ['CHEALI_CHARGER_ARCHITECTURE_INFO',
           'N11ProgramData7Battery3DOT_03DOT_2E', 'Vb4',
           'Vout_minus_pin', 'IsmpsSet', 'Calibration',
           'Vout_plus_pin', 'Vb8_pin', 'Vb7_pin', 'Vb2', 'Vb1', 'Vb7',
           'Textern', 'Vb5', 'VirtualInputs', 'Vb8', 'deltaTextern',
           'CalibrationPoint', 'deltaLastCount', 'Vb5_pin', 'Ismps',
           'Vout', 'N11ProgramData7Battery3DOT_0E',
           'CHEALI_CHARGER_ARCHITECTURE_GENERIC', 'deltaVoutMax',
           'Vbalancer', 'CHEALI_CHARGER_ARCHITECTURE_CPU', 'Pout',
           'CHEALI_CHARGER_ARCHITECTURE', 'deltaVout', 'Vb1_pin',
           'Idischarge', 'MAX_BANANCE_CELLS', 'Data', 'Vb6_pin',
           'CHEALI_CHARGER_EEPROM_SETTINGS_VERSION',
           'N11ProgramData7Battery3DOT_03DOT_1E', 'Name', 'Settings',
           'IdischargeSet', 'Iout', 'Vb4_pin',
           'CHEALI_CHARGER_EEPROM_PROGRAMDATA_VERSION', 'LastInput',
           'Eout', 'VoutBalancer', 'Tintern', 'VbalanceInfo',
           'CHEALI_CHARGER_EPPROM_VERSION_STRING', 'Vb3', 'VoutMux',
           'Vb3_pin', 'Cout',
           'CHEALI_CHARGER_ARCHITECTURE_GENERIC_STRING', 'Battery',
           'CHEALI_CHARGER_ARCHITECTURE_CPU_STRING', 'Vin', 'Vb2_pin',
           'Vb0_pin', 'CHEALI_CHARGER_EEPROM_CALIBRATION_VERSION',
           'uint32_t', 'int16_t', 'uint8_t', 'uint16_t', 'ValueType',
           'VobInfo', 'Vb6']
