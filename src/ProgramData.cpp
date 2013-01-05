#include "ProgramData.h"
#include "memory.h"
#include "LcdPrint.h"
#include "ProgramDataMenu.h"
#include "Utils.h"
#include "Settings.h"

ProgramData allProgramData[MAX_PROGRAMS] EEMEM;
ProgramData ProgramData::currentProgramData;


const uint16_t voltsPerCell[ProgramData::LAST_BATTERY_TYPE][ProgramData::LAST_VOLTAGE_TYPE] PROGMEM  =
{
//      { VIdle,              VCharge,            VDischarge,           VStorage};
        { 1,                  1,                  1,                    1}, // Unknown
//                               ???
        { ANALOG_VOLT(1.200), ANALOG_VOLT(1.820), ANALOG_VOLT(0.850),   0}, //    NiCd //??
//                              wikipedia
        { ANALOG_VOLT(1.200), ANALOG_VOLT(1.600), ANALOG_VOLT(1.000),   0}, //    NiMH //??
        { ANALOG_VOLT(2.000), ANALOG_VOLT(2.460), ANALOG_VOLT(1.500),   0}, //Pb
//LiXX
        { ANALOG_VOLT(3.300), ANALOG_VOLT(3.600), ANALOG_VOLT(2.000),   ANALOG_VOLT(3.300) /*??*/}, //Life
        { ANALOG_VOLT(3.600), ANALOG_VOLT(4.100), ANALOG_VOLT(2.500),   ANALOG_VOLT(3.750) /*??*/}, //Lilo
        { ANALOG_VOLT(3.700), ANALOG_VOLT(4.200), ANALOG_VOLT(3.000),   ANALOG_VOLT(3.850) /*??*/}, //LiPo

};

const ProgramData::BatteryData defaultProgram[ProgramData::LAST_BATTERY_TYPE] PROGMEM = {
        {ProgramData::Unknown,  ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900), 10000},
        {ProgramData::NiCd,     ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900), 1},
        {ProgramData::NiMH,     ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900), 1},
        {ProgramData::Pb,       ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900), 6},
        {ProgramData::Life,     ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900), 3},
        {ProgramData::Lilo,     ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900), 3},
        {ProgramData::Lipo,     ANALOG_CHARGE(2.200), ANALOG_AMP(2.200), ANALOG_AMP(1.900), 3}
};

const char batteryString_Unknown[]  PROGMEM = "Unknown";
const char batteryString_NiCd[]     PROGMEM = "NiCd";
const char batteryString_NiMH[]     PROGMEM = "NiMH";
const char batteryString_Pb[]       PROGMEM = "Pb";
const char batteryString_Life[]     PROGMEM = "Life";
const char batteryString_Lilo[]     PROGMEM = "Lilo";
const char batteryString_Lipo[]     PROGMEM = "Lipo";
const char * const  batteryString[ProgramData::LAST_BATTERY_TYPE] PROGMEM = {
        batteryString_Unknown,
        batteryString_NiCd,
        batteryString_NiMH,
        batteryString_Pb,
        batteryString_Life,
        batteryString_Lilo,
        batteryString_Lipo
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
    printUInt(buf, maxSize, battery.Ic);
    printChar(buf, maxSize, '/');
    printUInt(buf, maxSize, battery.cells);
}

void ProgramData::loadProgramData(int index)
{
    eeprom::read<ProgramData>(currentProgramData, &allProgramData[index]);
}

void ProgramData::saveProgramData(int index)
{
    eeprom::write<ProgramData>(&allProgramData[index], currentProgramData);
}

uint16_t ProgramData::getVoltagePerCell(VoltageType type) const
{
    return pgm::read(&voltsPerCell[battery.type][type]);
}
uint16_t ProgramData::getVoltage(VoltageType type) const
{
    return battery.cells * getVoltagePerCell(type);
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
        lcdPrintDigit(battery.cells);
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

uint8_t ProgramData::printChargeString() const
{
    lcdPrintCharge(battery.C, 7);
    return 8;
}


char * ProgramData::getName_E(int index)
{
    return allProgramData[index].name;
}


bool ProgramData::edit(int index)
{
    ProgramDataMenu menu(*this, index);
    if(menu.run()) {
        *this = menu.p_;
        return true;
    }
    return false;
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
    changeMax(battery.cells, direction, max);
}

void ProgramData::changeCharge(int direction)
{
    changeMax(battery.C, direction, ANALOG_CHARGE(65.000));
    battery.Ic = battery.C;
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
    changeMax(battery.Ic, direction, getMaxIc());
}
void ProgramData::changeId(int direction)
{
    changeMax(battery.Id, direction, getMaxId());
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
