#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "StaticMenu.h"
#include "ProgramData.h"

class MainMenu : public StaticMenu {
public:
    MainMenu(const char * const* staticMenu,uint8_t staticMenuItems):
            StaticMenu(staticMenu, MAX_PROGRAMS + staticMenuItems){}

    virtual uint8_t printItem(uint8_t i) {
        uint8_t staticMenuSize = size_ - MAX_PROGRAMS;
        if(i < staticMenuSize) {
            return StaticMenu::printItem(i);
        } else {
            return lcdPrint_E(ProgramData::getName_E(i - staticMenuSize), PROGRAM_DATA_MAX_NAME);
        }
    }
};

#endif /* MENU_H_ */

