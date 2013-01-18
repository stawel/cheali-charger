#include "SettingsMenu.h"
#include "Utils.h"
#include "LcdPrint.h"
#include "Buzzer.h"

const char string_backlight[]   PROGMEM = "backlight:";
const char string_fanOn[]       PROGMEM = "fan on:   ";
const char string_dischOff[]    PROGMEM = "disch off:";
const char string_externT[]     PROGMEM = "extrn T:   ";
const char string_externTCO[]   PROGMEM = "extrn TCO:";
const char string_dTdt[]        PROGMEM = "dT/dt:";
const char string_NiMHdV[]      PROGMEM = "NiMH -dV:";
const char string_NiCddV[]      PROGMEM = "NiCd -dV:";
const char string_CDcycles[]    PROGMEM = "C/D cycles:";
const char string_capCoff[]     PROGMEM = "cap COff:";


const char string_inputLow[]    PROGMEM = "input low:";
const char string_view[]        PROGMEM = "view: ";
const char string_reset[]       PROGMEM = "   reset";
const char string_save[]        PROGMEM = "    save"    ;

const char * const SettingsStaticMenu[] PROGMEM =
{
        string_backlight,
        string_fanOn,
        string_dischOff,
        string_externT,
        string_externTCO,
        string_dTdt,
        string_NiMHdV,
        string_NiCddV,
        string_CDcycles,
        string_capCoff,
        string_inputLow,
        string_view,
        string_reset,
        string_save
};


SettingsMenu::SettingsMenu(const Settings &p):
        EditMenu(SettingsStaticMenu, sizeOfArray(SettingsStaticMenu)), p_(p){};


uint8_t SettingsMenu::printItem(uint8_t index)
{
    StaticMenu::printItem(index);
    if(getBlinkIndex() != index) {
        switch (index) {
            case 0:     lcdPrintUnsigned(p_.backlight_, 3);     break;
            case 1:     printTemp(p_.fanTempOn_);               break;
            case 2:     printTemp(p_.dischargeTempOff_);        break;
            case 3:     lcdPrintYesNo(p_.externT_);             break;
            case 4:     printTemp(p_.externTCO_);               break;
            case 5:     printDeltaT(p_.deltaT_);                break;
            case 6:     lcdPrintDeltaV(p_.deltaV_NiMH_, 5);     break;
            case 7:     lcdPrintDeltaV(p_.deltaV_NiCd_, 5);     break;
            case 8:     lcdPrintUnsigned(p_.CDcycles_, 3);      break;
            case 9:     lcdPrintPercentage(p_.capCutoff_, 5);   break;
            case 10:    printVolt(p_.inputVoltageLow_);         break;
            case 11:    printViewType();                        break;
        }
    }
}

void SettingsMenu::editItem(uint8_t index, uint8_t key)
{
    int dir = -1;
    if(key == BUTTON_INC) dir = 1;
//    dir *= keyboard.getSpeedFactor();

    switch(index) {
        case 0:     changeBacklight(dir);                       break;
        case 1:     changeTemp(p_.fanTempOn_, dir);             break;
        case 2:     changeTemp(p_.dischargeTempOff_, dir);      break;
        case 3:     changeMax(p_.externT_, dir, 1);             break;
        case 4:     changeTemp(p_.externTCO_,dir);              break;
        case 5:     changeDeltaTemp(p_.deltaT_,dir);            break;
        case 6:     changeMax(p_.deltaV_NiMH_, dir, 20);        break;
        case 7:     changeMax(p_.deltaV_NiCd_, dir, 20);        break;
        case 8:     change1Max(p_.CDcycles_, dir, 5);           break;
        case 9:     change1Max(p_.capCutoff_, dir, 250);        break;
        case 10:    changeVolt(p_.inputVoltageLow_, dir);       break;
        case 11:    changeViewType(dir);                        break;
    }
}

bool SettingsMenu::run() {
    int8_t index;
    do {
        index = runSimple();

        if(index < 0) return false;
        switch(index) {
        case sizeOfArray(SettingsStaticMenu) - 2:  //reset
            p_.setDefault();
            buzzer.soundSelect();
            break;
        case sizeOfArray(SettingsStaticMenu) - 1:  //save
            return true;
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
    lcdPrintChar('C');
}
void SettingsMenu::printVolt(AnalogInputs::ValueType v) {
    lcdPrintUnsigned(v/1000, 3);
    lcdPrintChar('V');
}


void SettingsMenu::printDeltaT(AnalogInputs::ValueType dt)
{
    lcdPrintUnsigned(dt/100, 3);
    lcdPrintChar('.');
    lcdPrintDigit((dt%100)/10);
    lcdPrint_P(PSTR("C/m"));
}


void SettingsMenu::changeBacklight(int dir) {
    changeMaxSmart(p_.backlight_, dir, 99);
    p_.apply();
}

void SettingsMenu::changeViewType(int dir)
{
    uint16_t v = p_.view_;
    if(dir < 0) p_.view_ = Screen::Normal;
    if(dir > 0) p_.view_ = Screen::Debug;
}

void SettingsMenu::printViewType() const
{
    if(p_.view_ == Screen::Normal)
        lcdPrint_P(PSTR("normal"));
    else
        lcdPrint_P(PSTR("debug"));
}

void SettingsMenu::changeTemp(AnalogInputs::ValueType &v, int step) {
    const AnalogInputs::ValueType min = ANALOG_CELCIUS(1);
    const AnalogInputs::ValueType max = ANALOG_CELCIUS(99);
    step *= ANALOG_CELCIUS(1);
    v+=step;
    if(v < min) v = min;
    if(v > max) v = max;
}
void SettingsMenu::changeDeltaTemp(AnalogInputs::ValueType &v, int step) {
    const AnalogInputs::ValueType min = ANALOG_CELCIUS(0.1);
    const AnalogInputs::ValueType max = ANALOG_CELCIUS(10);
    step *= ANALOG_CELCIUS(0.1);
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
