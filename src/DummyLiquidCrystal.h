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
#ifdef LiquidCrystal_h
#define LiquidCrystal_h

#include <inttypes.h>

class LiquidCrystal {
public:
    LiquidCrystal(int rs, int enable,
            int d0, int d1, int d2, int d3) {};
    void begin(int,int) {};
    void clear(){};
    void setCursor(int x, int y){};
    void print(const char *){};
    void print(int){};
    void print(unsigned int){};
    void print(long int, int x = 1){};
    void print(unsigned long int, int x = 1){};
    void print(double, int x = 1){};

};

#define DummyLiquidCrystal_h


#endif

