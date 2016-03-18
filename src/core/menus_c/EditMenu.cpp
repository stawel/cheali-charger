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
#include "EditMenu.h"
#include "Keyboard.h"



void EditMenu::initialize(Data *d, const char * const* staticMenu, EditMethod editItem) {
	StaticMenu::initialize(&d->menu, staticMenu);
	Blink::initialize();
}

void EditMenu::setPrintMethod(Data *d, PrintMethod printItem) {
	d->menu.d.printItem = (Menu::PrintMethod)printItem;
}

bool EditMenu::runEdit(Data *d)
{
    Blink::startBlinkOff(getIndex(&d->menu.d));
    uint8_t key;
    d->menu.d.render_ = true;
    do {
        key =  Keyboard::getPressedWithDelay();
        if(key == BUTTON_DEC || key == BUTTON_INC) {
        	uint8_t index = getIndex(&d->menu.d);
            d->editItem(d, index, key);
            Blink::startBlinkOn(index);
            d->menu.d.render_ = true;
        } else if(key == BUTTON_STOP || key == BUTTON_START) {
            break;
        }
        if(Blink::getBlinkChanged())
        	d->menu.d.render_ = true;
        if(d->menu.d.render_)
            display(&d->menu.d);
        Blink::incBlinkTime();
    } while(true);

    Blink::stopBlink();
    d->menu.d.waitRelease_ = true;
    d->menu.d.render_ = true;
    return key == BUTTON_START;
}
