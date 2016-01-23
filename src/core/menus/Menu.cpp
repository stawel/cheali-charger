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
#include <inttypes.h>
#include "Hardware.h"
#include "LcdPrint.h"
#include "Menu.h"


Menu::Menu(uint8_t size):
        pos_(0), begin_(0), size_(size), render_(true), waitRelease_(true)
{}

uint8_t Menu::run() {
    uint8_t button = Keyboard::getPressedWithDelay();

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
int8_t Menu::runSimple(bool animate) {
    uint8_t key;
    render();
    do {
        key = run();
        if(key == BUTTON_NONE) waitRelease_ = false;

        if(!waitRelease_ && key == BUTTON_START)  {
            waitRelease_ = true;
            return getIndex();
        }
        if(animate) render();
    } while(key != BUTTON_STOP || waitRelease_);
    return MENU_EXIT;
}


void Menu::incIndex() {
    uint8_t lines = LCD_LINES;
    if(size_ < lines) lines = size_;
    if(pos_ < lines - 1) pos_++;
    else if( begin_ + pos_ < getMenuSize() - 1) begin_++;
}
void Menu::decIndex() {
    if(pos_ > 0) pos_--;
    else if(begin_ > 0) begin_--;
}

void Menu::display() {
    uint8_t lines = LCD_LINES;
    for(uint8_t i = 0; i < lines; i++) {
        lcdSetCursor(0, i);
        lcdPrintChar(i == pos_ ? '>' : ' ');
        if(i + begin_ < size_) {
            printItem(i + begin_);
        }
        lcdPrintSpaces();
    }
    debug();
    render_ = false;
}


void Menu::debug()
{
}
