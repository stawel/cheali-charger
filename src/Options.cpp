#include "Options.h"
#include "LcdPrint.h"
#include "StaticMenu.h"
#include "SMPS.h"
#include "Utils.h"
#include "Calibrate.h"
#include "Screen.h"
#include "Version.h"
#include "Settings.h"

const char string_o1[] PROGMEM = "settings";
const char string_o2[] PROGMEM = "calibrate";
const char string_o3[] PROGMEM = "reset default";

const char * const optionsStaticMenu[] PROGMEM =
{ string_o1,
  string_o2,
  string_o3,
};

void Options::resetDefault()
{
    lcdClear();
    lcdSetCursor0_0();
    lcdPrint_P(PSTR("Reseting: "));

    lcdSetCursor(7,1);
    lcdPrint_P(PSTR(" 0%"));
    analogInputs.restoreDefault();

    lcdSetCursor(7,1);
    lcdPrint_P(PSTR("50%"));
    ProgramData::restoreDefault();
    Settings::restoreDefault();


    lcdSetCursor(6,1);
    lcdPrint_P(PSTR("Done"));
    Version::restoreDefault();
    timer.delay(100);
}

void Options::run()
{
    StaticMenu optionsMenu(optionsStaticMenu, sizeOfArray(optionsStaticMenu));
    Calibrate calibrate;
    int8_t i;

    do {
        i = optionsMenu.runSimple();
        switch(i) {
        case 0: settings.edit();; break;
        case 1: calibrate.run(); break;
        case 2: resetDefault(); break;
        }
    } while(i>=0);
}
