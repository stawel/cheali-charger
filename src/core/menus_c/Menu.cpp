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
#include <stdint.h>
#include "Hardware.h"
#include "LcdPrint.h"
#include "Menu.h"


namespace Menu {
	void incIndex(struct Menu *d);
	void decIndex(struct Menu *d);
	void display(struct Menu *d);

	void debug() {}
}

uint8_t Menu::getIndex(struct Menu *d) { return d->begin_ + d->pos_; }


void Menu::initialize(struct Menu *d, uint8_t size, PrintMethod printItem) {
	d->pos_=0;
	d->begin_=0;
	d->size_ = size;
	d->render_ = true;
	d->waitRelease_ = true;
	d->printItem = printItem;
}

uint8_t Menu::run(struct Menu *d) {
    uint8_t button = Keyboard::getPressedWithDelay();

    if(PolarityCheck::runReversedPolarityInfo()) {
        d->render_ = true;
        return PolarityCheck::REVERSED_POLARITY;
    }

    uint8_t index = getIndex(d);
    switch (button) {
    case BUTTON_INC:
        incIndex(d);
        break;
    case BUTTON_DEC:
        decIndex(d);
        break;
    }
    if(index != getIndex(d))
        d->render_ = true;

    if(d->render_)
        display(d);

    return button;
}
int8_t Menu::runSimple(struct Menu *d, bool animate) {
    uint8_t key;
    d->render_ = true;
    do {
        key = run(d);
        if(key == BUTTON_NONE) d->waitRelease_ = false;

        if(!d->waitRelease_ && key == BUTTON_START)  {
            d->waitRelease_ = true;
            return getIndex(d);
        }
        if(animate) d->render_ = true;
    } while(key != BUTTON_STOP || d->waitRelease_);
    return MENU_EXIT;
}


void Menu::incIndex(struct Menu *d) {
    uint8_t lines = LCD_LINES;
    if(d->size_ < lines) lines = d->size_;
    if(d->pos_ < lines - 1) d->pos_++;
    else if(d->begin_ + d->pos_ < d->size_ - 1) d->begin_++;
}
void Menu::decIndex(struct Menu *d) {
    if(d->pos_ > 0) d->pos_--;
    else if(d->begin_ > 0) d->begin_--;
}

void Menu::display(struct Menu *d) {
    uint8_t lines = LCD_LINES;
    for(uint8_t i = 0; i < lines; i++) {
        lcdSetCursor(0, i);
        lcdPrintChar(i == d->pos_ ? '>' : ' ');
        if(i + d->begin_ < d->size_) {
            d->printItem(d, i + d->begin_);
        }
        lcdPrintSpaces();
    }
    debug();
    d->render_ = false;
}

