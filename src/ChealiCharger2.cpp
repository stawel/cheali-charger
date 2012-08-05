#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include "ChealiCharger2.h"
#include "MainMenu.h"
#include "ProgramData.h"
#include "AnalogInputs.h"
#include "EditName.h"
#include "StartProgram.h"
#include "Options.h"
#include "Utils.h"

MainMenu::MenuData mainMenuData EEMEM;

char string_2[] PROGMEM = "options";

const char * progmemMainMenu[] PROGMEM =
{string_2 };

MainMenu mainMenu(&mainMenuData, progmemMainMenu, 1);
ProgramData currentProgram;

char charge_str[] PROGMEM = "charge";
char balCha_str[] PROGMEM = "balance+charge";
char balanc_str[] PROGMEM = "balance";
char discha_str[] PROGMEM = "discharge";
char fastCh_str[] PROGMEM = "fast charge";
char storag_str[] PROGMEM = "storage";
char edName_str[] PROGMEM = "edit name";
char edBatt_str[] PROGMEM = "edit battery";

const char * progmemMainMenu2[] PROGMEM =
{ charge_str,
  balCha_str,
  balanc_str,
  discha_str,
  fastCh_str,
  storag_str,
  edName_str,
  edBatt_str
};


void napiecie();
MainMenu doMenu(NULL, progmemMainMenu2, sizeOfArray(progmemMainMenu2));

void editMenuName(int index) {
	EditName editName((char*)&(mainMenuData.items[index].name), MAINMENU_MAX_ITEM_NAME, PSTR("Edit name:"));
	editName.run();
}

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
			switch(i) {
			case 0:
				startProgram(ProgramData::Charge);
				doMenu.render();
				release = true;
				break;
			case 2:
				startProgram(ProgramData::Balance);
				doMenu.render();
				release = true;
				break;
			case 4:
				startProgram(ProgramData::FastCharge);
				doMenu.render();
				release = true;
				break;
			case 6:
				editMenuName(index);
				doMenu.render();
				release = true;
				break;
			case 7:
				if(ProgramData::currentProgramData.edit())
					ProgramData::saveProgramData(index);
				doMenu.render();
				release = true;
				break;
			}
		}
	} while(key != BUTTON_STOP || release);
}


int backlight_val = 1200;


void loop()
{
	uint8_t key = mainMenu.run();
	if(key == BUTTON_START)  {
		int index = mainMenu.getIndex();
		switch(index) {
		case 0:
			Options::run();
			mainMenu.render();
			break;
		default:
			doProgram(index - 1);
			mainMenu.render();
		}
	}
}


void setup()
{

#ifdef USE_SERIAL
	Serial.begin(9600);
	Serial.println("ChealiCharger hello!");
#endif //USE_SERIAL
	analogReference(EXTERNAL);

	hardware::init();
	hardware::setLCDBacklight(backlight_val);
	lcdPrint_P(PSTR("  ChealiCharger"));
	lcd.setCursor(0,1);
	lcdPrint_P(PSTR("    welcome ;)"));
	timer.delay(1000);
}

void napiecie()
{
	uint8_t key;
	lcd.clear();
	bool pin = true;
	do {
		lcd.setCursor(0,0);
		lcd.print('p');
		lcd.print(pin);
		lcd.print(' ');
		lcd.print(analogInputs.getRealValue(AnalogInputs::Vin));
		lcd.print(' ');
		lcd.print(analogInputs.getRealValue(AnalogInputs::Vout));
		lcd.print(' ');
		lcd.setCursor(0,1);
		lcd.print('s');
		lcd.print(smps.getValue());
		lcd.print(' ');
		lcd.print(analogInputs.getRealValue(AnalogInputs::Tintern));
		lcd.print(' ');

		key = keyboard.getPressedWithSpeed();
	} while(key != BUTTON_STOP);
}
