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
#include "Hardware.h"
#include "Settings.h"
#include "memory.h"
#include "Version.h"
#include "LcdPrint.h"
#include "eeprom.h"

#define CHARS_TO_UINT16(x,y) (((y)<< 8) + (x))

namespace eeprom {
    Data data EEMEM;

    bool testWriteVersion(uint16_t * adr, uint16_t version) {
        if(eeprom::read(adr) == version)
            return false;
        eeprom::write(adr, version);
        return true;
    }

    void calibrationDisplayMessage(bool calib, bool force)
    {
        if(force)
            Screen::runResettingEeprom();

        if(calib)
            Screen::runCalibrateBeforeUse();
    }

    void restoreDefault(bool force) {
        bool calib = false;

        if(testWriteVersion((uint16_t*) &data.magicString[0], CHARS_TO_UINT16('c','h')))  {
            calib = force = true;
        }
        if(testWriteVersion((uint16_t*) &data.magicString[2], CHARS_TO_UINT16('l','i')))  {
            calib = force = true;
        }

        if(testWriteVersion(&data.calibrationVersion, CHEALI_CHARGER_EEPROM_CALIBRATION_VERSION) || force)  {
            calib = force = true;
            AnalogInputs::restoreDefault();
        }
        if(testWriteVersion(&data.programDataVersion, CHEALI_CHARGER_EEPROM_PROGRAMDATA_VERSION) || force)  {
            force = true;
            ProgramData::restoreDefault();
        }
        if(testWriteVersion(&data.settingVersion, CHEALI_CHARGER_EEPROM_SETTINGS_VERSION) || force)  {
            force = true;
            Settings::restoreDefault();
        }

        calibrationDisplayMessage(calib, force);
    }
}
