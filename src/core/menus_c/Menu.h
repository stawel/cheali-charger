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

#define MENU_EXIT   -1

namespace Menu {

    typedef void (*CallMethod)();
    typedef void (*PrintMethod)(uint8_t item);
    typedef void (*EditMethod)(uint8_t item, uint8_t key);

    extern PrintMethod printMethod_;
    extern EditMethod editMethod_;
    extern uint8_t index_;
    extern uint8_t begin_;
    extern uint8_t size_;

    struct StaticMenu {
        const char * string;
        const CallMethod call;
    };

    int8_t runStatic(const struct StaticMenu * menu);

    void initialize(uint8_t size);
    int8_t run(bool alwaysRefresh = false);
    bool runEdit();

    inline uint8_t getIndex()             { return index_; }
    inline void setIndex(uint8_t index)   {  index_ = index; }

}

#endif /* MENU_H_ */

