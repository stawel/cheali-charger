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
#ifndef EDITNAME_H_
#define EDITNAME_H_

#include <inttypes.h>

#define MAX_EDIT_NAME 16

class EditName {
    char * name_;
    uint8_t size_;
    const char * printStringP_;
    uint8_t cursor_;
    uint8_t beginSL_;
    uint8_t cursorSL_;
public:
    EditName(char * name, int size, const char *printStringP);

    bool run();
    void mainScreen();
    void selectLetter();
    void selectLetterScreen();
    void save();
    void incSL();
    void decSL();
    uint8_t getSL();
};

#endif /* EDITNAME_H_ */
