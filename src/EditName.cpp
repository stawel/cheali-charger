#include "EditName.h"
#include "LcdPrint.h"
#include "Hardware.h"
#include "Buzzer.h"

#include <avr/eeprom.h>



void EditName::mainScreen()
{
	lcd.setCursor(0, 0);
	lcdPrint_P(printStringP_);
	lcd.setCursor(0, 1);
	lcdPrint(name_, size_);
}


void EditName::incSL()
{
	if(cursorSL_< LCD_COLUMNS*LCD_LINES -1) cursorSL_++;
	else beginSL_++;
}
void EditName::decSL()
{
	if(cursorSL_> 0) cursorSL_--;
	else beginSL_--;
}
uint8_t EditName::getSL()
{
	return beginSL_ + cursorSL_;
}

void EditName::selectLetterScreen()
{
	uint8_t begin = beginSL_;
	for(uint8_t j=0;j<LCD_LINES;j++) {
		lcd.setCursor(0,j);
		for(uint8_t i=0;i<LCD_COLUMNS;i++)
			lcd.print((char)begin++);
	}
}

void EditName::selectLetter()
{
	uint8_t key, begin=-1;
	lcd.blink();
	do {
		if(begin != beginSL_) {
			selectLetterScreen();
			begin = beginSL_;
		}
		lcd.setCursor(cursorSL_&(LCD_COLUMNS-1), cursorSL_ >> 4);
		key = keyboard.getPressedWithSpeed();
		switch(key) {
		case BUTTON_STOP: return;
		case BUTTON_INC: incSL(); break;
		case BUTTON_DEC: decSL(); break;
		}
		if(key == BUTTON_STOP)
			return;
	} while(key != BUTTON_START);

	name_[cursor_] = getSL();
}

EditName::EditName(char * name, int size, const char *printStringP)
{
		name_ = name;
		size_ = size;
		printStringP_ = printStringP;
		cursor_ = 0;
		beginSL_ = ' ';
		cursorSL_ = 0;
};



bool EditName::run(){
	lcd.blink();
	uint8_t key;
	do {
		lcd.clear();
		mainScreen();
		do {
			lcd.setCursor(cursor_, 1);
			key = keyboard.getPressedWithSpeed();
			switch(key) {
			case BUTTON_STOP:
				return false;
			case BUTTON_INC:
				if(cursor_ < size_ - 1) cursor_++;
				break;
			case BUTTON_DEC:
				if (cursor_ > 0) cursor_--;
				break;
			}
		} while(key != BUTTON_START);

		if(keyboard.getPressTime()>1) {
			save();
			return true;
		}

		selectLetter();

	} while(1);
}

void EditName::save()
{
	buzzer.soundSave();
}
