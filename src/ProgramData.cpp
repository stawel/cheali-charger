#include <avr/pgmspace.h>

#include "ProgramData.h"
#include "memory.h"
#include "LcdPrint.h"
#include "ProgramDataMenu.h"

ProgramData allProgramData[MAX_PROGRAMS] EEMEM;
ProgramData ProgramData::currentProgramData;


uint16_t voltsPerCell[ProgramData::LAST_BATTERY_TYPE][ProgramData::LAST_VOLTAGE_TYPE] PROGMEM  =
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

ProgramData defaultProgram[ProgramData::LAST_BATTERY_TYPE] PROGMEM = {
{ProgramData::Unknown, 	ProgramData::Charge, ProgramData::BDisable, ProgramData::Vout, ProgramData::Ismps, 2200, 2200, 10000},
{ProgramData::NiCd, 	ProgramData::Charge, ProgramData::BDisable, ProgramData::Vout, ProgramData::Ismps, 2200, 2200, 1},
{ProgramData::NiMH, 	ProgramData::Charge, ProgramData::BDisable, ProgramData::Vout, ProgramData::Ismps, 2200, 2200, 1},
{ProgramData::Pb, 		ProgramData::Charge, ProgramData::BDisable, ProgramData::Vout, ProgramData::Ismps, 2200, 2200, 6},
{ProgramData::Life, 	ProgramData::Charge, ProgramData::BAuto, ProgramData::VAuto, ProgramData::Ismps, 2200, 2200, 3},
{ProgramData::Lilo, 	ProgramData::Charge, ProgramData::BAuto, ProgramData::VAuto, ProgramData::Ismps, 2200, 2200, 3},
{ProgramData::Lipo, 	ProgramData::Charge, ProgramData::BAuto, ProgramData::VAuto, ProgramData::Ismps, 2200, 2200, 3},
};

char p0[] PROGMEM = "Idle";
char p1[] PROGMEM = "Charge";
char p2[] PROGMEM = "Discharge";
char p3[] PROGMEM = "Storage";
char p4[] PROGMEM = "Fast charge";

char * programString[ProgramData::LAST_PROGRAM_TYPE] PROGMEM = {p0,p1,p2,p3,p4};

char b0[] PROGMEM = "Unknown";
char b1[] PROGMEM = "NiCd";
char b2[] PROGMEM = "NiMH";
char b3[] PROGMEM = "Pb";
char b4[] PROGMEM = "Life";
char b5[] PROGMEM = "Lilo";
char b6[] PROGMEM = "Lipo";
char * batteryString[ProgramData::LAST_BATTERY_TYPE] PROGMEM = {b0,b1,b2,b3,b4,b5,b6};

char bl0[] PROGMEM = "Disable";
char bl1[] PROGMEM = "Auto";
char bl2[] PROGMEM = "Enable";
char * balanceString[ProgramData::LAST_BALANCE_TYPE] PROGMEM = {bl0,bl1,bl2};

char vi0[] PROGMEM = "Vout";
char vi1[] PROGMEM = "Auto";
char vi2[] PROGMEM = "Vbalacer";
char vi3[] PROGMEM = "Vout mux";
char * voltageInputString[ProgramData::LAST_VOLTAGE_INPUT] PROGMEM = {vi0,vi1,vi2,vi3};

char ii0[] PROGMEM = "Iout";
char ii1[] PROGMEM = "smps value";
char * CurrentInputString[ProgramData::LAST_CURRENT_INPUT] PROGMEM = {ii0,ii1};





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


void ProgramData::printProgramString() const { lcdPrint_P((char*)pgm_read_word(&programString[programType])); }
void ProgramData::printBatteryString(int n) const { lcdPrint_P((char*)pgm_read_word(&batteryString[batteryType]), n); }
void ProgramData::printBalanceString() const { lcdPrint_P((char*)pgm_read_word(&balanceString[balanceType])); }
void ProgramData::printVoltageInputString() const { lcdPrint_P((char*)pgm_read_word(&voltageInputString[voltageInput])); }
void ProgramData::printCurrentInputString() const { lcdPrint_P((char*)pgm_read_word(&CurrentInputString[currentInput])); }

void ProgramData::printVoltageString() const
{
	if(batteryType == Unknown)
		lcdPrintEValueEnd(getVoltage(), 6, true, PSTR("V"));
	else {
		lcdPrintEValueEnd(getVoltage(), 4, true, PSTR("V/"));
		lcd.print(cells);
		lcd.print('C');
	}
}

void ProgramData::printCurrentString() const
{
	lcdPrintEValueEnd(I, 5, false, PSTR("A"));
}
void ProgramData::printChargeString() const
{
	lcdPrintEValueEnd(C, 6, false, PSTR("Ah"));
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

void ProgramData::changeProgram(int direction)
{
	do {
		change(programType, direction, LAST_PROGRAM_TYPE);
	} while(!isLiXX() && programType == FastCharge);
}

void ProgramData::changeBattery(int direction)
{
	change(batteryType, direction, LAST_BATTERY_TYPE);
	loadDefault();
}

void ProgramData::changeBalance(int direction)
{
	if(isLiXX())
		change(balanceType, direction, LAST_BALANCE_TYPE);
}

void ProgramData::changeVoltageInput(int direction)
{
	do {
		change(voltageInput, direction, LAST_VOLTAGE_INPUT);
	} while(!isLiXX() && (voltageInput == Vbalacer || voltageInput == VAuto));
}

void ProgramData::changeCurrentInput(int direction)
{
	change(currentInput, direction, LAST_CURRENT_INPUT);
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
	if(programType == Discharge) return MAX_DISCHARGE_P;
	return MAX_CHARGE_P;
}

uint16_t ProgramData::getMaxCurrent() const
{
	if(programType == Discharge) return MAX_DISCHARGE_I;
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


AnalogInputs::Name ProgramData::getVm() const
{
	switch(voltageInput)
	{
	case Vout: 		return AnalogInputs::Vout;
	case VAuto: 	return AnalogInputs::VoutBalancer;
	case Vbalacer:	return AnalogInputs::Vbalacer;
	case VoutMux:	return AnalogInputs::VoutMux;
	}
	return AnalogInputs::Vout;
}

AnalogInputs::Name ProgramData::getIm() const
{
	switch(currentInput)
	{
	case Ismps: 		return AnalogInputs::Ismps;
	case IsmpsValue: 	return AnalogInputs::IsmpsValue;
	}
	return AnalogInputs::Ismps;
}

void ProgramData::initSmps() const
{
	smps.Vm_ = getVm();
	smps.Im_ = getIm();
}

