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

const char charge_str[] PROGMEM = "charge";
const char chaBal_str[] PROGMEM = "charge+balance";
const char balanc_str[] PROGMEM = "balance";
const char discha_str[] PROGMEM = "discharge";
const char fastCh_str[] PROGMEM = "fast charge";
const char storag_str[] PROGMEM = "storage";
const char stoBal_str[] PROGMEM = "storage+balanc";
const char edBatt_str[] PROGMEM = "edit battery";

struct programMemuType {
	const char * str;
	Program::ProgramType type;
};

const char * const progmemMainMenu2[] PROGMEM =
{ charge_str,
  chaBal_str,
  balanc_str,
  discha_str,
  fastCh_str,
  storag_str,
  stoBal_str,
  edBatt_str
};

const Program::ProgramType progmemMainMenu2Type[] PROGMEM =
{ Program::Charge,
  Program::Charge_Balance,
  Program::Balance,
  Program::Discharge,
  Program::FastCharge,
  Program::Storage,
  Program::Storage_Balance,
  Program::EditBattery
};



MainMenu doMenu(progmemMainMenu2, sizeOfArray(progmemMainMenu2));


void doProgram(int index)
{
	uint8_t key;
	bool release = true;
	doMenu.render();
	ProgramData::loadProgramData(index);

	do {
		key = doMenu.run();

		if(key == BUTTON_NONE)
			release = false;

		if(!release && key == BUTTON_START)  {
			int i = doMenu.getIndex();
			Program::ProgramType prog = pgm_read(&progmemMainMenu2Type[i]);
			switch(prog) {
			case Program::EditBattery:
				if(ProgramData::currentProgramData.edit(index)) {
					buzzer.soundSave();
					ProgramData::saveProgramData(index);
				}
				break;
			default:
				Program::run(prog);
				break;
			}
			doMenu.render();
			release = true;
		}
	} while(key != BUTTON_STOP || release);
}


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
			doProgram(index - 1);
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
