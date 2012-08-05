#ifndef MENU_H_
#define MENU_H_

#include <inttypes.h>
#include "Hardware.h"
#include "LcdPrint.h"

class Menu {
public:
	uint8_t pos_;
	uint8_t begin_;
	uint8_t size_;
	bool render_;
public:
	Menu(uint8_t size):
		pos_(0), begin_(0), size_(size), render_(true){}

	void render() { render_ = true; }

	uint8_t run() {
		uint8_t button = keyboard.getPressedWithSpeed();
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

	void incIndex() {
		if(pos_ < LCD_LINES - 1) pos_++;
		else if( begin_ + pos_ < getMenuSize() - 1) begin_++;
	}
	void decIndex() {
		if(pos_ > 0) pos_--;
		else if(begin_ > 0) begin_--;
	}

	uint8_t getIndex() {
		return begin_ + pos_;
	}

	virtual void printItem(int i) {}

	uint8_t getMenuSize(){
		return size_;
	}

	void display() {
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
};

#endif /* MENU_H_ */

