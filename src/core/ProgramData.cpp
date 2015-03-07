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

using namespace programData;

ProgramData::Battery ProgramData::battery;

//battery voltage limits, see also: ProgramData::getVoltagePerCell, ProgramData::getVoltage
const AnalogInputs::ValueType voltsPerCell[][ProgramData::LAST_VOLTAGE_TYPE] PROGMEM  =
{
//          { VIdle,              VCharge,            VDischarge,         VStorage,           ValidEmpty};
/*None*/    { 1,                  1,                  1,                  1,                  1},
/*Unknown*/ { 1,                  1,                  1,                  1,                  1},
/*NiCd*/    { ANALOG_VOLT(1.200), 0/*settings*/,      ANALOG_VOLT(0.850), 0,                  ANALOG_VOLT(0.850)},
//http://en.wikipedia.org/wiki/Nickel%E2%80%93metal_hydride_battery
//http://eu.industrial.panasonic.com/sites/default/pidseu/files/downloads/files/ni-mh-handbook-2014_interactive.pdf
//http://www6.zetatalk.com/docs/Batteries/Chemistry/Duracell_Ni-MH_Rechargeable_Batteries_2007.pdf
/*NiMH*/    { ANALOG_VOLT(1.200), 0/*settings*/,      ANALOG_VOLT(1.000), 0,                  ANALOG_VOLT(1.000)},

//Pb based on:
//http://www.battery-usa.com/Catalog/NPAppManual%28Rev0500%29.pdf
//charge start current 0.25C (stage 1 - constant current)
//charge end current 0.05C (end current = start current / 5) (stage 2 - constant voltage)
//Stage 3 (float charge) - not implemented
//http://batteryuniversity.com/learn/article/charging_the_lead_acid_battery
/*Pb*/      { ANALOG_VOLT(2.000), ANALOG_VOLT(2.450), ANALOG_VOLT(1.750), ANALOG_VOLT(0.000), ANALOG_VOLT(1.900)},

//LiXX - see also settings: overCharge_LiXX, overDischarge_LiXX
/*Life*/    { ANALOG_VOLT(3.300), ANALOG_VOLT(3.600), ANALOG_VOLT(2.000), ANALOG_VOLT(3.300), ANALOG_VOLT(3.000)},
/*Lilo*/    { ANALOG_VOLT(3.600), ANALOG_VOLT(4.100), ANALOG_VOLT(2.500), ANALOG_VOLT(3.750), ANALOG_VOLT(3.500)},
/*LiPo*/    { ANALOG_VOLT(3.700), ANALOG_VOLT(4.200), ANALOG_VOLT(3.000), ANALOG_VOLT(3.850), ANALOG_VOLT(3.209)},
/*Li430*/   { ANALOG_VOLT(3.700), ANALOG_VOLT(4.300), ANALOG_VOLT(3.000), ANALOG_VOLT(3.850), ANALOG_VOLT(3.209)},
/*Li435*/   { ANALOG_VOLT(3.700), ANALOG_VOLT(4.350), ANALOG_VOLT(3.000), ANALOG_VOLT(3.850), ANALOG_VOLT(3.209)},

//based on "mars" settings, TODO: find datasheet
/*NiZn*/    { ANALOG_VOLT(1.600), ANALOG_VOLT(1.900), ANALOG_VOLT(1.300), ANALOG_VOLT(1.600), ANALOG_VOLT(1.400)},

};

uint16_t ProgramData::getVoltagePerCell(VoltageType type)
{
    STATIC_ASSERT(sizeOfArray(voltsPerCell) == ProgramData::LAST_BATTERY_TYPE);
    uint16_t result = pgm::read(&voltsPerCell[battery.type][type]);
    if (type == VCharge) {
        if (battery.type == NiMH) {
            result = settings.cutoffV_NiMH;
        } else if (battery.type == NiCd) {
            result = settings.cutoffV_NiCd;
        } else if (isLiXX()) {
            result += settings.overCharge_LiXX;
        }
    } else if (type == VDischarge) {
        if (isLiXX()) {
            result += settings.overDischarge_LiXX;
        }
    }
    return result;
}
uint16_t ProgramData::getVoltage(VoltageType type)
{
    uint16_t cells = battery.cells;
    uint16_t voltage = getVoltagePerCell(type);

    if(type == VDischarge && battery.type == NiMH && cells > 6) {
        //based on http://eu.industrial.panasonic.com/sites/default/pidseu/files/downloads/files/ni-mh-handbook-2014_interactive.pdf
        //page 11: "Discharge end voltage"
        cells--;
        voltage = ANALOG_VOLT(1.200);
    }
    return cells * voltage;
}


//                              def. capacity          chargei             dischargei       cell   tlimit
/*const ProgramData::Battery defaultProgram[] PROGMEM = {
        {ProgramData::None,     ANALOG_CHARGE(0), ANALOG_AMP(0), ANALOG_AMP(0), 0, 0},
        {ProgramData::Unknown,  ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900), 10000, 600},
        {ProgramData::NiCd,     ANALOG_CHARGE(2.200), ANALOG_AMP(0.500), ANALOG_AMP(1.900),     1, 480},
        {ProgramData::NiMH,     ANALOG_CHARGE(2.200), ANALOG_AMP(0.500), ANALOG_AMP(1.900),     1, 480},
        {ProgramData::Pb,       ANALOG_CHARGE(2.200), ANALOG_AMP(0.220), ANALOG_AMP(1.900),     6, 480},
        {ProgramData::Life,     ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900),     3, 120},
        {ProgramData::Lilo,     ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900),     3, 120},
        {ProgramData::Lipo,     ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900),     3, 120},
        {ProgramData::Li430,    ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900),     3, 120},
        {ProgramData::Li435,    ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900),     3, 120},
        {ProgramData::NiZn,     ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900),     3, 120}
};*/

const char * const  ProgramData::batteryString[] PROGMEM = {
        string_battery_None,
        string_battery_Unknown,
        string_battery_NiCd,
        string_battery_NiMH,
        string_battery_Pb,
        string_battery_Life,
        string_battery_Lilo,
        string_battery_Lipo,
        string_battery_Li430,
        string_battery_Li435,
        string_battery_NiZn
};

ProgramData::BatteryClass ProgramData::getBatteryClass() {
    if( battery.type == NiZn) return ClassNiZn;
    if( battery.type == Life  || battery.type == Lilo  || battery.type == Lipo
     || battery.type == Li430 || battery.type == Li435) return ClassLiXX;
    if( battery.type == NiCd  || battery.type == NiMH) return ClassNiXX;
    return ClassPb;
}

void ProgramData::loadProgramData(int index)
{
    eeprom::read(battery, &eeprom::data.battery[index]);
}

void ProgramData::saveProgramData(int index)
{
    eeprom::write(&eeprom::data.battery[index], battery);
    eeprom::restoreProgramDataCRC();
}

uint16_t ProgramData::getCapacityLimit()
{
    uint32_t cap = battery.C;
    cap *= settings.capCutoff;
    cap/=100;
    if(cap>ANALOG_MAX_CHARGE)
        cap = ANALOG_MAX_CHARGE;
    return cap;
}

int16_t ProgramData::getDeltaVLimit()
{
    int16_t v = 0;
    if(battery.type == NiCd) v = settings.deltaV_NiCd;
    if(battery.type == NiMH) v = settings.deltaV_NiMH;
    return battery.cells * v;
}

void ProgramData::restoreDefault()
{
  //  pgm::read(battery, &defaultProgram[Lipo]);
    check();
    for(int i=0;i< MAX_PROGRAMS;i++) {
        saveProgramData(i);
    }
    eeprom::restoreProgramDataCRC();
}

void ProgramData::loadDefault()
{
//    STATIC_ASSERT(sizeOfArray(defaultProgram) == ProgramData::LAST_BATTERY_TYPE);
//    pgm::read(battery, &defaultProgram[battery.type]);
}


void ProgramData::printBatteryString()
{
    STATIC_ASSERT(sizeOfArray(batteryString) == ProgramData::LAST_BATTERY_TYPE);
    lcdPrint_P(batteryString, battery.type);
}

uint16_t ProgramData::getMaxIc()
{
    uint32_t i;
    uint16_t v;
    v = getVoltage(VDischarge);
    i = MAX_CHARGE_P;
    i *= ANALOG_VOLT(1);
#ifdef ENABLE_DYNAMIC_MAX_POWER
    if(v > ANALOG_VOLT(8)) {
        v -= ANALOG_VOLT(8);
    } else {
        v = 1;
    }
#endif

    i /= v;

    if(i > MAX_CHARGE_I)
        i = MAX_CHARGE_I;
    return i;
}

uint16_t ProgramData::getMaxId()
{
    uint32_t i;
    uint16_t v;
    v = getVoltage(VDischarge);
    i = MAX_DISCHARGE_P;
    i *= ANALOG_VOLT(1);
    i /= v;

    if(i > MAX_DISCHARGE_I)
        i = MAX_DISCHARGE_I;
    return i;
}

uint16_t ProgramData::getMaxCells()
{
    uint16_t v = getVoltagePerCell(VCharge);
    return MAX_CHARGE_V / v;
}

void ProgramData::check()
{
    uint16_t v;

    v = ANALOG_MIN_CHARGE;
    if(battery.C < v) battery.C = v;
    v = ANALOG_MAX_CHARGE;
    if(battery.C > v) battery.C = v;

#ifdef ENABLE_TIME_LIMIT
    v = ANALOG_MAX_TIME_LIMIT;
    if(battery.time > v) battery.time = v;
#endif

    v = getMaxCells();
    if(battery.cells > v) battery.cells = v;

    v = getMaxIc();
    if(battery.Ic > v) battery.Ic = v;

    v = getMaxId();
    if(battery.Id > v) battery.Id = v;

    v = settings.minIout;
    if(battery.Ic < v) battery.Ic = v;
    if(battery.Id < v) battery.Id = v;
}
