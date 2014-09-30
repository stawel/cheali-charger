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

namespace eeprom {
    struct Data {
        uint8_t magicString[EEPROM_MAGIC_STRING_LEN];
        uint16_t calibrationVersion;
        uint16_t programDataVersion;
        uint16_t settingVersion;

        AnalogInputs::Calibration calibration[AnalogInputs::PHYSICAL_INPUTS];
        ProgramData programData[MAX_PROGRAMS];
        Settings settings;
    };
    extern Data data;
    void restoreDefault(bool force = false);
}
