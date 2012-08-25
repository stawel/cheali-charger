#include "Options.h"
#include "LcdPrint.h"
#include "MainMenu.h"
#include "SMPS.h"

extern MainMenu::MenuData mainMenuData;


char string_o1[] PROGMEM = "time";
char string_o2[] PROGMEM = "calibrate I";
char string_o3[] PROGMEM = "calibrate V";
char string_o4[] PROGMEM = "calibrate B0-2";
char string_o5[] PROGMEM = "calibrate B3-5";
char string_o6[] PROGMEM = "calibrate T";
char string_o7[] PROGMEM = "calibrate Vin";
char string_o8[] PROGMEM = "reset default";

const char * optionsMainMenu[] PROGMEM =
{ string_o1,
  string_o2,
  string_o3,
  string_o4,
  string_o5,
  string_o6,
  string_o7,
  string_o8,
};


MainMenu optionsMenu(NULL, optionsMainMenu, sizeof(optionsMainMenu)/sizeof(optionsMainMenu[0]));


void resetDefault()
{
	lcd.clear();
	lcd.setCursor(0,0);
	lcdPrint_P(PSTR("Reseting: "));

	lcd.setCursor(7,1);
	lcdPrint_P(PSTR(" 0%"));
	analogInputs.restoreDefault();

	lcd.setCursor(7,1);
	lcdPrint_P(PSTR("30%"));
	mainMenuData.restoreDefault();

	lcd.setCursor(7,1);
	lcdPrint_P(PSTR("80%"));
	ProgramData::restoreDefault();

	lcd.setCursor(6,1);
	lcdPrint_P(PSTR("Done"));
	timer.delay(100);
}

class Calibrate {
public:
	uint16_t value;
	uint16_t dispVal;

	void print_v(uint8_t dig = 3){
		lcd.setCursor(0,0);
		switch(dispVal) {
		case 0:	lcdPrint_P(PSTR("v:")); break;
		case 1: lcdPrint_P(PSTR("r:")); break;
		case 2: lcdPrint_P(PSTR("R:")); break;
		}
		lcdPrintEValueU(value, dig);
	}
	void print_m(const char *str, AnalogInputs::Name name, int dig = 7)
	{
		lcdPrint_P(str);
		switch(dispVal) {
		case 0:	lcdPrintEValueU(analogInputs.getValue(name), dig-2);
		 	 	break;
		case 1: analogInputs.printRealValue(name, dig);
				break;
		case 2: analogInputs.printMeasuredValue(name, dig);
				break;
		}
	}

	void print_m_2(const char *str, AnalogInputs::Name name, int dig = 7)
	{
		print_m(str, name, dig);
		lcdPrintSpaces();
	}
	void print_m_1(const char *str, AnalogInputs::Name name, int dig = 7)
	{
		lcd.setCursor(0,1);
		print_m(str, name, dig);
	}
	void print_m_3(const char *str, AnalogInputs::Name name, int dig = 7)
	{
		print_m_1(str, name , dig);
		lcdPrintSpaces();
	}


	void printCalibrateI()
	{
		print_v();
		print_m_2(PSTR(" Is:"), AnalogInputs::Ismps);
		print_m_3(PSTR("Iv:"),   AnalogInputs::IsmpsValue);
	}

	void printCalibrateVout()
	{
		print_v();
		print_m_2(PSTR(" Vo:"), AnalogInputs::Vout);
		print_m_3(PSTR("VoutMux:"), AnalogInputs::VoutMux);
	}

	void printCalibrateB0_2()
	{
		print_v(5);
		uint8_t dig = 7;
		if(dispVal != 0) dig = 6;
		print_m_2(PSTR(" 0:"), AnalogInputs::Vb0, dig);
		print_m_1(PSTR("1:"),  AnalogInputs::Vb1, dig);
		if(dispVal == 0) lcd.print(' ');
		print_m_2(PSTR("2:"), AnalogInputs::Vb2, dig);
	}

	void printCalibrateB3_5()
	{
		print_v(5);
		uint8_t dig = 7;
		if(dispVal != 0) dig = 6;
		print_m_2(PSTR(" 3:"), AnalogInputs::Vb3, dig);
		print_m_1(PSTR("4:"), AnalogInputs::Vb4, dig);
		if(dispVal == 0) lcd.print(' ');
		print_m_2(PSTR("5:"), AnalogInputs::Vb5, dig);
	}

	void printCalibrateT()
	{
		print_v();
		print_m_2(PSTR(" Ti:"), AnalogInputs::Tintern);
		print_m_3(PSTR("Textern:"), AnalogInputs::Textern);
	}
	void printCalibrateVin()
	{
		print_v();
		lcdPrintSpaces();
		print_m_3(PSTR("Vin:"), AnalogInputs::Vin);
	}

	void printCalibrate(int p) {
		switch(p) {
		case 0: printCalibrateI(); 		break;
		case 1: printCalibrateVout(); 	break;
		case 2: printCalibrateB0_2(); 	break;
		case 3: printCalibrateB3_5(); 	break;
		case 4: printCalibrateT(); 		break;
		case 5: printCalibrateVin(); 	break;
		}
	}

	void calibrate(int p)
	{
		value = 0;
		dispVal = 0;
		smps.powerOn();
		analogInputs.doFullMeasurement();
		uint8_t key;
		do {
			printCalibrate(p);
			key = keyboard.getPressedWithSpeed();
			if(key == BUTTON_INC && value < 760) {
				value++;
				smps.setValue(value);
			}
			if(key == BUTTON_DEC && value > 0) {
				value--;
				smps.setValue(value);
			}
			if(key == BUTTON_START) {
				dispVal = (dispVal+1)%3;
			}

		} while(key != BUTTON_STOP);
		smps.powerOff();
	}
};

void timeM()
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

void Options::run()
{
	uint8_t key;
	bool release = true;
	optionsMenu.render();
	Calibrate c;

	do {
		key = optionsMenu.run();
		if(key == BUTTON_NONE) release = false;

		if(!release && key == BUTTON_START)  {
			int i = optionsMenu.getIndex();
			switch(i) {
			case 0: timeM(); break;
			case 1: c.calibrate(0); break;
			case 2: c.calibrate(1); break;
			case 3: c.calibrate(2); break;
			case 4: c.calibrate(3); break;
			case 5: c.calibrate(4); break;
			case 6: c.calibrate(5); break;
			case 7: resetDefault(); break;
			}
			optionsMenu.render();
			release = true;
		}
	} while(key != BUTTON_STOP || release);
}



