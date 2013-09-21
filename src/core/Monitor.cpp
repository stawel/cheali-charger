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
#include "AnalogInputs.h"
#include "Hardware.h"

#include "Monitor.h"
#include "Utils.h"
#include "Settings.h"
#include "ProgramData.h"
#include <util/atomic.h>


namespace Monitor {

uint16_t VoutMaxMesured_;

#ifdef ENABLE_FAN
#ifdef ENABLE_T_INTERNAL
    AnalogInputs::ValueType monitor_on_T;
    AnalogInputs::ValueType monitor_off_T;
#endif
#endif
} // namespace Monitor



void Monitor::doInterrupt()
{
#ifdef ENABLE_FAN
#ifdef ENABLE_T_INTERNAL

    AnalogInputs::ValueType t = AnalogInputs::getMeasuredValue(AnalogInputs::Tintern);
    bool retu = false;
    if(t > monitor_off_T) {
        hardware::setFan(false);
    } else if(t < monitor_on_T) {
        hardware::setFan(true);
    }
#endif
#endif
}
void Monitor::update()
{
#ifdef ENABLE_FAN
#ifdef ENABLE_T_INTERNAL
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        monitor_off_T = AnalogInputs::reverseCalibrateValue(AnalogInputs::Tintern, settings.fanTempOn_ - Settings::TempDifference);
        monitor_on_T  = AnalogInputs::reverseCalibrateValue(AnalogInputs::Tintern, settings.fanTempOn_);
    }
#endif
#endif
}

void Monitor::powerOn() {
    VoutMaxMesured_ = AnalogInputs::reverseCalibrateValue(AnalogInputs::Vout, MAX_CHARGE_V+ANALOG_VOLT(3.000));
}


Strategy::statusType Monitor::run()
{
#ifdef ENABLE_T_INTERNAL
    AnalogInputs::ValueType t = AnalogInputs::getRealValue(AnalogInputs::Tintern);

    if(t > settings.dischargeTempOff_+Settings::TempDifference) {
        Program::stopReason_ = PSTR("intern T");
        return Strategy::ERROR;
    }
#endif

    AnalogInputs::ValueType VMout = AnalogInputs::getMeasuredValue(AnalogInputs::Vout);
    if(VMout > VoutMaxMesured_) {
        Program::stopReason_ = PSTR("bat disc");
        return Strategy::ERROR;
    }

    AnalogInputs::ValueType Vin = AnalogInputs::getRealValue(AnalogInputs::Vin);
    if(AnalogInputs::isConnected(AnalogInputs::Vin) && Vin < settings.inputVoltageLow_) {
        Program::stopReason_ = PSTR("input V");
        return Strategy::ERROR;
    }

    AnalogInputs::ValueType c = AnalogInputs::getRealValue(AnalogInputs::Cout);
    if(c > ProgramData::currentProgramData.getCapacityLimit()) {
        Program::stopReason_ = PSTR("cap COFF");
        return Strategy::COMPLETE;
    }
    if(settings.externT_) {
        AnalogInputs::ValueType Textern = AnalogInputs::getRealValue(AnalogInputs::Textern);
        if(Textern > settings.externTCO_) {
            Program::stopReason_ = PSTR("ext TCOF");
            return Strategy::COMPLETE;
        }
    }

    return Strategy::RUNNING;
}

