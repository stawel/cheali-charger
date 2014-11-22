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

namespace Strategy {

    const VTable * strategy;
    bool exitImmediately;

    AnalogInputs::ValueType endV;
    AnalogInputs::ValueType maxI;
    AnalogInputs::ValueType minI;
    uint8_t minIdiv;
    bool doBalance;

    void setMinI(AnalogInputs::ValueType i)  {
        if (i < settings.minIout) i = settings.minIout;
        minI = i;
    }

    void setVI(ProgramData::VoltageType vt, bool charge) {
        endV = ProgramData::currentProgramData.getVoltage(vt);

        if(charge) {
            maxI = ProgramData::currentProgramData.battery.Ic;
            if(vt == ProgramData::VCharge && ProgramData::currentProgramData.isLiXX()) {
                endV += settings.overCharge_LiXX * ProgramData::currentProgramData.battery.cells;
            }
        } else {
            maxI = ProgramData::currentProgramData.battery.Id;
            if(vt == ProgramData::VDischarge && ProgramData::currentProgramData.isLiXX()) {
                endV += settings.overDischarge_LiXX * ProgramData::currentProgramData.battery.cells;
            }
        }

        setMinI(maxI/minIdiv);
    }


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
        callVoidMethod_P(&strategy->powerOn);
    }

    void strategyPowerOff() {
        callVoidMethod_P(&strategy->powerOff);
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
        Screen::keyboardButton = BUTTON_NONE;
        bool run = true;
        uint16_t newMesurmentData = 0;
        Strategy::statusType status = Strategy::RUNNING;
        strategyPowerOn();
        Screen::powerOn();
        do {
            Screen::keyboardButton =  Keyboard::getPressedWithSpeed();
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
            if(!run && exitImmediately)
                return status;
        } while(Screen::keyboardButton != BUTTON_STOP);

        Screen::powerOff();
        strategyPowerOff();
        return status;
    }
} // namespace Strategy

