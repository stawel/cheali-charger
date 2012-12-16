#include "Options.h"
#include "LcdPrint.h"
#include "MainMenu.h"
#include "SMPS.h"
#include "Utils.h"
#include "Calibrate.h"
#include "Screen.h"
#include "Version.h"

const char string_o1[] PROGMEM = "settings";
const char string_o2[] PROGMEM = "calibrate";
const char string_o3[] PROGMEM = "reset default";

const char * const optionsMainMenu[] PROGMEM =
{ string_o1,
  string_o2,
  string_o3,
};

void Options::resetDefault()
{
	lcd.clear();
	lcd.setCursor(0,0);
	lcdPrint_P(PSTR("Reseting: "));

	lcd.setCursor(7,1);
	lcdPrint_P(PSTR(" 0%"));
	analogInputs.restoreDefault();

	lcd.setCursor(7,1);
	lcdPrint_P(PSTR("50%"));
	ProgramData::restoreDefault();

	lcd.setCursor(6,1);
	lcdPrint_P(PSTR("Done"));
	Version::restoreDefault();
	timer.delay(100);
}

void Options::run()
{
	MainMenu optionsMenu(optionsMainMenu, sizeOfArray(optionsMainMenu));
	Calibrate calibrate;
	int i;

	do {
		i = optionsMenu.runSimple();
		switch(i) {
		case 0: Screen::notImplemented(); break;
		case 1: calibrate.run(); break;
		case 2: resetDefault(); break;
		}
	} while(i>=0);
}
