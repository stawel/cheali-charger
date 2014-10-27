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

#include "Strategy.h"
#include "LcdPrint.h"
#include "Buzzer.h"
#include "memory.h"
#include "Monitor.h"
#include "AnalogInputs.h"

namespace Strategy {

    const VTable * strategy;
    bool exitImmediately;


    void chargingComplete() {
        Monitor::powerOff();
        lcdClear();
        Screen::displayScreenProgramCompleted();
        Buzzer::soundProgramComplete();
        waitButtonPressed();
        Buzzer::soundOff();
    }

    void chargingMonitorError() {
        Monitor::powerOff();
        lcdClear();
        Screen::displayMonitorError();
        Buzzer::soundError();
        waitButtonPressed();
        Buzzer::soundOff();
    }

    void strategyPowerOn() {
        void (*powerOn)() = pgm::read(&strategy->powerOn);
        powerOn();
    }
    void strategyPowerOff() {
        void (*powerOff)() = pgm::read(&strategy->powerOff);
        powerOff();
    }
    Strategy::statusType strategyDoStrategy() {
        Strategy::statusType (*doStrategy)() = pgm::read(&strategy->doStrategy);
        return doStrategy();
    }


    bool analizeStrategyStatus(Strategy::statusType status) {
        bool run = true;
        if(status == Strategy::ERROR) {
            strategyPowerOff();
            chargingMonitorError();
            run = false;
        }

        if(status == Strategy::COMPLETE) {
            strategyPowerOff();
            if(!exitImmediately)
                chargingComplete();
            run = false;
        }
        return run;
    }

    Strategy::statusType doStrategy()
    {
        uint8_t key = BUTTON_NONE;
        bool run = true;
        uint16_t newMesurmentData = 0;
        Strategy::statusType status = Strategy::RUNNING;
        strategyPowerOn();
        Screen::powerOn();
        do {
            Screen::doStrategy(key);
            key =  Keyboard::getPressedWithSpeed();

            if(run) {
                status = Monitor::run();
                run = analizeStrategyStatus(status);

                if(run && newMesurmentData != AnalogInputs::getFullMeasurementCount()) {
                    newMesurmentData = AnalogInputs::getFullMeasurementCount();
                    status = strategyDoStrategy();
                    run = analizeStrategyStatus(status);
                }
            }
            if(!run && exitImmediately)
                return status;
        } while(key != BUTTON_STOP);

        Screen::powerOff();
        strategyPowerOff();
        return status;
    }
} // namespace Strategy

