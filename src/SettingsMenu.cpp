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
//        string_SETM6,
        string_SETM7,
};


SettingsMenu::SettingsMenu(const Settings &p):
        EditMenu(SettingsStaticMenu, sizeOfArray(SettingsStaticMenu)), p_(p){};


uint8_t SettingsMenu::printItem(uint8_t index)
{
    StaticMenu::printItem(index);
    if(getBlinkIndex() != index) {
        switch (index) {
            case 0:    lcdPrintUnsigned(p_.backlight_, 3);     break;
            case 1:    printTemp(p_.fanTempOn_);                break;
            case 2:    printTemp(p_.fanTempOff_);                break;
            case 3:    printTemp(p_.dischargeTempOn_);            break;
            case 4:printTemp(p_.dischargeTempOff_);            break;
            case 5:    printVolt(p_.inputVoltageLow_);            break;
        }
    }
}

void SettingsMenu::editItem(uint8_t index, uint8_t key)
{
    int dir = -1;
    if(key == BUTTON_INC) dir = 1;
//    dir *= keyboard.getSpeedFactor();

    switch(index) {
        case 0:    changeBacklight(dir);                    break;
        case 1:    changeTemp(p_.fanTempOn_, dir);            break;
        case 2:    changeTemp(p_.fanTempOff_,dir);            break;
        case 3:    changeTemp(p_.dischargeTempOn_, dir);    break;
        case 4:    changeTemp(p_.dischargeTempOff_, dir);    break;
        case 5:    changeVolt(p_.inputVoltageLow_, dir);    break;
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


void SettingsMenu::printTemp(AnalogInputs::ValueType t) {
    lcdPrintUnsigned(t/100, 3);
    lcd.print('C');
}
void SettingsMenu::printVolt(AnalogInputs::ValueType v) {
    lcdPrintUnsigned(v/1000, 3);
    lcd.print('V');
}


void SettingsMenu::changeBacklight(int dir) {
    changeMax(p_.backlight_, dir, 99);
    p_.apply();
}

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
