from ctypes import *

STRING = c_char_p


class Data(Structure):
    pass
uint8_t = c_uint8
uint16_t = c_uint16
class Calibration(Structure):
    pass
class CalibrationPoint(Structure):
    pass
ValueType = uint16_t
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
__all__ = ['ClassNiZn', 'Calibration', 'VIdle', 'ClassNiXX',
           'ClassPb', 'VCharge', 'int16_t',
           'CHEALI_CHARGER_EEPROM_CALIBRATION_VERSION',
           'CalibrationPoint', 'BatteryData', 'uint8_t',
           'ProgramData', 'VDischarge', 'ValueType', 'ValidEmpty',
           'Data', 'BatteryClass', 'LAST_VOLTAGE_TYPE', 'VStorage',
           'Settings', 'VoltageType',
           'CHEALI_CHARGER_EEPROM_SETTINGS_VERSION',
           'CHEALI_CHARGER_EEPROM_PROGRAMDATA_VERSION',
           'CHEALI_CHARGER_EPPROM_VERSION_STRING', 'uint32_t',
           'ClassLiXX', 'uint16_t']
