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
#ifndef CALIBRATE_H_
#define CALIBRATE_H_

#include "AnalogInputs.h"

#define ACCEPT_DELAY 3

class Calibrate {
public:
    uint16_t value_;
    uint8_t dispVal_;
    int8_t blink_;
    bool blinkOn_;

    enum screenType { SCREEN_ICHARGE = 0, SCREEN_VOUT = 1, SCREEN_B1_3 = 2, SCREEN_B4_6 = 3,
            SCREEN_T = 4, SCREEN_VIN = 5, SCREEN_VREVERSE = 6, SCREEN_VUNKNOWN = 7,
            SCREEN_VOUT_VBAL = 99, SCREEN_IDISCHARGE = 100,
            SCREEN_B1_3_BLINK = 101, SCREEN_B4_6_BLINK = 102};

    Calibrate(){};

    void print_v(uint8_t dig = 3);
    void print_d(AnalogInputs::Name name, int dig = 7);
    void print_m(const char *str, AnalogInputs::Name name, int dig = 7);
    void print_m_2(const char *str, AnalogInputs::Name name, int dig = 7);
    void print_m_1(const char *str, AnalogInputs::Name name, int dig = 7);
    void print_m_3(const char *str, AnalogInputs::Name name, int dig = 7);
    void printCalibrateIcharge();
    void printCalibrateIdischarge();
    void printCalibrateVout();
    void printCalibrateVoutVbal();
    void printCalibrateB1_3();
    void printCalibrateB4_6();
    void printCalibrateT();
    void printCalibrateVin();
    void printCalibrateVreverse();
    void printCalibrateVunknown();

    void printCalibrateB1_3_Blink();
    void printCalibrateB4_6_Blink();

    void printCalibrate(screenType screen);
    void info(screenType screen);
    void infoDis();
    void infoTimeM();

    void run();
    void runInfo();
    bool calibrate(screenType screen);
    bool calibrateBlink(screenType screen, int8_t maxBlink);
    bool calibrateDischarge();

    void setBlink(screenType screen);
    void copyVbalVout();
    void calibrateI(screenType screen, AnalogInputs::Name name1, AnalogInputs::Name name2);
    void setBalancer(AnalogInputs::Name firstName);
    bool setValue(uint8_t x, uint8_t y, AnalogInputs::ValueType &v, AnalogInputs::Type, uint8_t dig);

};

#endif /* CALIBRATE_H_ */
