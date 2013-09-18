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
#ifndef SERIALLOG_H_
#define SERIALLOG_H_

#define SERIAL_SPEED 57600
//#define SERIAL_SPEED 9600

namespace SerialLog {

    void initialize();
    void powerOn();
    void send();
    void powerOff();

    void sendTime(uint32_t t);
    void printChar(char c);
    void printString(char *s);
    void printUInt(uint16_t x);
    void printD();
} //namespace SerialLog


#endif /* SERIALLOG_H_ */
