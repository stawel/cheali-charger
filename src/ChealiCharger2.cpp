#include "ChealiCharger2.h"
#include "MainMenu.h"
#include "ProgramData.h"
#include "AnalogInputs.h"
#include "EditName.h"
#include "Program.h"
#include "Options.h"
#include "Utils.h"
#include "Buzzer.h"
#include "Version.h"
#include "Settings.h"

const char string_options[] PROGMEM = "options";
const char * const progmemMainMenu[] PROGMEM =
{string_options };

MainMenu mainMenu(progmemMainMenu, 1);


int backlight_val = 1200;

void loop()
{
    int8_t index = mainMenu.runSimple();
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

    Settings::load();
    lcdPrint_P(PSTR("  ChealiCharger"));
    lcdSetCursor0_1();
    lcdPrint_P(PSTR("    ver: "  CHEALI_CHARGER_VERSION_STRING));
    timer.delay(1000);
    if(Version::getCurrentEEPROMVersion() != CHEALI_CHARGER_EEPROM_VERSION) {
        lcdClear();
        lcdSetCursor0_0();
        lcdPrint_P(PSTR("reseting eeprom"));
        lcdSetCursor0_1();
        lcdPrint_P(PSTR("to ver: " CHEALI_CHARGER_EPPROM_VERSION_STRING));
        timer.delay(5000);

        Options::resetDefault();

        lcdClear();
        lcdSetCursor0_0();
        lcdPrint_P(PSTR("please calibrate"));
        lcdSetCursor0_1();
        lcdPrint_P(PSTR("before use"));
        timer.delay(5000);
    }
}
