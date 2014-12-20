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
IsmpsSet = 15
IdischargeSet = 16
VirtualInputs = 17
Vout = 18
Vbalancer = 19
VoutBalancer = 20
VobInfo = 21
VbalanceInfo = 22
Iout = 23
Pout = 24
Cout = 25
Eout = 26
deltaVout = 27
deltaVoutMax = 28
deltaTextern = 29
deltaLastCount = 30
Vb1 = 31
Vb2 = 32
Vb3 = 33
Vb4 = 34
Vb5 = 35
Vb6 = 36
LastInput = 37
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
class ProgramData(Structure):
    pass
class BatteryData(Structure):
    pass
BatteryData._pack_ = 1
BatteryData._fields_ = [
    ('type', uint16_t),
    ('C', uint16_t),
    ('Ic', uint16_t),
    ('Id', uint16_t),
    ('cells', uint16_t),
    ('time', uint16_t),
]

# values for enumeration 'VoltageType'
VIdle = 0
VCharge = 1
VDischarge = 2
VStorage = 3
ValidEmpty = 4
LAST_VOLTAGE_TYPE = 5
VoltageType = c_int # enum
int16_t = c_int16

# values for enumeration 'BatteryClass'
ClassNiXX = 0
ClassPb = 1
ClassLiXX = 2
ClassNiZn = 3
BatteryClass = c_int # enum
ProgramData._fields_ = [
    ('battery', BatteryData),
    ('name', c_char * 14),
]
class Settings(Structure):
    pass
uint32_t = c_uint32
Settings._fields_ = [
    ('backlight', uint16_t),
    ('fanTempOn', ValueType),
    ('dischargeTempOff', ValueType),
    ('externT', uint16_t),
    ('externTCO', ValueType),
    ('deltaT', ValueType),
    ('enable_deltaV', uint16_t),
    ('deltaV_NiMH', int16_t),
    ('deltaV_NiCd', int16_t),
    ('Rwire', ValueType),
    ('DCcycles', uint16_t),
    ('DCRestTime', uint16_t),
    ('audioBeep', uint16_t),
    ('minIoutDiv', uint16_t),
    ('minIout', uint16_t),
    ('capCutoff', uint16_t),
    ('inputVoltageLow', ValueType),
    ('overCharge_LiXX', int16_t),
    ('overDischarge_LiXX', int16_t),
    ('dischargeAggressive_LiXX', uint16_t),
    ('forceBalancePort', uint16_t),
    ('balancerError', ValueType),
    ('UART', uint16_t),
    ('UARTspeed', uint16_t),
    ('UARTinput', uint16_t),
    ('calibratedState', uint16_t),
    ('SMPS_Upperbound_Value', uint16_t),
    ('DISCHARGER_Upperbound_Value', uint16_t),
]
Data._pack_ = 1
Data._fields_ = [
    ('magicString', uint8_t * 4),
    ('calibrationVersion', uint16_t),
    ('programDataVersion', uint16_t),
    ('settingVersion', uint16_t),
    ('calibration', Calibration * 17),
    ('calibrationCRC', uint16_t),
    ('programData', ProgramData * 30),
    ('programDataCRC', uint16_t),
    ('settings', Settings),
    ('settingsCRC', uint16_t),
]
CHEALI_CHARGER_EPPROM_VERSION_STRING = '9.2.6' # Variable STRING '(const char*)"9.2.6"'
CHEALI_CHARGER_EEPROM_PROGRAMDATA_VERSION = 2 # Variable c_int '2'
CHEALI_CHARGER_EEPROM_CALIBRATION_VERSION = 9 # Variable c_int '9'
CHEALI_CHARGER_EEPROM_SETTINGS_VERSION = 6 # Variable c_int '6'
__all__ = ['Vout_minus_pin', 'IsmpsSet', 'Calibration',
           'Vout_plus_pin', 'VIdle', 'ClassNiXX', 'Vb4', 'ClassPb',
           'Vb3', 'Vb2', 'Vb1', 'int16_t', 'Textern', 'Vb5',
           'VirtualInputs', 'deltaTextern', 'CalibrationPoint',
           'deltaLastCount', 'Vb5_pin', 'Ismps', 'BatteryData',
           'ClassNiZn', 'Vout', 'ProgramData', 'VDischarge',
           'deltaVoutMax', 'ValidEmpty', 'Vbalancer', 'Pout',
           'deltaVout', 'Vb1_pin', 'Idischarge', 'Data', 'Vb6_pin',
           'CHEALI_CHARGER_EEPROM_SETTINGS_VERSION', 'BatteryClass',
           'LAST_VOLTAGE_TYPE', 'Name', 'Settings', 'IdischargeSet',
           'VoltageType', 'Iout', 'Vb4_pin',
           'CHEALI_CHARGER_EEPROM_PROGRAMDATA_VERSION', 'LastInput',
           'Eout', 'VoutBalancer', 'Tintern', 'VbalanceInfo',
           'CHEALI_CHARGER_EPPROM_VERSION_STRING', 'VoutMux',
           'Vb3_pin', 'Cout', 'VCharge', 'Vin', 'Vb2_pin', 'Vb0_pin',
           'CHEALI_CHARGER_EEPROM_CALIBRATION_VERSION', 'VStorage',
           'uint32_t', 'ClassLiXX', 'uint8_t', 'uint16_t',
           'ValueType', 'VobInfo', 'Vb6']
