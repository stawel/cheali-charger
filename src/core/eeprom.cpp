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

    bool testWriteVersion(int * adr, int version) {
        if(eeprom::read(adr) == version)
            return false;
        eeprom::write(adr, version);
        return true;
    }
    

    void restoreDefault(bool force) {
        bool calib = false;

        
        
        if(testWriteVersion( &data.EEPROMidentification1, 67) || force)  {  //C char
            calib = force = true;
            restoreDefaultAll();
        }
        if(testWriteVersion( &data.EEPROMidentification2, 72) || force)  {  // H char
            calib = force = true;
            restoreDefaultAll();
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

      calibrationDisplaymessage(calib, force);
        
    }



void restoreDefaultAll()
    {
           resetDisplay();
           AnalogInputs::restoreDefault();
           ProgramData::restoreDefault();
           Settings::restoreDefault();
    }
    
    void calibrationDisplaymessage(bool calib, bool force)
    {
        if(force)
            resetDisplay();
            Timer::delay(2000);

        if(calib)
            Screen::runCalibrateBeforeUse();
    
    }
    
    void resetDisplay()
    {
        Screen::displayStrings(PSTR("reseting eeprom:"),
                               PSTR("v: " CHEALI_CHARGER_EPPROM_VERSION_STRING " "));
        
    }


}
