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
#ifndef ANALOGINPUTSTYPES_H_
#define ANALOGINPUTSTYPES_H_

#include <inttypes.h>

#define ANALOG_CELCIUS(x) ((AnalogInputs::ValueType)((x)*100))
#define ANALOG_CHARGE(x) ((AnalogInputs::ValueType)((x)*1000))
#define ANALOG_VOLT(x) ((AnalogInputs::ValueType)((x)*1000))
#define ANALOG_AMP(x) ((AnalogInputs::ValueType)((x)*1000))
#define ANALOG_OHM(x) ((AnalogInputs::ValueType)((x)*1000))
#define ANALOG_WATT(x) ((AnalogInputs::ValueType)((x)*100))
#define ANALOG_WATTH(x) ((AnalogInputs::ValueType)((x)*100))

#define ANALOG_MAX_TIME_LIMIT 1000 //infinity
#define ANALOG_MAX_CHARGE ANALOG_CHARGE(65.000)
#define ANALOG_MIN_CHARGE ANALOG_CHARGE(0.100)

namespace AnalogInputs {
    typedef uint16_t ValueType;

    enum Type {
        Current,
        Voltage,
        Power,
        Work,
        Temperature,
        Charge,
        Resistance,
        Procent,
        SignedVoltage,
        Unsigned,
        TemperatureMinutes,
        Minutes,
        TimeLimitMinutes,
        YesNo,
        Unknown
    };

    static const ValueType  STABLE_VALUE_ERROR  = 6;
    static const uint16_t   STABLE_MIN_VALUE    = 3;

    AnalogInputs::ValueType evalI(AnalogInputs::ValueType P, AnalogInputs::ValueType U);
};

#endif /* ANALOGINPUTSTYPES_H_ */
