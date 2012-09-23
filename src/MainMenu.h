#ifndef MAINMENU_H_
#define MAINMENU_H_

#include <avr/eeprom.h>
#include "Hardware.h"
#include "LcdPrint.h"
#include "memory.h"
#include "Menu.h"
#include "ProgramData.h"

#define MAINMENU_MAX_ITEMS MAX_PROGRAMS
#define MAINMENU_MAX_ITEM_NAME  14


class MainMenu : public Menu {
public:
	struct Item {
		char name[MAINMENU_MAX_ITEM_NAME];
	};
	struct MenuData {
		 uint8_t pos;
		 Item items[MAINMENU_MAX_ITEMS];

		 void restoreDefault() {
			 char buf[MAINMENU_MAX_ITEM_NAME];
			 for(int j=0;j<MAINMENU_MAX_ITEM_NAME;j++) {
				 buf[j] = ' ';
			 }
			 for(int i=0;i<MAINMENU_MAX_ITEMS;i++) {
				 buf[0] = ((i+1)/10) + '0';
				 buf[1] = ((i+1)%10) + '0';
				 buf[2] = ':';
				 eeprom_write(&items[i].name, buf);
			 }
		 }
	};

	MenuData * menuE_;
	const char * const* progmemMenu_;
	uint8_t progmemMenuItems_;

public:
	MainMenu(MenuData *menuE, const char * const* progmemMenu,uint8_t progmemMenuItems):
			Menu((menuE ? MAINMENU_MAX_ITEMS : 0) + progmemMenuItems),
			menuE_(menuE), progmemMenu_(progmemMenu), progmemMenuItems_(progmemMenuItems){}


	virtual uint8_t printItem(int i) {
		if(i < progmemMenuItems_) {
			return lcdPrint_P((char*)pgm_read_word(&progmemMenu_[i]), MAINMENU_MAX_ITEM_NAME);
		} else {
			return lcdPrint_E(menuE_->items[i - progmemMenuItems_].name, MAINMENU_MAX_ITEM_NAME);
		}
	}
};

#endif /* MENU_H_ */

