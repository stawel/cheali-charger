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

#include <cstring>
#include <sstream>

#include "extractor.h"
#include "../extractor.h"
#include "eeprom.h"

class EEPROM : public Extractor {
public:
    virtual std::string getName() const { return CHEALI_CHARGER_EPPROM_VERSION_STRING; }
    virtual int getSize() const { return sizeof(d);}
    virtual void load(const void *adr, int size) { std::memcpy(&d, adr, min(size, getSize())); }
    virtual bool isValid() const
    {
        return //d.magicString == "CLCR" &&
                d.calibrationVersion == CHEALI_CHARGER_EEPROM_CALIBRATION_VERSION &&
                d.programDataVersion == CHEALI_CHARGER_EEPROM_PROGRAMDATA_VERSION &&
                d.settingVersion     == CHEALI_CHARGER_EEPROM_SETTINGS_VERSION;
    }
    virtual std::string toString() const;

    eeprom::Data d;
};


Extractor * CREATE_GET(8, 1, 5)
{ return new EEPROM; }


//////toString
#define SETT(name) << "s_" CHEALI_CHARGER_STRING(name) "="<< d.settings.name << "\n"

#define CALIB_P(name, nr) \
    << "c_" CHEALI_CHARGER_STRING(name)  "__p" CHEALI_CHARGER_STRING(nr) "_x=" << d.calibration[name].p[nr].x << "\n" \
    << "c_" CHEALI_CHARGER_STRING(name)  "__p" CHEALI_CHARGER_STRING(nr) "_y=" << d.calibration[name].p[nr].y << "\n"
#define CALIB(name) CALIB_P(name, 0) CALIB_P(name, 1)

using namespace AnalogInputs;


std::string EEPROM::toString() const
{
    std::stringstream s;
    s 
    << "magicString=" << std::string((char *) d.magicString, MAGIC_STRING_LEN) <<"\n"
    << "calibrationVersion=" << d.calibrationVersion << "\n"
    << "programDataVersion=" << d.programDataVersion << "\n"
    << "settingVersion=" << d.settingVersion << "\n"
    << "\n"

//calibration
/*
    << "c_Vout_plus_pin__p0_x="<<d.calibration[Vout_plus_pin][0].x << "\n"
    << "c_Vout_plus_pin__p0_y="<<d.calibration[Vout_plus_pin][0].y << "\n"
    << "c_Vout_plus_pin__p1_x="<<d.calibration[Vout_plus_pin][1].x << "\n"
    << "c_Vout_plus_pin__p1_y="<<d.calibration[Vout_plus_pin][1].y << "\n"
*/
    CALIB(Vout_plus_pin)
    CALIB(Vout_minus_pin)
    CALIB(Ismps)
    CALIB(Idischarge)

    CALIB(VoutMux)
    CALIB(Tintern)
    CALIB(Vin)
    CALIB(Textern)

    CALIB(Vb0_pin)
    CALIB(Vb1_pin)
    CALIB(Vb2_pin)
    CALIB(Vb3_pin)
    CALIB(Vb4_pin)
    CALIB(Vb5_pin)
    CALIB(Vb6_pin)

#if MAX_BANANCE_CELLS > 6
    CALIB(Vb7_pin)
    CALIB(Vb8_pin)
#endif

    CALIB(IsmpsValue)
    CALIB(IdischargeValue)

    << "\n"
//settings
    SETT(backlight_)
    SETT(fanTempOn_)
    SETT(dischargeTempOff_)
    SETT(externT_)
    SETT(externTCO_)
    SETT(deltaT_)
    SETT(enable_deltaV_)
    SETT(deltaV_NiMH_)
    SETT(deltaV_NiCd_)
    SETT(CDcycles_)
    SETT(capCutoff_)
    SETT(inputVoltageLow_)
    SETT(dischargeOffset_LiXX_)
    SETT(dischargeAggressive_LiXX_)
    SETT(forceBalancePort_)
    SETT(balancerError_)
    SETT(UART_)
    SETT(UARTspeed_)
    ;
    return s.str();
}
