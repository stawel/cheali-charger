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
#include "Hardware.h"

#ifdef ENABLE_LCD_RAM_CG
   #define SCREEN_FULL_CELL_CHAR   2
   #define SCREEN_AVR_CELL_CHAR    1
   #define SCREEN_EMPTY_CELL_CHAR  0
#else
   #define SCREEN_FULL_CELL_CHAR   255    //'!'-34 (fulfill char)
   #define SCREEN_AVR_CELL_CHAR    '-'
   #define SCREEN_EMPTY_CELL_CHAR  '_'
#endif

#define PAGE_NONE 0
#define PAGE_ALWAYS                 0x7fffffff

#define PAGE_START_INFO             (1L<<30)
#define PAGE_BALANCE_PORT           (1L<<29)
#define PAGE_PROGRAM(program)       (1<<(program))
#define PAGE_BATTERY(_class)        ((1<<9)<<(_class))

namespace Screen {

    extern Blink blink;
    extern uint8_t keyboardButton;

    void displayCalibrationErrorScreen(uint8_t errNo);

    void displayScreenProgramCompleted();
    void displayMonitorError();
    void displayStrings(const char *s);
    void displayStrings(const char *s1, const char *s2);


    void displayNotImplemented();
    void displayScreenReversedPolarity();

    void displayResettingEeprom();
    void runAskResetEeprom(uint8_t what);
    void runResetEepromDone(uint8_t before, uint8_t after);
    void runNotImplemented();
    void runWelcomeScreen();
    void runCalibrationError(const char *s);

    void runNeedForceBalance();

    void initialize();
    void doStrategy();
    void powerOn();
    void powerOff();
};

#endif /* SCREEN_H_ */
