/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2016  Paweł Stawicki. All right reserved.

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
#include "Hakko907Strategy.h"
#include "ProgramData.h"
#include "Screen.h"
#include "Hardware.h"
#include "TheveninMethod.h"
#include "Program.h"

namespace Hakko907Strategy {

    Strategy::statusType doStrategy();

    const Strategy::VTable vtable PROGMEM = {
        powerOn,
        powerOff,
        doStrategy
    };

    int32_t PID_I;

    //TODO: a better calibration method is required
    AnalogInputs::ValueType getTprobeVolts_x10() {
        //TODO: ?
        uint32_t v = AnalogInputs::getAvrADCValue(AnalogInputs::Textern);
        v *= 2117;
        v /= 2685;
        return v;
    }
    AnalogInputs::ValueType getTprobeOhms_x100() {
        //TODO: ?
        const uint16_t Vs_x10 = 50440;//5.044V
        const uint16_t R = 1200;//1200Ohm (1kOhm+200Ohm)

        AnalogInputs::ValueType Vt_x10 = getTprobeVolts_x10();
        uint32_t v = Vt_x10;
        v *= R;
        v *= 100;
        v /= Vs_x10 - Vt_x10;
        return v;
    }

    int min(int a, int b) {
        if(a<b)return a;
        return b;
    }
}


AnalogInputs::ValueType Hakko907Strategy::getTemperature()
{
    //TODO: ?? "out of air" data
    // A) 52Ohm  -  23C
    // B) 100Ohm - 250C
    int32_t t = getTprobeOhms_x100();
    t -= 5200;
    t *= 25000 - 2300;
    t /= 10000 - 5200;
    t += 2300;
    return t;
}


void Hakko907Strategy::powerOn()
{
    SMPS::powerOn();
    PID_I = 0;
}

void Hakko907Strategy::powerOff()
{
    SMPS::powerOff();
}

Strategy::statusType Hakko907Strategy::doStrategy()
{
    AnalogInputs::ValueType T = getTemperature();
    AnalogInputs::ValueType Vout = AnalogInputs::getVbattery();
    if (T > ANALOG_CELCIUS(400)) {
        Program::stopReason = Monitor::string_externalTemperatureCutOff;
        return Strategy::ERROR;
    }

    //simple PID (I) - TODO: rewrite

    int T_error = ProgramData::battery.T1;
    T_error -= getTemperature();
    if(T_error > 0) {
        T_error *= 4;
    } else {
        T_error *= 8;
    }

    PID_I += T_error;

    if(PID_I < 0) PID_I = 0;

    //make sure output power is smaller than ProgramData::battery.power
    int maxI = MAX_CHARGE_I;
    if(Vout > 0) {
        maxI = AnalogInputs::evalI(ProgramData::battery.power, Vout);
    }
    maxI <<= 4;
    if(PID_I > maxI) PID_I = maxI;

    SMPS::setIout(PID_I>>4);

    return Strategy::RUNNING;
}



