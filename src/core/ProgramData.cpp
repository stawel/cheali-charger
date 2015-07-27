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

ProgramData::Battery ProgramData::battery;

//battery voltage limits, see also: ProgramData::getVoltagePerCell, ProgramData::getVoltage
const AnalogInputs::ValueType voltsPerCell[][ProgramData::LAST_VOLTAGE_TYPE] PROGMEM  =
{
//          { VIdle,              VCharge,            VDischarge,         VStorage,           ValidEmpty};
/*None*/    { 1,                  1,                  1,                  1,                  1},
/*NiCd*/    { ANALOG_VOLT(1.200), ANALOG_VOLT(1.800), ANALOG_VOLT(0.850), 0,                  ANALOG_VOLT(0.850)},
//http://en.wikipedia.org/wiki/Nickel%E2%80%93metal_hydride_battery
//http://eu.industrial.panasonic.com/sites/default/pidseu/files/downloads/files/ni-mh-handbook-2014_interactive.pdf
//http://www6.zetatalk.com/docs/Batteries/Chemistry/Duracell_Ni-MH_Rechargeable_Batteries_2007.pdf
/*NiMH*/    { ANALOG_VOLT(1.200), ANALOG_VOLT(1.800), ANALOG_VOLT(1.000), 0,                  ANALOG_VOLT(1.000)},

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

/*Unknown*/ { 1,                  ANALOG_VOLT(4.000), ANALOG_VOLT(2.000), 1,                  1},
//PowerSupply
/*LED*/     { 1,                  ANALOG_VOLT(4.000), 1,                  1,                  1},

};

uint16_t ProgramData::getDefaultVoltagePerCell(VoltageType type)
{
    STATIC_ASSERT(sizeOfArray(voltsPerCell) == ProgramData::LAST_BATTERY_TYPE);
    uint16_t result = pgm::read(&voltsPerCell[battery.type][type]);
    return result;
}
uint16_t ProgramData::getDefaultVoltage(VoltageType type)
{
    uint16_t cells = battery.cells;
    uint16_t voltage = getDefaultVoltagePerCell(type);

    if(type == VDischarge && battery.type == NiMH && cells > 6) {
        //based on http://eu.industrial.panasonic.com/sites/default/pidseu/files/downloads/files/ni-mh-handbook-2014_interactive.pdf
        //page 11: "Discharge end voltage"
        cells--;
        voltage = ANALOG_VOLT(1.200);
    }
    return cells * voltage;
}

uint16_t ProgramData::getVoltage(VoltageType type) {
    uint16_t cells = battery.cells;
    uint16_t voltage = getDefaultVoltagePerCell(type);
    if (type == VCharge) {
        voltage = battery.Vc_per_cell;
    } else if (type == VDischarge) {
        voltage = battery.Vd_per_cell;
    } else if (type == VStorage) {
        voltage = battery.Vs_per_cell;
    }
    //TODO:type == VDischarge && battery.type == NiMH && cells > 6
    //see getDefaultVoltage
    return cells * voltage;
}


const char * const  ProgramData::batteryString[] PROGMEM = {
        string_battery_None,
        string_battery_NiCd,
        string_battery_NiMH,
        string_battery_Pb,
        string_battery_Life,
        string_battery_Lilo,
        string_battery_Lipo,
        string_battery_Li430,
        string_battery_Li435,
        string_battery_NiZn,
        string_battery_Unknown,
        string_battery_LED,
};

STATIC_ASSERT(sizeOfArray(ProgramData::batteryString) == ProgramData::LAST_BATTERY_TYPE);

const ProgramData::BatteryClass ProgramData::batteryClassMap[] PROGMEM = {
/*None*/    ClassUnknown,
/*NiCd*/    ClassNiXX,
/*NiMH*/    ClassNiXX,
/*Pb*/      ClassPb,
/*Life*/    ClassLiXX,
/*Lilo*/    ClassLiXX,
/*Lipo*/    ClassLiXX,
/*Li430*/   ClassLiXX,
/*Li435*/   ClassLiXX,
/*NiZn*/    ClassNiZn,
/*Unknown*/ ClassUnknown,
/*LED*/     ClassLED
};


ProgramData::BatteryClass ProgramData::getBatteryClass() {
    STATIC_ASSERT(sizeOfArray(ProgramData::batteryClassMap) == ProgramData::LAST_BATTERY_TYPE);
    return pgm::read(&ProgramData::batteryClassMap[battery.type]);
}

uint16_t ProgramData::getCapacityLimit()
{
    uint32_t cap = battery.capacity;
    cap *= battery.capCutoff;
    cap/=100;
    if(cap>ANALOG_MAX_CHARGE || isPowerSupply())
        cap = ANALOG_MAX_CHARGE;
    return cap;
}

int16_t ProgramData::getDeltaVLimit()
{
    int16_t v = battery.deltaV;
    return battery.cells * v;
}

uint16_t ProgramData::getMaxIc()
{
    AnalogInputs::ValueType v = getDefaultVoltage(VDischarge);
#ifdef ENABLE_DYNAMIC_MAX_POWER
    if(v > ANALOG_VOLT(8)) {
        v -= ANALOG_VOLT(8);
    } else {
        v = 1;
    }
#endif

    AnalogInputs::ValueType i = AnalogInputs::evalI(MAX_CHARGE_P, v);

    if(i > MAX_CHARGE_I)
        i = MAX_CHARGE_I;
    return i;
}

uint16_t ProgramData::getMaxId()
{
    AnalogInputs::ValueType v = getDefaultVoltage(VDischarge);
    AnalogInputs::ValueType i = AnalogInputs::evalI(MAX_DISCHARGE_P, v);

    if(i > MAX_DISCHARGE_I)
        i = MAX_DISCHARGE_I;
    return i;
}

uint16_t ProgramData::getMaxCells()
{
    if(battery.type == Unknown || battery.type == LED)
        return 1;
    uint16_t v = getDefaultVoltagePerCell(VCharge);
    return MAX_CHARGE_V / v;
}

void ProgramData::check()
{
    uint16_t v;

    v = ANALOG_MIN_CHARGE;
    if(battery.capacity < v) battery.capacity = v;
    v = ANALOG_MAX_CHARGE;
    if(battery.capacity > v) battery.capacity = v;

#ifdef ENABLE_TIME_LIMIT
    v = ANALOG_MAX_TIME_LIMIT;
    if(battery.time > v) battery.time = v;
#endif

    v = getMaxCells();
    if(battery.cells > v) battery.cells = v;
    v = 1;
    if(battery.cells < v) battery.cells = v;


    v = getMaxIc();
    if(battery.Ic > v) battery.Ic = v;
    v = battery.Ic;
    if(battery.minIc > v) battery.minIc = v;

    v = getMaxId();
    if(battery.Id > v) battery.Id = v;
    v = battery.Id;
    if(battery.minId > v) battery.minId = v;

    v = settings.minIc;
    if(battery.Ic < v) battery.Ic = v;
    if(battery.minIc < v) battery.minIc = v;

    v = settings.minId;
    if(battery.Id < v) battery.Id = v;
    if(battery.minId < v) battery.minId = v;
}

void ProgramData::loadProgramData(uint8_t index)
{
    eeprom::read(battery, &eeprom::data.battery[index]);
    check();
}

void ProgramData::saveProgramData(uint8_t index)
{
    eeprom::write(&eeprom::data.battery[index], battery);
    eeprom::restoreProgramDataCRC();
}

void ProgramData::restoreDefault()
{
    battery.type = None;
    changedType();

    for(int i=0;i< MAX_PROGRAMS;i++) {
        saveProgramData(i);
    }
    eeprom::restoreProgramDataCRC();
}

void ProgramData::changedType()
{
    battery.Vc_per_cell = getDefaultVoltagePerCell(VCharge);
    battery.Vd_per_cell = getDefaultVoltagePerCell(VDischarge);

    if(battery.type == None) {
        battery.type = None;
        battery.capacity = ANALOG_CHARGE(2.000);
        battery.cells = 3;

        battery.time = ANALOG_MAX_TIME_LIMIT;
        battery.enable_externT = false;
        battery.externTCO = ANALOG_CELCIUS(60);

        battery.enable_adaptiveDischarge = false;
        battery.DCRestTime = 30;
        battery.capCutoff = 120;
    }

    if(isNiXX()) {
        battery.enable_deltaV = true;
        if(battery.type == NiMH) {
            battery.deltaV = -ANALOG_VOLT(0.005);
        } else {
            battery.deltaV = -ANALOG_VOLT(0.015);
        }
        battery.deltaVIgnoreTime = 3;
        battery.deltaT = ANALOG_CELCIUS(1);
        battery.DCcycles = 5;
    } else {
        battery.balancerError = ANALOG_VOLT(0.008);
        battery.Vs_per_cell = getDefaultVoltagePerCell(VStorage);
    }
    changedCapacity();

}

void ProgramData::changedIc()
{
    ProgramData::check();
    ProgramData::battery.minIc = ProgramData::battery.Ic/10;
}

void ProgramData::changedId()
{
    ProgramData::check();
    ProgramData::battery.minId = ProgramData::battery.Id/10;
}

void ProgramData::changedCapacity()
{
    ProgramData::check();
    ProgramData::battery.Ic = ProgramData::battery.capacity;
    if(ProgramData::battery.type == Pb)
        ProgramData::battery.Ic/=4;

    changedIc();
    ProgramData::battery.Id = ProgramData::battery.capacity;
    changedId();
}


//TODO: move print... to ??
#include "ScreenStartInfo.h"
void ProgramData::printProgramData(uint8_t index)
{
    loadProgramData(index);
    if(battery.type != None) {
        Screen::StartInfo::printBatteryString();
        lcdPrintSpace1();
    }

    switch(battery.type) {
    case None:
        lcdPrintUInt(index+1);
        lcdPrintChar(':');
        break;
    case Unknown:
        Screen::StartInfo::printVoltageString(9);
        break;
    case LED:
        lcdPrintCurrent(battery.Ic,5);
        lcdPrintSpace1();
        Screen::StartInfo::printVoltageString(4);
        break;
    default:
        lcdPrintCharge(battery.capacity, 7);
        lcdPrintChar('/');
        lcdPrintUInt(battery.cells);
        break;
    }
}

