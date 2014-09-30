/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013  Paweł Stawicki. All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "AnalogInputs.h"
#include "ProgramData.h"
#include "Settings.h"

#define EEPROM_MAGIC_STRING_LEN 4

#define EEPROM_RESTORE_MAGIC_STRING         1
#define EEPROM_RESTORE_CALIBRATION_VERSION  2
#define EEPROM_RESTORE_PROGRAM_DATA_VERSION 4
#define EEPROM_RESTORE_SETTING_VERSION      8
#define EEPROM_RESTORE_CALIBRATION          16
#define EEPROM_RESTORE_PROGRAM_DATA         32
#define EEPROM_RESTORE_SETTINGS             64


namespace eeprom {
    struct Data {
        uint8_t magicString[EEPROM_MAGIC_STRING_LEN];
        uint16_t calibrationVersion;
        uint16_t programDataVersion;
        uint16_t settingVersion;

        AnalogInputs::Calibration calibration[AnalogInputs::PHYSICAL_INPUTS];
        uint16_t calibrationCRC;

        ProgramData programData[MAX_PROGRAMS];
        uint16_t programDataCRC;

        Settings settings;
        uint16_t settingsCRC;
    };
    extern Data data;

#ifdef ENABLE_EEPROM_CRC
    bool restoreCalibrationCRC(bool restore = true);
    bool restoreProgramDataCRC(bool restore = true);
    bool restoreSettingsCRC(bool restore = true);
#else
    inline bool restoreCalibrationCRC(bool restore = true)  { return false; }
    inline bool restoreProgramDataCRC(bool restore = true)  { return false; }
    inline bool restoreSettingsCRC(bool restore = true)     { return false; }
#endif

#ifdef ENABLE_EEPROM_RESTORE_DEFAULT
    void check();
    void restoreDefault();
#else
    inline void check() {};
    inline void restoreDefault() {};
#endif


}
