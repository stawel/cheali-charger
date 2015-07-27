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
#include "Screen.h"

#define STRATEGY_DISABLE_OUTPUT_AFTER_SECONDS (3*60)

namespace Strategy {

    const VTable * strategy;
    bool exitImmediately;

    AnalogInputs::ValueType endV;
    AnalogInputs::ValueType maxI;
    AnalogInputs::ValueType minI;
    bool doBalance;

    void setVI(ProgramData::VoltageType vt, bool charge) {
        endV = ProgramData::getVoltage(vt);

        if(charge) {
            maxI = ProgramData::battery.Ic;
            minI = ProgramData::battery.minIc;
        } else {
            maxI = ProgramData::battery.Id;
            minI = ProgramData::battery.minId;
        }

    }

    void strategyPowerOn() {
        callVoidMethod_P(&strategy->powerOn);
    }

    void strategyPowerOff() {
        callVoidMethod_P(&strategy->powerOff);
    }


    void chargingEnd() {
        strategyPowerOff();
        Monitor::powerOff();
        lcdClear();
    }

    void waitButtonOrDisableOutput() {
        uint16_t time = Time::getSecondsU16();

        do {
            if(Time::diffU16(time, Time::getSecondsU16()) > STRATEGY_DISABLE_OUTPUT_AFTER_SECONDS) {
                AnalogInputs::powerOff();
            }
        } while(Keyboard::getPressedWithDelay() == BUTTON_NONE);

        Buzzer::soundOff();
    }

    void chargingComplete() {
        chargingEnd();
        Screen::displayScreenProgramCompleted();
        Buzzer::soundProgramComplete();
        waitButtonOrDisableOutput();
    }

    void chargingMonitorError() {
        chargingEnd();
        AnalogInputs::powerOff();
        Screen::displayMonitorError();

        Buzzer::soundError();
        waitButtonOrDisableOutput();
    }

    Strategy::statusType strategyDoStrategy() {
        Strategy::statusType (*doStrategy)() = pgm::read(&strategy->doStrategy);
        return doStrategy();
    }


    bool analizeStrategyStatus(Strategy::statusType status) {
        if(status == Strategy::ERROR) {
            chargingMonitorError();
            return false;
        }

        if(status == Strategy::COMPLETE) {
            if(!exitImmediately)
                chargingComplete();
            return false;
        }
        return true;
    }

    Strategy::statusType doStrategy()
    {
        Screen::keyboardButton = BUTTON_NONE;
        bool run = true;
        uint16_t newMesurmentData = 0;
        Strategy::statusType status = Strategy::RUNNING;
        strategyPowerOn();
        do {
            Screen::keyboardButton =  Keyboard::getPressedWithDelay();
            Screen::doStrategy();

            if(run) {
                status = Monitor::run();
                run = analizeStrategyStatus(status);

                if(run && newMesurmentData != AnalogInputs::getFullMeasurementCount()) {
                    newMesurmentData = AnalogInputs::getFullMeasurementCount();
                    status = strategyDoStrategy();
                    run = analizeStrategyStatus(status);
                }
            }
            if(!run && exitImmediately && status != Strategy::ERROR)
                break;
        } while(Screen::keyboardButton != BUTTON_STOP);

        strategyPowerOff();
        return status;
    }
} // namespace Strategy

