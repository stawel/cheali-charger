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
#include "memory.h"

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

    inline uint16_t isqrt(uint32_t x) {
        uint16_t retu = 0, b = 1<<15;
        while(b) {
            retu |= b;
            if(uint32_t(retu)*retu > x) {
                retu ^= b;
            }
            b >>= 1;
        }
        return retu;
    }

    uint16_t maxCurrent(AnalogInputs::ValueType Vout, AnalogInputs::ValueType Iout) {
        if(Vout == 0) {
            //assume 4 Ohms resistance
            Vout = ANALOG_VOLT(4);
            Iout = ANALOG_AMP(1);
        }

        uint32_t i = ProgramData::battery.power;
        //i =  (P/R)^0.5, R = Vout/Iout
        i *= Iout;
        i /= Vout;
        i *= ANALOG_VOLT(1) * ANALOG_AMP(1) / ANALOG_WATT(1);
        return isqrt(i);
    }

    void setIout(int32_t i);
}


AnalogInputs::ValueType Hakko907Strategy::getTemperature()
{
    //TODO: ?? "out of air" data
    // A) 52Ohm  -  23C
    // B) 100Ohm - 250C
    int32_t t = getTprobeOhms_x100();
    t -= 5200;
    t *= ANALOG_CELCIUS(250) - ANALOG_CELCIUS(23);
    t /= 10000 - 5200;
    t += ANALOG_CELCIUS(23);
    return t;
}


void Hakko907Strategy::powerOn()
{
    SMPS::powerOn();
    PID_I = 0;
    SMPS::trySetIout(ANALOG_AMP(0.5));
}

void Hakko907Strategy::powerOff()
{
    SMPS::powerOff();
}

#define PID_precision 5
#define Ki 3
#define Kp 64

void Hakko907Strategy::setIout(int32_t i) {
    if(i<0) i=0;
    int16_t maxI = maxCurrent(AnalogInputs::getVbattery(), AnalogInputs::getIout());

    i >>= PID_precision;
    if(i > maxI) i = maxI;

    SMPS::setIout(i);

}

Strategy::statusType Hakko907Strategy::doStrategy()
{
    AnalogInputs::ValueType T = getTemperature();
    if (T > ANALOG_CELCIUS(400)) {
        Program::stopReason = Monitor::string_externalTemperatureCutOff;
        return Strategy::ERROR;
    }

    //simple PID (I) - TODO: rewrite

    int32_t Iout, T_error;
    T_error = ProgramData::battery.T1;
    T_error -= getTemperature();

    PID_I += Ki*T_error;

    Iout = PID_I + Kp *T_error;

    // truncate I part
    if(PID_I < 0) PID_I = 0;
    //MAX_CHARGE_I ??
    if(PID_I > (MAX_CHARGE_I<<PID_precision)) PID_I = MAX_CHARGE_I<<PID_precision;

    setIout(Iout);
    //make sure output power is smaller than ProgramData::battery.power

    return Strategy::RUNNING;
}



