#include <avr/pgmspace.h>

#include "ProgramData.h"
#include "memory.h"
#include "LcdPrint.h"
#include "ProgramDataMenu.h"

ProgramData allProgramData[MAX_PROGRAMS] EEMEM;
ProgramData ProgramData::currentProgramData;


const uint16_t voltsPerCell[ProgramData::LAST_BATTERY_TYPE][ProgramData::LAST_VOLTAGE_TYPE] PROGMEM  =
{
	//  {Idle, Charge, Discharge, Storage};
		{   	1,     1,      1,	1}, // Unknown
		{	1200, 	 1200,	 850,	0}, //	NiCd //??
		{	1200,	 1200,	1000,	0}, //	NiMH //??
		{	2000,	 2460,	1500,	0}, //Pb
//LiXX
		{	3300,	 3600,	2000,	3300 /*??*/}, //Life
		{	3600,	 4100,	2500,	3750 /*??*/}, //Lilo
		{	3700,	 4200,	3000,	3850 /*??*/}, //LiPo

};

const ProgramData defaultProgram[ProgramData::LAST_BATTERY_TYPE] PROGMEM = {
{ProgramData::Unknown, 	2200, 2200, 10000},
{ProgramData::NiCd, 	2200, 2200, 1},
{ProgramData::NiMH, 	2200, 2200, 1},
{ProgramData::Pb, 		2200, 2200, 6},
{ProgramData::Life, 	2200, 2200, 3},
{ProgramData::Lilo, 	2200, 2200, 3},
{ProgramData::Lipo, 	2200, 2200, 3}
};

const char b0[] PROGMEM = "Unknown";
const char b1[] PROGMEM = "NiCd";
const char b2[] PROGMEM = "NiMH";
const char b3[] PROGMEM = "Pb";
const char b4[] PROGMEM = "Life";
const char b5[] PROGMEM = "Lilo";
const char b6[] PROGMEM = "Lipo";
const char * const  batteryString[ProgramData::LAST_BATTERY_TYPE] PROGMEM = {b0,b1,b2,b3,b4,b5,b6};

void ProgramData::loadProgramData(int index)
{
	eeprom_read<ProgramData>(currentProgramData, &allProgramData[index]);
}

void ProgramData::saveProgramData(int index)
{
	eeprom_write<ProgramData>(&allProgramData[index], currentProgramData);
}

uint16_t ProgramData::getVoltagePerCell(VoltageType type) const
{
	return pgm_read<uint16_t>(&voltsPerCell[batteryType][type]);
}
void ProgramData::restoreDefault()
{
	pgm_read(currentProgramData, &defaultProgram[Lipo]);
	for(int i=0; i < MAX_PROGRAMS; i++) {
		saveProgramData(i);
	}
}
void ProgramData::loadDefault()
{
	pgm_read(*this, &defaultProgram[batteryType]);
}


void ProgramData::printBatteryString(int n) const { lcdPrint_P((char*)pgm_read_word(&batteryString[batteryType]), n); }

void ProgramData::printVoltageString() const
{
	if(batteryType == Unknown) {
		lcdPrintVoltage(getVoltage(), 7);
	} else {
		lcdPrintVoltage(getVoltage(), 5);
		lcd.print('/');
		lcd.print(cells);
		lcd.print('C');
	}
}

void ProgramData::printCurrentString() const
{
	lcdPrintCurrent(I, 6);
}
void ProgramData::printChargeString() const
{
	lcdPrintCharge(C, 8);
}



bool ProgramData::edit()
{
	ProgramDataMenu menu(*this);
	if(menu.edit()) {
		*this = menu.p_;
		return true;
	}
	return false;
}

template<class val_t>
void change(val_t &v, int direction, uint16_t max)
{
	v+=direction;
	if(v>max)
		v = max-1;
	v%=max;
}

void changeMax(uint16_t &v, int direction, uint16_t max)
{
	int32_t nv = v;
	nv += direction;
	if(nv < 0) nv = 0;
	else if(nv > max) nv = max;
	v = nv;
}

void ProgramData::changeBattery(int direction)
{
	change(batteryType, direction, LAST_BATTERY_TYPE);
	loadDefault();
}

void ProgramData::changeVoltage(int direction)
{
	uint16_t max = getMaxCells();
	changeMax(cells, direction, max);
}

void ProgramData::changeCharge(int direction)
{
	changeMax(C, direction, 65535);
}

void ProgramData::changeCurrent(int direction)
{
	changeMax(I, direction, getMaxCurrent());
}

uint16_t ProgramData::getMaxCells() const
{
	uint16_t v = getVoltagePerCell(VCharge);
	return MAX_CHARGE_V / v;
}

uint32_t ProgramData::getMaxPower() const
{
	return MAX_CHARGE_P;
}

uint16_t ProgramData::getMaxCurrent() const
{
	return MAX_CHARGE_I;
}

void ProgramData::check()
{
	uint16_t v = getMaxCells();
	uint16_t i = getMaxCurrent();
	uint32_t p = getMaxPower();

	if(cells > v) cells = v;
	if(I > i) I = i;

	uint32_t i2 = p/v;
	if(I > i2) I = i2;
}


