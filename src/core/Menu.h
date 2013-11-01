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
#ifndef MENU_H_
#define MENU_H_

#include <inttypes.h>
#include "PolarityCheck.h"

class Menu : public PolarityCheck {
public:
    static const int8_t MENU_EXIT = -1;

    uint8_t pos_;
    uint8_t begin_;
    uint8_t size_;
    bool render_;
    bool waitRelease_;
public:
    Menu(uint8_t size);
    void render() { render_ = true; }

    uint8_t run();
    int8_t runSimple(bool animate = false);

    void incIndex();
    void decIndex();

    uint8_t getIndex() { return begin_ + pos_; }
    virtual uint8_t printItem(uint8_t i) {}
    uint8_t getMenuSize() const { return size_; }

    void display();
};

#endif /* MENU_H_ */

