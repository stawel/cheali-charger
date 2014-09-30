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
#ifndef GTPOWERA610_H_
#define GTPOWERA610_H_

#include "HardwareConfig.h"

#include "LiquidCrystal.h"
#include "Keyboard.h"
#include "SMPS.h"
#include "Discharger.h"
#include "Balancer.h"
#include "Timer1.h"
#include "Time.h"
#include "Buzzer.h"
#include "adc.h"

#include "GTPowerA6-10-pins.h"

#include STRINGS_HEADER

extern LiquidCrystal lcd;

namespace hardware {
    void initialize();
    uint8_t getKeyPressed();
    void delay(uint16_t t);
    void setLCDBacklight(uint8_t val);
    void setBuzzer(uint8_t val);
    void setBatteryOutput(bool enable);
    void setChargerOutput(bool enable);
    void setBalancerOutput(bool enable);
    void setDischargerOutput(bool enable);
    void setChargerValue(uint16_t value);
    void setDischargerValue(uint16_t value);
    void setFan(bool enable);
    void setBalancer(uint8_t balance);
    inline void doInterrupt(){}

    void soundInterrupt();
    inline void setExternalTemperatueOutput(bool enable) {};
}


#endif /* GTPOWERA610_H_ */
