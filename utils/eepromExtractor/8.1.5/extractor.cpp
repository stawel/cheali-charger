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
#include <iomanip>
#include <iostream>

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


// some magic

#define MAGIC 0xea
void __reset16(uint16_t **x) {
    **x = (MAGIC<<8) + MAGIC;
}
void __reset8(uint8_t **x) {
    **x = MAGIC;
}

void __reset_name(char **x) {
    char * name = *x;
    for(int i=0;i<PROGRAM_DATA_MAX_NAME;i++) name[i] = MAGIC;
}

void  testIntegrity(void * data, int size)
{
    int ok = 0;
    for(int i = 0; i < size; i++) {
        unsigned char * d = ((unsigned char*)data)+i;
        if(*d == MAGIC) ok++;
        else {
            std::cerr << "ERROR adr: " << i << " = " << (unsigned int)*(unsigned char*)d <<  "\n";
        }
    }
    std::cerr << " ok = " << ok << " size = " << size << "\n";
}

#define SETT(name) \
    uint16_t *svar ## name  __attribute__((__cleanup__(__reset16))) = & d.settings.name; \
    s << "s_" CHEALI_CHARGER_STRING(name) "="<< d.settings.name << "\n"

#define CALIB_P(name, nr, str, div) \
    uint16_t *xvar ## nr ## name  __attribute__((__cleanup__(__reset16))) = & d.calibration[name].p[nr].x; \
    uint16_t *yvar ## nr ## name  __attribute__((__cleanup__(__reset16))) = & d.calibration[name].p[nr].y; \
    s << "c_p" CHEALI_CHARGER_STRING(nr) "_x__" CHEALI_CHARGER_STRING(name) "=" << * xvar ## nr ## name << "\n" \
    << "c_p" CHEALI_CHARGER_STRING(nr) "_y__" CHEALI_CHARGER_STRING(name) "='ANALOG_" str "("<< ((float) * yvar ## nr ## name )/1000 << ")'\n";
#define CALIB_ALL(name, str, div) CALIB_P(name, 0, str, div) CALIB_P(name, 1, str, div)
#define CALIB_V(name) CALIB_ALL(name, "VOLT", 1000)
#define CALIB_I(name) CALIB_ALL(name, "AMP", 1000)
#define CALIB_T(name) CALIB_ALL(name, "CELCIUS", 100)


#define PRINT_C(name, var, reset) \
    auto v_ ## name  __attribute__((__cleanup__(reset))) =  &var; \
    s << CHEALI_CHARGER_STRING(name) "=" << var << "\n";

#define PRINT16(name, var) PRINT_C(name, var, __reset16)

#define PROGDATA_NAME(i) \
    char * dbnvar ## name  __attribute__((__cleanup__(__reset_name))) =  d.programData[i].name; \
    s << "d_" << i << "_name='"<< d.programData[i].name << "'\n"


#define PROGDATA_BATTERY_(i, name, reset)\
    auto *dbvar ## name  __attribute__((__cleanup__(reset))) = & d.programData[i].battery.name; \
    s << "d_" << i <<  "_battery_" CHEALI_CHARGER_STRING(name) "="<< (uint16_t)d.programData[i].battery.name << "\n"

#define PROGDATA_BATTERY16(i, name) PROGDATA_BATTERY_(i,name, __reset16)
#define PROGDATA_BATTERY8(i, name) PROGDATA_BATTERY_(i,name, __reset8)



using namespace AnalogInputs;

//////toString

std::string EEPROM::toString() const
{
    eeprom::Data d(this->d);
    std::stringstream s;
    {
        s << std::setprecision(3) << std::fixed;
        s << "magicString='" << std::string((char *) d.magicString, EEPROM_MAGIC_STRING_LEN) <<"'\n";
        d.magicString[0] = d.magicString[1] = d.magicString[2] = d.magicString[3] = MAGIC;
        PRINT16(calibrationVersion, d.calibrationVersion);
        PRINT16(programDataVersion, d.programDataVersion);
        PRINT16(settingVersion,     d.settingVersion);
        s << "\n";

//calibration
        CALIB_V(Vout_plus_pin);
        CALIB_V(Vout_minus_pin);
        CALIB_I(Ismps);
        CALIB_I(Idischarge);

        CALIB_V(VoutMux);
        CALIB_T(Tintern);
        CALIB_V(Vin);
        CALIB_T(Textern);

        CALIB_V(Vb0_pin);
        CALIB_V(Vb1_pin);
        CALIB_V(Vb2_pin);
        CALIB_V(Vb3_pin);
        CALIB_V(Vb4_pin);
        CALIB_V(Vb5_pin);
        CALIB_V(Vb6_pin);

#if MAX_BANANCE_CELLS > 6
        CALIB_V(Vb7_pin);
        CALIB_V(Vb8_pin);
#endif

        CALIB_I(IsmpsValue);
        CALIB_I(IdischargeValue);

        s << "\n";

//programData

        for(int i=0; i< MAX_PROGRAMS; i++) {
            PROGDATA_NAME(i);
            PROGDATA_BATTERY8(i, type);
            PROGDATA_BATTERY16(i, C);
            PROGDATA_BATTERY16(i, Ic);
            PROGDATA_BATTERY16(i, Id);
            PROGDATA_BATTERY16(i, cells);
        }

        s << "\n";

//settings
        SETT(backlight_);
        SETT(fanTempOn_);
        SETT(dischargeTempOff_);
        SETT(externT_);
        SETT(externTCO_);
        SETT(deltaT_);
        SETT(enable_deltaV_);
        SETT(deltaV_NiMH_);
        SETT(deltaV_NiCd_);
        SETT(CDcycles_);
        SETT(capCutoff_);
        SETT(inputVoltageLow_);
        SETT(dischargeOffset_LiXX_);
        SETT(dischargeAggressive_LiXX_);
        SETT(forceBalancePort_);
        SETT(balancerError_);
        SETT(UART_);
        SETT(UARTspeed_);
    }
    testIntegrity(&d, getSize());
    return s.str();
}
