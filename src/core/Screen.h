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
#ifndef SCREEN_H_
#define SCREEN_H_

#include <inttypes.h>
#include "Blink.h"

namespace Screen {
    enum ScreenType {
        ScreenEnd,
        ScreenStartInfo,
        ScreenFirst, ScreenCIVlimits, ScreenTime, ScreenTemperature,
        ScreenBalancer1_3,      ScreenBalancer4_6,      ScreenBalancer7_9,
        ScreenBalancer1_3Rth,   ScreenBalancer4_6Rth,   ScreenBalancer7_9Rth,
        ScreenDeltaFirst,
        ScreenR,
        ScreenVout,
        ScreenVinput,
        ScreenDeltaVout,
        ScreenDeltaTextern,
        ScreenEnergy,  //multiscreen
        ScreenCycles,  //multiscreen
        };

    extern Blink blink;

    uint16_t getTimeSec();
    uint16_t getTotalChargDischargeTime();
    uint8_t getChargeProcent();
    void doSlowInterrupt();

    void powerOn();
    void powerOff();

    void display(ScreenType screen);

    void displayScreenFirst();
    void displayScreenCIVlimits();
    void displayScreenTime();
    void displayScreenTemperature();
    void displayScreenR();
    void displayScreenVout();
    void displayScreenVinput();
    void displayDeltaVout();
    void displayDeltaTextern();
    void displayDeltaFirst();
    void displayScreenEnergy();
    void displayScreenCycles();
    void displayCalibrationErrorScreen(uint8_t errNo);
    void displayAnimation();
    void getDeltaProcentTimeSec();
    uint16_t getETATime();
    void resetETA();
    void resetCycleHistory();
    void storeCycleHistoryInfo();
    bool displayBlink();

    void displayScreenProgramCompleted();

    void displayMonitorError();
    void displayStartInfo();

    void printCharge();
    void printChar_Time();

    void displayStrings(const char *s1, const char *s2);
    void displayNotImplemented();
    void displayScreenReversedPolarity();

    void runCalibrateBeforeUse();
    void runNotImplemented();
};

#endif /* SCREEN_H_ */
