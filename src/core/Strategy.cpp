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
#include "PolarityCheck.h"

namespace Strategy {

    const VTable * strategy_;


    void chargingComplete() {
        lcdClear();
        Screen::displayScreenProgramCompleted();      
#ifdef LOUDSOUND
        Buzzer::loudSound1();
#else
        Buzzer::soundProgramComplete();
#endif
        waitButtonPressed();
        Buzzer::soundOff();
    }

    void chargingMonitorError() {
        lcdClear();
        Screen::displayMonitorError();
        Buzzer::soundError();
        waitButtonPressed();
        Buzzer::soundOff();
    }

    void strategyPowerOn() {
        void (*powerOn)() = pgm::read(&strategy_->powerOn);
        powerOn();
    }
    void strategyPowerOff() {
        void (*powerOff)() = pgm::read(&strategy_->powerOff);
        powerOff();
    }
    Strategy::statusType strategyDoStrategy() {
        Strategy::statusType (*doStrategy)() = pgm::read(&strategy_->doStrategy);
        return doStrategy();
    }


    bool analizeStrategyStatus(Strategy::statusType status, bool exitImmediately) {
        bool run = true;
        if(status == Strategy::ERROR) {
            Screen::powerOff();
            strategyPowerOff();
            chargingMonitorError();
            run = false;
        }

        if(status == Strategy::COMPLETE) {
            Screen::powerOff();
            strategyPowerOff();
            if(!exitImmediately)
                chargingComplete();
            run = false;
        }
        return run;
    }

    Strategy::statusType doStrategy(const Screen::ScreenType chargeScreens[], bool exitImmediately)
    {
        uint8_t key;
        bool run = true;
        uint16_t newMesurmentData = 0;
        Strategy::statusType status = Strategy::RUNNING;
        strategyPowerOn();
        Screen::powerOn();
        Monitor::powerOn();
        lcdClear();
        uint8_t screen_nr = 0;
        do {
            if(!PolarityCheck::runReversedPolarityInfo()) {
                Screen::display(pgm::read(&chargeScreens[screen_nr]));
            }

            {
                //change displayed screen
                key =  Keyboard::getPressedWithSpeed();
                if(key == BUTTON_INC && pgm::read(&chargeScreens[screen_nr+1]) != Screen::ScreenEnd)
                    screen_nr++;
                if(key == BUTTON_DEC && screen_nr > 0)
                    screen_nr--;
            }

            if(run) {
                status = Monitor::run();
                run = analizeStrategyStatus(status, exitImmediately);

                if(run && newMesurmentData != AnalogInputs::getFullMeasurementCount()) {
                    newMesurmentData = AnalogInputs::getFullMeasurementCount();
                    status = strategyDoStrategy();
                    run = analizeStrategyStatus(status, exitImmediately);
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

