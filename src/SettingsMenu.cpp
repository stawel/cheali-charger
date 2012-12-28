#include "SettingsMenu.h"
#include "Utils.h"
#include "LcdPrint.h"

const char string_SETM0[] PROGMEM = "backlight:";
const char string_SETM1[] PROGMEM = "fan on:   ";
const char string_SETM2[] PROGMEM = "fan off:  ";
const char string_SETM3[] PROGMEM = "disch on: ";
const char string_SETM4[] PROGMEM = "disch Off:";
const char string_SETM5[] PROGMEM = "input low:";
const char string_SETM6[] PROGMEM = "view: ";
const char string_SETM7[] PROGMEM = "     save";


const char * const SettingsStaticMenu[] PROGMEM =
{
		string_SETM0,
		string_SETM1,
		string_SETM2,
		string_SETM3,
		string_SETM4,
		string_SETM5,
		//TODO: implement view type
//		string_SETM6,
		string_SETM7,
};


SettingsMenu::SettingsMenu(const Settings &p):
		EditMenu(SettingsStaticMenu, sizeOfArray(SettingsStaticMenu)), p_(p){};


uint8_t SettingsMenu::printItem(uint8_t index)
{
	StaticMenu::printItem(index);
	if(getBlinkIndex() != index) {
		switch (index) {
			case 0:	printBacklightString();			break;
			case 1:	printFanTempOnString();			break;
			case 2:	printFanTempOffString();		break;
			case 3:	printDischargeTempOnString();	break;
			case 4:	printDischargeTempOffString();	break;
			case 5:	printInputVoltageLowString();	break;
//			case 6:	printViewTypeString();			break;
		}
	}
	return 0;
}

void SettingsMenu::editItem(uint8_t index, uint8_t key)
{
	int dir = -1;
	if(key == BUTTON_INC) dir = 1;
//	dir *= keyboard.getSpeedFactor();

	switch(index) {
		case 0:	changeBacklight(dir);			break;
		case 1:	changeFanTempOn(dir);			break;
		case 2:	changeFanTempOff(dir);			break;
		case 3:	changeDischargeTempOn(dir);		break;
		case 4:	changeDischargeTempOff(dir);	break;
		case 5:	changeInputVoltageLow(dir);		break;
//		case 6:	changeViewType(dir);			break;
	}
}

bool SettingsMenu::run() {
	int8_t index;
	do {
		index = runSimple();

		if(index < 0) return false;
		switch(index) {
		case sizeOfArray(SettingsStaticMenu) - 1: return true; //save
		default:
			Settings undo(p_);
			if(!runEdit(index))
				p_ = undo;
			p_.check();
			p_.apply();
			break;
		}
	} while(true);
}


void SettingsMenu::printBacklightString() const			{ lcdPrintUnsigned(p_.backlight_, 3); }
void SettingsMenu::printFanTempOnString() const 			{ printTemp(p_.fanTempOn_); }
void SettingsMenu::printFanTempOffString() const			{ printTemp(p_.fanTempOff_); }
void SettingsMenu::printDischargeTempOnString() const 	{ printTemp(p_.dischargeTempOn_);}
void SettingsMenu::printDischargeTempOffString() const	{ printTemp(p_.dischargeTempOff_);}
void SettingsMenu::printInputVoltageLowString() const 	{ printVolt(p_.inputVoltageLow_); }
void SettingsMenu::printViewTypeString() const 			{ lcdPrintUnsigned(p_.view_, 4);}
void SettingsMenu::printTemp(AnalogInputs::ValueType t) {
	lcdPrintUnsigned(t/100, 3);
	lcd.print('C');
}
void SettingsMenu::printVolt(AnalogInputs::ValueType v) {
	lcdPrintUnsigned(v/1000, 3);
	lcd.print('V');
}


void SettingsMenu::changeBacklight(int dir) {
	changeMax(p_.backlight_, dir, 100);
	p_.apply();
}
void SettingsMenu::changeFanTempOn(int dir)			{	changeTemp(p_.fanTempOn_, dir);}
void SettingsMenu::changeFanTempOff(int dir)		{	changeTemp(p_.fanTempOff_,dir);}
void SettingsMenu::changeDischargeTempOn(int dir)	{	changeTemp(p_.dischargeTempOn_, dir);}
void SettingsMenu::changeDischargeTempOff(int dir)	{	changeTemp(p_.dischargeTempOff_, dir);}
void SettingsMenu::changeInputVoltageLow(int dir)	{	changeVolt(p_.inputVoltageLow_, dir);}
void SettingsMenu::changeViewType(int dir)
{
	uint16_t v = p_.view_;
	changeMax(v, dir, 3);
	p_.view_ = Screen::ScreenViewType(v);
}
void SettingsMenu::changeTemp(AnalogInputs::ValueType &v, int step) {
	const AnalogInputs::ValueType min = ANALOG_CELCIUS(1);
	const AnalogInputs::ValueType max = ANALOG_CELCIUS(99);
	step *= ANALOG_CELCIUS(1);
	v+=step;
	if(v < min) v = min;
	if(v > max) v = max;
}
void SettingsMenu::changeVolt(AnalogInputs::ValueType &v, int step) {
	const AnalogInputs::ValueType min = ANALOG_VOLT(7);
	const AnalogInputs::ValueType max = ANALOG_VOLT(30);
	step *= ANALOG_VOLT(1);
	v+=step;
	if(v < min) v = min;
	if(v > max) v = max;
}



