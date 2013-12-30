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
#include "ProgramData.h"
#include "memory.h"
#include "LcdPrint.h"
#include "ProgramDataMenu.h"
#include "Utils.h"
#include "Settings.h"
#include "eeprom.h"

ProgramData ProgramData::currentProgramData;

//TODO_NJ
//expanded Validempty. Needed clear EEPROM!!!! (I increase EEPROM version to 6)
const AnalogInputs::ValueType voltsPerCell[ProgramData::LAST_BATTERY_TYPE][ProgramData::LAST_VOLTAGE_TYPE] PROGMEM  =
{
//      { VIdle,              VCharge,            VDischarge,           VStorage, VUpperLimit,         ValidEmpty};
/*Unknown*/{ 1,               1,                  1,                    1,        0,                   1},
//                               ???
/*NiCd*/{ ANALOG_VOLT(1.200), ANALOG_VOLT(1.820), ANALOG_VOLT(0.850),   0,  ANALOG_VOLT(1.800),        ANALOG_VOLT(0.850)},
//http://en.wikipedia.org/wiki/Nickel%E2%80%93metal_hydride_battery
//http://industrial.panasonic.com/eu/i/21291/Handbook2011/Handbook2011.pdf
//http://www6.zetatalk.com/docs/Batteries/Chemistry/Duracell_Ni-MH_Rechargeable_Batteries_2007.pdf
/*NiMH*/{ ANALOG_VOLT(1.200), ANALOG_VOLT(1.600), ANALOG_VOLT(1.000),   0,  ANALOG_VOLT(1.800),        ANALOG_VOLT(1.000)},

//Pb based on:
//http://www.battery-usa.com/Catalog/NPAppManual%28Rev0500%29.pdf
//charge start current 0.25C (stage 1 - constant current)
//charge end current 0.05C (end current = start current / 5) (stage 2 - constant voltage)
//Stage 3 (float charge) - not implemented
//http://batteryuniversity.com/learn/article/charging_the_lead_acid_battery
/*Pb*/  { ANALOG_VOLT(2.000), ANALOG_VOLT(2.450), ANALOG_VOLT(1.750),   0,  0, /*??*/                 ANALOG_VOLT(1.900)},
//LiXX
/*Life*/{ ANALOG_VOLT(3.300), ANALOG_VOLT(3.600), ANALOG_VOLT(2.000),   ANALOG_VOLT(3.300) /*??*/, 0, ANALOG_VOLT(3.000)},
/*Lilo*/{ ANALOG_VOLT(3.600), ANALOG_VOLT(4.100), ANALOG_VOLT(2.500),   ANALOG_VOLT(3.750) /*??*/, 0, ANALOG_VOLT(3.500)},
/*LiPo*/{ ANALOG_VOLT(3.700), ANALOG_VOLT(4.200), ANALOG_VOLT(3.000),   ANALOG_VOLT(3.850) /*??*/, 0, ANALOG_VOLT(3.209)},
/*Li43*/{ ANALOG_VOLT(3.700), ANALOG_VOLT(4.350), ANALOG_VOLT(3.000),   ANALOG_VOLT(3.850) /*??*/, 0, ANALOG_VOLT(3.309)},

//based on "mars" settings - not tested
/*NiZn*/{ ANALOG_VOLT(1.600), ANALOG_VOLT(1.900), ANALOG_VOLT(1.300),   ANALOG_VOLT(1.600) /*Probably not??*/, 0, ANALOG_VOLT(1.500)},
};

//                              def. capacity          chargei             dischargei       cell   tlimit                               
const ProgramData::BatteryData defaultProgram[ProgramData::LAST_BATTERY_TYPE] PROGMEM = {
        {ProgramData::Unknown,  ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900), 10000, 600,},
        {ProgramData::NiCd,     ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900),     1, 480,},
        {ProgramData::NiMH,     ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900),     1, 480},
        {ProgramData::Pb,       ANALOG_CHARGE(2.200), ANALOG_AMP(0.220), ANALOG_AMP(1.900),     6, 480},
        {ProgramData::Life,     ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900),     3, 120},
        {ProgramData::Lilo,     ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900),     3, 120},
        {ProgramData::Lipo,     ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900),     3, 120},
        {ProgramData::Li43,     ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900),     3, 120},
        {ProgramData::NiZn,     ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900),     3, 120}
};

const char batteryString_Unknown[]  PROGMEM = "Unknown";
const char batteryString_NiCd[]     PROGMEM = "NiCd";
const char batteryString_NiMH[]     PROGMEM = "NiMH";
const char batteryString_Pb[]       PROGMEM = "Pb";
const char batteryString_Life[]     PROGMEM = "Life";
const char batteryString_Lilo[]     PROGMEM = "Lilo";
const char batteryString_Lipo[]     PROGMEM = "Lipo";
const char batteryString_Li43[]     PROGMEM = "Li43";
const char batteryString_NiZn[]     PROGMEM = "NiZn";

const char * const  batteryString[ProgramData::LAST_BATTERY_TYPE] PROGMEM = {
        batteryString_Unknown,
        batteryString_NiCd,
        batteryString_NiMH,
        batteryString_Pb,
        batteryString_Life,
        batteryString_Lilo,
        batteryString_Lipo,
        batteryString_Li43,
        batteryString_NiZn
};

void ProgramData::printIndex(char *&buf, uint8_t &maxSize, uint8_t index)
{
    printUInt(buf, maxSize, index);
    printChar(buf, maxSize, ':');
}

void ProgramData::createName(int index)
{
    char *buf = name;
    uint8_t maxSize = PROGRAM_DATA_MAX_NAME;
    const char * type = pgm::read(&batteryString[battery.type]);
    printIndex(buf,maxSize, index);
    print_P  (buf, maxSize, type);
    printChar(buf, maxSize, ' ');
    printUInt(buf, maxSize, battery.C);
    printChar(buf, maxSize, '/');
    printUInt(buf, maxSize, battery.cells);
}

void ProgramData::loadProgramData(int index)
{
    eeprom::read<ProgramData>(currentProgramData, &eeprom::data.programData[index]);
}

void ProgramData::saveProgramData(int index)
{
    eeprom::write<ProgramData>(&eeprom::data.programData[index], currentProgramData);
}

uint16_t ProgramData::getVoltagePerCell(VoltageType type) const
{
    return pgm::read(&voltsPerCell[battery.type][type]);
}
uint16_t ProgramData::getVoltage(VoltageType type) const
{
    return battery.cells * getVoltagePerCell(type);
}

uint16_t ProgramData::getCapacityLimit() const
{
    uint32_t cap = battery.C;
    cap *= settings.capCutoff_;
    cap/=100;
    if(cap>PROGRAM_DATA_MAX_CHARGE)
        cap = PROGRAM_DATA_MAX_CHARGE;
    return cap;
}

uint16_t ProgramData::getTimeLimit() const
{
    uint16_t tim = battery.Time;
    return tim;
}

int16_t ProgramData::getDeltaVLimit() const
{
    int16_t v = 0;
    if(battery.type == NiCd) v = settings.deltaV_NiCd_;
    if(battery.type == NiMH) v = settings.deltaV_NiMH_;
    return battery.cells * v;
}
int16_t ProgramData::getDeltaTLimit() const
{
    return settings.deltaT_;
}


void ProgramData::restoreDefault()
{
    pgm::read(currentProgramData.battery, &defaultProgram[Lipo]);
    currentProgramData.check();
    for(int i=0;i< MAX_PROGRAMS;i++) {
        uint8_t maxSize = PROGRAM_DATA_MAX_NAME;
        char *buf = currentProgramData.name;
        printIndex(buf, maxSize, i+1);
        saveProgramData(i);
    }
}

void ProgramData::loadDefault()
{
    pgm::read(battery, &defaultProgram[battery.type]);
}


uint8_t ProgramData::printBatteryString(int n) const { return lcdPrint_P(pgm::read(&batteryString[battery.type]), n); }
uint8_t ProgramData::printBatteryString() const { return printBatteryString(LCD_COLUMNS); }

uint8_t ProgramData::printVoltageString() const
{
    if(battery.type == Unknown) {
        lcdPrintVoltage(getVoltage(), 7);
        return 7;
    } else {
        uint8_t r = 5+2+1;
        lcdPrintVoltage(getVoltage(), 5);
        lcdPrintChar('/');
        lcdPrintUInt(battery.cells);
        lcdPrintChar('C');
        return r;
    }
}

uint8_t ProgramData::printIcString() const
{
    lcdPrintCurrent(battery.Ic, 6);
    return 6;
}
uint8_t ProgramData::printIdString() const
{
    lcdPrintCurrent(battery.Id, 6);
    return 6;
}

uint8_t ProgramData::printTimeString() const
{
    if(battery.Time == 1000) {
        lcdPrint_P(PSTR("unlimited"));
    } else {
        lcdPrintUInt(battery.Time);
        lcdPrint_P(PSTR("min."));
    }
    return 6;
}

uint8_t ProgramData::printChargeString() const
{
    if(battery.C == PROGRAM_DATA_MAX_CHARGE)
        lcdPrint_P(PSTR("unlimited"));
    else
        lcdPrintCharge(battery.C, 7);
    return 8;
}


char * ProgramData::getName_E(int index)
{
    return eeprom::data.programData[index].name;
}


void ProgramData::edit(int index)
{
    ProgramDataMenu menu(*this, index);
    menu.run();
    *this = menu.p_;
}

template<class val_t>
void change(val_t &v, int direction, uint16_t max)
{
}

void ProgramData::changeBattery(int direction)
{
    battery.type+=direction;
    if(battery.type>=LAST_BATTERY_TYPE)
        battery.type=Unknown;
    loadDefault();
}

void ProgramData::changeVoltage(int direction)
{
    uint16_t max = getMaxCells();
    bool noPow10=false;
    if (battery.type == Unknown) noPow10=true;
    changeMaxSmart(battery.cells, direction, max, noPow10, 1);
}

void ProgramData::changeCharge(int direction)
{
    changeMaxSmart(battery.C, direction, PROGRAM_DATA_MAX_CHARGE,false,100);
    battery.Ic = battery.C;
    if(isPb())
        battery.Ic/=4; //0.25C
    battery.Id = battery.C;
    check();
}

uint16_t ProgramData::getMaxIc() const
{
    uint32_t i;
    uint16_t v;
    v = getVoltage(VCharge);
    i = MAX_CHARGE_P;
    i *= ANALOG_VOLT(1);
    i /= v;                 

    if(i > MAX_CHARGE_I)  
        i = MAX_CHARGE_I; 
    return i;
}

uint16_t ProgramData::getMaxId() const
{
    uint32_t i;
    uint16_t v;
    v = getVoltage(VCharge);
    i = MAX_DISCHARGE_P;
    i *= ANALOG_VOLT(1);
    i /= v;

    if(i > MAX_DISCHARGE_I) 
        i = MAX_DISCHARGE_I;
    return i;
}

void ProgramData::changeIc(int direction)
{
    changeMaxSmart(battery.Ic, direction, getMaxIc(),false,100);
}
void ProgramData::changeId(int direction)
{
    changeMaxSmart(battery.Id, direction, getMaxId(),false,100);
}

void ProgramData::changeTime(int direction)
{
    changeMaxStep10(battery.Time, direction, 1000);
}

uint16_t ProgramData::getMaxCells() const
{
    uint16_t v = getVoltagePerCell(VCharge);
    return MAX_CHARGE_V / v;
}

void ProgramData::check()
{
    uint16_t v;

    v = getMaxCells();
    if(battery.cells > v) battery.cells = v;

    v = getMaxIc();
    if(battery.Ic > v) battery.Ic = v;

    v = getMaxId();
    if(battery.Id > v) battery.Id = v;
}

