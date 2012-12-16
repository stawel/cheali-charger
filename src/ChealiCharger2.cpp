#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include "ChealiCharger2.h"
#include "MainMenu.h"
#include "ProgramData.h"
#include "AnalogInputs.h"
#include "EditName.h"
#include "Program.h"
#include "Options.h"
#include "Utils.h"
#include "Buzzer.h"

const char string_options[] PROGMEM = "options";
const char * const progmemMainMenu[] PROGMEM =
{string_options };

MainMenu mainMenu(progmemMainMenu, 1, true);


int backlight_val = 1200;

void loop()
{
	int index = mainMenu.runSimple();
	if(index >= 0)  {
		switch(index) {
		case 0:
			Options::run();
			break;
		default:
			Program::selectProgram(index - 1);
		}
	}
}


void setup()
{
	hardware::init();

#ifdef USE_SERIAL
	Serial.begin(9600);
	Serial.println("ChealiCharger hello!");
#endif //USE_SERIAL

	hardware::setLCDBacklight(backlight_val);
	lcdPrint_P(PSTR("  ChealiCharger"));
	lcd.setCursor(0,1);
	lcdPrint_P(PSTR("    welcome ;)"));
	timer.delay(1000);
}
