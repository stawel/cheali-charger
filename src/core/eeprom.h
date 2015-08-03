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
#ifndef EEPROM_H_
#define EEPROM_H_

#include "AnalogInputs.h"
#include "ProgramData.h"
#include "Settings.h"
#include "cpu.h"

#define EEPROM_MAGIC_STRING_LEN 4

#define EEPROM_RESTORE_MAGIC_NUMBER         1
#define EEPROM_RESTORE_CALIBRATION          2
#define EEPROM_RESTORE_PROGRAM_DATA         4
#define EEPROM_RESTORE_SETTINGS             8


namespace eeprom {
    struct Data {
        uint8_t magicString[EEPROM_MAGIC_STRING_LEN];
        uint16_t architecture;
        uint16_t calibrationVersion;
        uint16_t programDataVersion;
        uint16_t settingVersion;

        AnalogInputs::Calibration calibration[AnalogInputs::PHYSICAL_INPUTS];
        uint16_t calibrationCRC;

        ProgramData::Battery battery[MAX_PROGRAMS];
        uint16_t batteryCRC;

        Settings settings;
        uint16_t settingsCRC;
    } CHEALI_EEPROM_PACKED;

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
    bool check();
    void restoreDefault();
#else
    inline bool check() { return true; /* OK */};
    inline void restoreDefault() {};
#endif


}
#endif //EEPROM_H_

