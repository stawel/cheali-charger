/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013  Paweł Stawicki. All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "EditName.h"
#include "LcdPrint.h"
#include "Hardware.h"
#include "Buzzer.h"


void EditName::mainScreen()
{
    lcdSetCursor0_0();
    lcdPrint_P(printStringP_);
    lcdSetCursor0_1();
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
        lcdSetCursor(0,j);
        for(uint8_t i=0;i<LCD_COLUMNS;i++)
            lcdPrintChar((char)begin++);
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
        lcdSetCursor(cursorSL_&(LCD_COLUMNS-1), cursorSL_ >> 4);
        key = Keyboard::getPressedWithSpeed();
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
        lcdClear();
        mainScreen();
        do {
            lcdSetCursor(cursor_, 1);
            key = Keyboard::getPressedWithSpeed();
            switch(key) {
            case BUTTON_STOP:
                lcd.noBlink();
                return false;
            case BUTTON_INC:
                if(cursor_ < size_ - 1) cursor_++;
                break;
            case BUTTON_DEC:
                if (cursor_ > 0) cursor_--;
                break;
            }
        } while(key != BUTTON_START);

        if(Keyboard::getPressTime()>1) {
            save();
            lcd.noBlink();
            return true;
        }

        selectLetter();

    } while(1);
}

void EditName::save()
{
    Buzzer::soundSelect();
}
