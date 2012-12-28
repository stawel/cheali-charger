#include <inttypes.h>
#include "Hardware.h"
#include "LcdPrint.h"
#include "Menu.h"


Menu::Menu(uint8_t size):
		pos_(0), begin_(0), size_(size), render_(true), waitRelease_(true)
{}

uint8_t Menu::run() {
	uint8_t button = keyboard.getPressedWithSpeed();

	if(runReversedPolarityInfo()) {
		render_ = true;
		return REVERSED_POLARITY;
	}

	uint8_t index = getIndex();
	switch (button) {
	case BUTTON_INC:
		incIndex();
		break;
	case BUTTON_DEC:
		decIndex();
		break;
	}
	if(index != getIndex())
		render_ = true;

	if(render_)
		display();

	return button;
}
int8_t Menu::runSimple() {
	uint8_t key;
	render();
	do {
		key = run();
		if(key == BUTTON_NONE) waitRelease_ = false;

		if(!waitRelease_ && key == BUTTON_START)  {
			waitRelease_ = true;
			return getIndex();
		}
	} while(key != BUTTON_STOP || waitRelease_);
	return MENU_EXIT;
}


void Menu::incIndex() {
	if(pos_ < LCD_LINES - 1) pos_++;
	else if( begin_ + pos_ < getMenuSize() - 1) begin_++;
}
void Menu::decIndex() {
	if(pos_ > 0) pos_--;
	else if(begin_ > 0) begin_--;
}


void Menu::display() {
	uint8_t lines = LCD_LINES;
	for(uint8_t i = 0; i < lines; i++) {
		lcd.setCursor(0, i);
		lcd.print(' ');
		printItem(i+begin_);
		lcdPrintSpaces();
	}
	lcd.setCursor(0,pos_);
	lcd.print('>');
	lcd.setCursor(LCD_COLUMNS - 1,pos_);
	lcd.print('<');
	render_ = false;
}


