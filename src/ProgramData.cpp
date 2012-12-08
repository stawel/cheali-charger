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

const ProgramData::BatteryData defaultProgram[ProgramData::LAST_BATTERY_TYPE] PROGMEM = {
{ProgramData::Unknown, 	2200, 2200, 2200, 10000},
{ProgramData::NiCd, 	2200, 2200, 2200, 1},
{ProgramData::NiMH, 	2200, 2200, 2200, 1},
{ProgramData::Pb, 		2200, 2200, 2200, 6},
{ProgramData::Life, 	2200, 2200, 2200, 3},
{ProgramData::Lilo, 	2200, 2200, 2200, 3},
{ProgramData::Lipo, 	2200, 2200, 2200, 3}
};

const char b0[] PROGMEM = "Unknown";
const char b1[] PROGMEM = "NiCd";
const char b2[] PROGMEM = "NiMH";
const char b3[] PROGMEM = "Pb";
const char b4[] PROGMEM = "Life";
const char b5[] PROGMEM = "Lilo";
const char b6[] PROGMEM = "Lipo";
const char * const  batteryString[ProgramData::LAST_BATTERY_TYPE] PROGMEM = {b0,b1,b2,b3,b4,b5,b6};

void ProgramData::createName(int index)
{
	char buf[PROGRAM_DATA_MAX_NAME+1];
	char * type = (char*)pgm_read_word(&batteryString[battery.type]);
	snprintf_P(buf, PROGRAM_DATA_MAX_NAME+1, PSTR("%d:%S %d/%dC"), index, type, battery.Ic, battery.cells);
	memcpy(name, buf, PROGRAM_DATA_MAX_NAME);
}

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
	return pgm_read<uint16_t>(&voltsPerCell[battery.type][type]);
}
uint16_t ProgramData::getVoltage(VoltageType type) const
{
	return battery.cells * getVoltagePerCell(type);
}

void ProgramData::restoreDefault()
{
	pgm_read(currentProgramData.battery, &defaultProgram[Lipo]);
	for(int i=0;i< MAX_PROGRAMS;i++) {
		sprintf_P(currentProgramData.name, PSTR("%d:"),i+1);
		saveProgramData(i);
	}
}

void ProgramData::loadDefault()
{
	pgm_read(battery, &defaultProgram[battery.type]);
}


uint8_t ProgramData::printBatteryString(int n) const { return lcdPrint_P((char*)pgm_read_word(&batteryString[battery.type]), n); }

uint8_t ProgramData::printVoltageString() const
{
	if(battery.type == Unknown) {
		lcdPrintVoltage(getVoltage(), 7);
		return 7;
	} else {
		uint8_t r = 5+2;
		lcdPrintVoltage(getVoltage(), 5);
		lcd.print('/');
		r+=lcd.print(battery.cells);
		lcd.print('C');
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
	uint16_t r;
	bool inc = direction > 0;
	int step = 1;

	if((v > 100)   || (inc && v == 100))   step = 10;
	if((v > 1000)  || (inc && v == 1000))  step = 100;
	if((v > 10000) || (inc && v == 10000)) step = 1000;

	r = v%step;

	if(r) {
		if(direction > 0) nv -= r;
		if(direction < 0) nv += step - r;
	}
	nv += direction*step;
	if(nv < 1) nv = 1;
	else if(nv > max) nv = max;
	v = nv;
}

void ProgramData::changeBattery(int direction)
{
	change(battery.type, direction, LAST_BATTERY_TYPE);
	loadDefault();
}

void ProgramData::changeVoltage(int direction)
{
	uint16_t max = getMaxCells();
	changeMax(battery.cells, direction, max);
}

void ProgramData::changeCharge(int direction)
{
	changeMax(battery.C, direction, 65000);
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
	i *= ANALOG_VOLTS(1);
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
	i *= ANALOG_VOLTS(1);
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
