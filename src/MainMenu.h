#ifndef MAINMENU_H_
#define MAINMENU_H_

#include <avr/eeprom.h>
#include "Hardware.h"
#include "LcdPrint.h"
#include "memory.h"
#include "Menu.h"
#include "ProgramData.h"


class MainMenu : public Menu {
public:
	const char * const* progmemMenu_;
	uint8_t progmemMenuItems_;

public:
	MainMenu(const char * const* progmemMenu,uint8_t progmemMenuItems, bool printPrograms = false):
			Menu((printPrograms ? MAX_PROGRAMS : 0) + progmemMenuItems),
			progmemMenu_(progmemMenu), progmemMenuItems_(progmemMenuItems){}


	virtual uint8_t printItem(int i) {
		if(i < progmemMenuItems_) {
			return lcdPrint_P((char*)pgm_read_word(&progmemMenu_[i]), PROGRAM_DATA_MAX_NAME);
		} else {
			return lcdPrint_E(ProgramData::getName_E(i - progmemMenuItems_), PROGRAM_DATA_MAX_NAME);
		}
	}
};

#endif /* MENU_H_ */

