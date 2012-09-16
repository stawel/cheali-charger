#include "Options.h"
#include "LcdPrint.h"
#include "MainMenu.h"
#include "SMPS.h"
#include "Utils.h"
#include "Calibrate.h"
#include "Screen.h"

extern MainMenu::MenuData mainMenuData;


char string_o1[] PROGMEM = "settings";
char string_o2[] PROGMEM = "calibrate";
char string_o3[] PROGMEM = "reset default";

const char * optionsMainMenu[] PROGMEM =
{ string_o1,
  string_o2,
  string_o3,
};

MainMenu optionsMenu(NULL, optionsMainMenu, sizeOfArray(optionsMainMenu));


void Options::resetDefault()
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

void Options::run()
{
	uint8_t key;
	bool release = true;
	optionsMenu.render();
	Calibrate calibrate;

	do {
		key = optionsMenu.run();
		if(key == BUTTON_NONE) release = false;

		if(!release && key == BUTTON_START)  {
			int i = optionsMenu.getIndex();
			switch(i) {
			case 0: Screen::notImplemented(); break;
			case 1: calibrate.run(); break;
			case 2: resetDefault(); break;
			}
			optionsMenu.render();
			release = true;
		}
	} while(key != BUTTON_STOP || release);
}
