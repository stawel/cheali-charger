#ifndef STATICMENU_H_
#define STATICMENU_H_

#include "LcdPrint.h"
#include "memory.h"
#include "Menu.h"

class StaticMenu : public Menu {
public:
    const char * const* staticMenu_;

public:
    StaticMenu(const char * const* staticMenu,uint8_t size):
            Menu(size), staticMenu_(staticMenu){}


    virtual uint8_t printItem(uint8_t i) {
        return lcdPrint_P(pgm::read(&staticMenu_[i]));
    }
};

#endif /* STATICMENU_H_ */

