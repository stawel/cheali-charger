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
#ifndef MULTIPLEXER_H_
#define MULTIPLEXER_H_

class Multiplexer {
public:
    static void init();
    static uint8_t digitalRead(uint8_t address);
    static uint16_t analogRead(uint8_t address);


    template<uint8_t address>
    static uint16_t analogRead() {
        return analogRead(address);
    }
    template<uint8_t address>
    static uint8_t digitalRead() {
        return digitalRead(address);
    }
};



#endif /* MULTIPLEXER_H_ */
