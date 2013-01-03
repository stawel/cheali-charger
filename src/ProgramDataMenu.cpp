#include "ProgramDataMenu.h"
#include "LcdPrint.h"
#include "GTPowerA610.h"
#include "EditName.h"
#include "Utils.h"


const char string_PDMM0[] PROGMEM = "Bat:  ";
const char string_PDMM1[] PROGMEM = "V:  ";
const char string_PDMM2[] PROGMEM = "Ch: ";
const char string_PDMM3[] PROGMEM = "Ic: ";
const char string_PDMM4[] PROGMEM = "Id: ";
const char string_PDMM5[] PROGMEM = "  create name";
const char string_PDMM6[] PROGMEM = "   edit name";
const char string_PDMM7[] PROGMEM = "     save";



const char * const ProgramDataStaticMenu[] PROGMEM =
{
        string_PDMM0,
        string_PDMM1,
        string_PDMM2,
        string_PDMM3,
        string_PDMM4,
        string_PDMM5,
        string_PDMM6,
        string_PDMM7,
};


ProgramDataMenu::ProgramDataMenu(const ProgramData &p, int programIndex):
        EditMenu(ProgramDataStaticMenu, sizeOfArray(ProgramDataStaticMenu)), p_(p), programIndex_(programIndex){};


void ProgramDataMenu::editName()
{
    EditName editName(p_.name, PROGRAM_DATA_MAX_NAME, PSTR("Edit name:"));
    editName.run();
    render();
}

void ProgramDataMenu::createName()
{
    p_.createName(programIndex_+1);
    editName();
}


uint8_t ProgramDataMenu::printItem(uint8_t index)
{
    StaticMenu::printItem(index);
    if(getBlinkIndex() != index) {
        switch (index) {
            case 0:    p_.printBatteryString();    break;
            case 1:    p_.printVoltageString();    break;
            case 2:    p_.printChargeString();        break;
            case 3:    p_.printIcString();            break;
            case 4:    p_.printIdString();            break;
        }
    }
    return 0;
}

void ProgramDataMenu::editItem(uint8_t index, uint8_t key)
{
    int dir = -1;
    if(key == BUTTON_INC) dir = 1;
//    dir *= keyboard.getSpeedFactor();

    switch(index) {
    case 0: p_.changeBattery(dir);     break;
    case 1: p_.changeVoltage(dir);     break;
    case 2: p_.changeCharge(dir);     break;
    case 3: p_.changeIc(dir);         break;
    case 4: p_.changeId(dir);         break;
    }
}

bool ProgramDataMenu::run() {
    int8_t index;
    do {
        index = runSimple();

        if(index < 0) return false;
        switch(index) {
        case 5: createName(); break;
        case 6: editName(); break;
        case PROGRAM_DATA_MENU_SIZE - 1: return true; //save
        default:
            ProgramData undo(p_);
            if(!runEdit(index))
                p_ = undo;
            p_.check();
            break;
        }
    } while(true);
}

