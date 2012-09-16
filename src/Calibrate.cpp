#include "Options.h"
#include "LcdPrint.h"
#include "MainMenu.h"
#include "SMPS.h"
#include "Calibrate.h"
#include "Utils.h"
#include "Screen.h"

extern MainMenu::MenuData mainMenuData;

char string_c0[] PROGMEM = "B0-2";
char string_c1[] PROGMEM = "B3-5";
char string_c2[] PROGMEM = "B0-5 to Vout";
char string_c3[] PROGMEM = "Iout";
char string_c4[] PROGMEM = "Idischarge";
char string_c5[] PROGMEM = "Vin";
char string_c6[] PROGMEM = "Tintern";
char string_c7[] PROGMEM = "Textern";
char string_c8[] PROGMEM = "Info";


char string_ci0[] PROGMEM = "Iout";
char string_ci1[] PROGMEM = "Vout";
char string_ci2[] PROGMEM = "B0-2";
char string_ci3[] PROGMEM = "B3-5";
char string_ci4[] PROGMEM = "Temp";
char string_ci5[] PROGMEM = "Vin";
char string_ci6[] PROGMEM = "Idischarge";
char string_ci7[] PROGMEM = "time";


const char * calibrateMenu[] PROGMEM =
{ string_c0,
  string_c1,
  string_c2,
  string_c3,
  string_c4,
  string_c5,
  string_c6,
  string_c7,
  string_c8,
};

const char * calibrateInfoMenu[] PROGMEM =
{ string_ci0,
  string_ci1,
  string_ci2,
  string_ci3,
  string_ci4,
  string_ci5,
  string_ci6,
  string_ci7,
};


char string_cI0[] PROGMEM = "50mA";
char string_cI1[] PROGMEM = "1000mA";


const char * calibrateIMenu[] PROGMEM =
{
	string_cI0,
	string_cI1,
};


void Calibrate::calibrateI(screenType screen, AnalogInputs::Name name1, AnalogInputs::Name name2)
{
	MainMenu menu(NULL, calibrateIMenu, sizeOfArray(calibrateIMenu));
	AnalogInputs::CalibrationPoint p;
	int i = 0;
	do {
		i = menu.runSimple();
		if(i < 0) return;
		if(calibrate(screen)) {
			if(i == 0) {
				p.y = ANALOG_AMPS(0.05);
			} else {
				i = 1;
				p.y = ANALOG_AMPS(1);
			}
			p.x = analogInputs.getValue(name1);
			analogInputs.setCalibrationPoint(name1, i, p);
			p.x = analogInputs.getValue(name2);
			analogInputs.setCalibrationPoint(name2, i, p);
		}
	} while(i >= 0);
}


void Calibrate::run()
{
	MainMenu menu(NULL, calibrateMenu, sizeOfArray(calibrateMenu));
	int i;
	do {
		i = menu.runSimple();
		switch(i) {
		case 3: calibrateI(SCREEN_IOUT, AnalogInputs::Ismps, AnalogInputs::IsmpsValue); break;
		case 8: runInfo(); break;
		default:
				Screen::notImplemented(); break;
		}
	} while(i >= 0);
}

void Calibrate::runInfo()
{
	MainMenu menu(NULL, calibrateInfoMenu, sizeOfArray(calibrateInfoMenu));
	int i;
	do {
		i = menu.runSimple();
		if(i >= 0 && i<=5)
			info(i);

		switch(i) {
		case 6: infoDis(); break;
		case 7: infoTimeM(); break;
		}
	} while(i>=0);
}



void Calibrate::print_v(uint8_t dig){
	lcd.setCursor(0,0);
	switch(dispVal_) {
	case 0:	lcdPrint_P(PSTR("v:")); break;
	case 1: lcdPrint_P(PSTR("r:")); break;
	case 2: lcdPrint_P(PSTR("R:")); break;
	}
	lcdPrintEValueU(value_, dig);
}

void Calibrate::print_m(const char *str, AnalogInputs::Name name, int dig)
{
	lcdPrint_P(str);
	switch(dispVal_) {
	case 0:	lcdPrintEValueU(analogInputs.getValue(name), dig-2);
			break;
	case 1: analogInputs.printRealValue(name, dig);
			break;
	case 2: analogInputs.printMeasuredValue(name, dig);
			break;
	}
}

void Calibrate::print_m_2(const char *str, AnalogInputs::Name name, int dig)
{
	print_m(str, name, dig);
	lcdPrintSpaces();
}
void Calibrate::print_m_1(const char *str, AnalogInputs::Name name, int dig)
{
	lcd.setCursor(0,1);
	print_m(str, name, dig);
}
void Calibrate::print_m_3(const char *str, AnalogInputs::Name name, int dig)
{
	print_m_1(str, name , dig);
	lcdPrintSpaces();
}


void Calibrate::printCalibrateI()
{
	print_v();
	print_m_2(PSTR(" Is:"), AnalogInputs::Ismps);
	print_m_3(PSTR("Iv:"),   AnalogInputs::IsmpsValue);
}

void Calibrate::printCalibrateDis()
{
	print_v();
	print_m_2(PSTR(" Io:"), AnalogInputs::Idischarge);
	print_m_3(PSTR("Iv:"),   AnalogInputs::IdischargeValue);
}

void Calibrate::printCalibrateVout()
{
	print_v();
	print_m_2(PSTR(" Vo:"), AnalogInputs::Vout);
	print_m_3(PSTR("VoutMux:"), AnalogInputs::VoutMux);
}

void Calibrate::printCalibrateB0_2()
{
	print_v(5);
	uint8_t dig = 7;
	if(dispVal_ != 0) dig = 6;
	print_m_2(PSTR(" 0:"), AnalogInputs::Vb0, dig);
	print_m_1(PSTR("1:"),  AnalogInputs::Vb1, dig);
	if(dispVal_ == 0) lcd.print(' ');
	print_m_2(PSTR("2:"), AnalogInputs::Vb2, dig);
}

void Calibrate::printCalibrateB3_5()
{
	print_v(5);
	uint8_t dig = 7;
	if(dispVal_ != 0) dig = 6;
	print_m_2(PSTR(" 3:"), AnalogInputs::Vb3, dig);
	print_m_1(PSTR("4:"), AnalogInputs::Vb4, dig);
	if(dispVal_ == 0) lcd.print(' ');
	print_m_2(PSTR("5:"), AnalogInputs::Vb5, dig);
}

void Calibrate::printCalibrateT()
{
	print_v();
	print_m_2(PSTR(" Ti:"), AnalogInputs::Tintern);
	print_m_3(PSTR("Textern:"), AnalogInputs::Textern);
}
void Calibrate::printCalibrateVin()
{
	print_v();
	lcdPrintSpaces();
	print_m_3(PSTR("Vin:"), AnalogInputs::Vin);
}

void Calibrate::printCalibrate(int p) {
	switch(p) {
	case SCREEN_IOUT: printCalibrateI(); 		break;
	case 1: printCalibrateVout(); 	break;
	case 2: printCalibrateB0_2(); 	break;
	case 3: printCalibrateB3_5(); 	break;
	case 4: printCalibrateT(); 		break;
	case 5: printCalibrateVin(); 	break;
	}
}

void Calibrate::info(int p)
{
	value_ = 0;
	dispVal_ = 2;
	smps.powerOn();
	analogInputs.doFullMeasurement();
	uint8_t key;
	do {
		printCalibrate(p);
		key = keyboard.getPressedWithSpeed();
		if(key == BUTTON_INC && value_ < 760) {
			value_++;
			smps.setValue(value_);
		}
		if(key == BUTTON_DEC && value_ > 0) {
			value_--;
			smps.setValue(value_);
		}
		if(key == BUTTON_START) {
			dispVal_ = (dispVal_+1)%3;
		}

	} while(key != BUTTON_STOP);
	smps.powerOff();
}

bool Calibrate::calibrate(int p)
{
	bool retu = false;
	value_ = 0;
	dispVal_ = 2;
	smps.powerOn();
	analogInputs.doFullMeasurement();
	uint8_t key;
	do {
		printCalibrate(p);
		key = keyboard.getPressedWithSpeed();
		if(key == BUTTON_INC && value_ < 760) {
			value_++;
			smps.setValue(value_);
		}
		if(key == BUTTON_DEC && value_ > 0) {
			value_--;
			smps.setValue(value_);
		}
		if(key == BUTTON_START && keyboard.getSpeed() == 5) {
			retu = true;
			break;
		}

	} while(key != BUTTON_STOP);
	smps.powerOff();
	return retu;
}

void Calibrate::infoDis()
{
	value_ = 0;
	dispVal_ = 0;
	discharger.powerOn();
	analogInputs.doFullMeasurement();
	uint8_t key;
	do {
		printCalibrateDis();
		key = keyboard.getPressedWithSpeed();
		if(key == BUTTON_INC && value_ < 760*2) {
			value_++;
			discharger.setValue(value_);
		}
		if(key == BUTTON_DEC && value_ > 0) {
			value_--;
			discharger.setValue(value_);
		}
		if(key == BUTTON_START) {
			dispVal_ = (dispVal_+1)%3;
		}

	} while(key != BUTTON_STOP);
	discharger.powerOff();
}

void Calibrate::infoTimeM()
{
	lcd.clear();
	uint8_t key;
	do {
		key = keyboard.getPressedWithSpeed();
		uint32_t t1,t0;
		t0 = timer.getMiliseconds();
		hardware::delay(100);
		t1 = timer.getMiliseconds();
		lcd.setCursor(0,0);
		lcdPrint_P(PSTR(" time: "));
		lcdPrintEValueU(t0);
		lcd.setCursor(0,1);
		lcdPrint_P(PSTR("100ms: "));
		lcdPrintEValueU(t1-t0);

	} while(key != BUTTON_STOP);
}


