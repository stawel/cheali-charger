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

namespace eeprom {
    Data data EEMEM;

    bool testWriteVersion(uint16_t * adr, uint16_t version) {
        if(eeprom::read(adr) == version)
            return false;
        eeprom::write(adr, version);
        return true;
    }

    void restoreDefault(bool force) {
        bool calib = false;

        Screen::displayStrings(PSTR("reseting eeprom:"),
                               PSTR("v: " CHEALI_CHARGER_EPPROM_VERSION_STRING " "));

        if(testWriteVersion(&data.calibrationVersion, CHEALI_CHARGER_EEPROM_CALIBRATION_VERSION) || force)  {
            lcdPrint_P(PSTR("c "));
            calib = force = true;
            AnalogInputs::restoreDefault();
        }
        if(testWriteVersion(&data.programDataVersion, CHEALI_CHARGER_EEPROM_PROGRAMDATA_VERSION) || force)  {
            lcdPrint_P(PSTR("d "));
            force = true;
            ProgramData::restoreDefault();
        }
        if(testWriteVersion(&data.settingVersion, CHEALI_CHARGER_EEPROM_SETTINGS_VERSION) || force)  {
            lcdPrint_P(PSTR("s "));
            force = true;
            Settings::restoreDefault();
        }

        if(force)
            Timer::delay(2000);

        if(calib)
            Screen::runCalibrateBeforeUse();
    }
}
