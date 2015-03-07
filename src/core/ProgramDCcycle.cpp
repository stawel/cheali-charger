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

#include "ProgramDCcycle.h"
#include "Program.h"
#include "DelayStrategy.h"
#include "Settings.h"
#include "Monitor.h"
#include "ScreenCycle.h"

using namespace Program;

namespace ProgramDCcycle {
    uint8_t currentCycle;

    Strategy::statusType runDCRestTime()
    {
        DelayStrategy::setDelay(ProgramData::battery.DCRestTime);
        Strategy::strategy = &DelayStrategy::vtable;
        return Strategy::doStrategy();
    }
}


Strategy::statusType ProgramDCcycle::runDCcycle(uint8_t firstCycle, uint8_t lastCycle)
{
    Strategy::statusType status = Strategy::COMPLETE;
    Strategy::exitImmediately = true;
    currentCycle = firstCycle;
    while(true) {
        if (currentCycle == lastCycle) {
            Strategy::exitImmediately = false;
        }

        status = Program::runWithoutInfo(currentCycle & 1 ? Program::Charge : Program::Discharge);
        if(status != Strategy::COMPLETE || (!Strategy::exitImmediately)) break;

        currentCycle++;

        Monitor::resetAccumulatedMeasurements();
        AnalogInputs::resetAccumulatedMeasurements();

        status = runDCRestTime();
        if(status != Strategy::COMPLETE) break;
    }
    return status;
}

