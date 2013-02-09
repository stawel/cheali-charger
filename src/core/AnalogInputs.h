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
#ifndef ANALOGINPUTS_H_
#define ANALOGINPUTS_H_

#include <Arduino.h>

#define MAX_CALIBRATION_POINTS 2
#define DELTA_TIME_MILISECONDS 60000


#define ANALOG_INPUTS_RESOLUTION         16  // bits
#define STD_ANALOG_INPUTS_RESOLUTION     10  // bits
#define MAKE_ANALOG_INPUTS_RESOLUTION(val, bits) ((val) << (ANALOG_INPUTS_RESOLUTION - (bits)))

#define FOR_ALL_PHY_INPUTS(iterator) for(AnalogInputs::Name iterator = AnalogInputs::Name(0); iterator < AnalogInputs::PHYSICAL_INPUTS; iterator = AnalogInputs::Name(iterator + 1) )
#define FOR_ALL_INPUTS(iterator)     for(AnalogInputs::Name iterator = AnalogInputs::Name(0); iterator < AnalogInputs::ALL_INPUTS;      iterator = AnalogInputs::Name(iterator + 1) )

#define ANALOG_CELCIUS(x) ((AnalogInputs::ValueType)((x)*100))
#define ANALOG_CHARGE(x) ((AnalogInputs::ValueType)((x)*1000))
#define ANALOG_VOLT(x) ((AnalogInputs::ValueType)((x)*1000))
#define ANALOG_AMP(x) ((AnalogInputs::ValueType)((x)*1000))

class AnalogInputs {
public:
    typedef uint16_t ValueType;
    typedef uint16_t (*MeasureFunction)();

    struct CalibrationPoint {
        ValueType x;
        ValueType y;
    };
    struct DefaultValues {
        MeasureFunction f;
        CalibrationPoint p0;
        CalibrationPoint p1;
    };
    struct Calibration {
        CalibrationPoint p[MAX_CALIBRATION_POINTS];
    };

    enum Type {
        Current,
        Voltage,
        Temperature,
        Charge,
        Resistance,
        Unknown
    };
    enum Name {
        Vout,
        VreversePolarity,
        Ismps,
        Idischarge,

        VoutMux,
        Tintern,
        Vin,
        Textern,

        Vb1,
        Vb2,
        Vb3,
        Vb4,
        Vb5,
        Vb6,

        IsmpsValue,
        IdischargeValue,

#ifdef ANALOG_INPUTS_V_UNKNOWN
        Vunknown0,
        Vunknown1,
#endif

        VirtualInputs,
        Vbalancer,
        VoutBalancer,
        VobInfo,
        VbalanceInfo,
        deltaVout,
        deltaVoutMax,
        deltaTextern,
        deltaLastCount,

        LastInput,
    };
    static const uint8_t    PHYSICAL_INPUTS     = VirtualInputs - Vout;
    static const uint8_t    ALL_INPUTS          = LastInput - Vout;
    static const uint16_t   AVR_MAX_COUNT       = 100;
    static const ValueType  STABLE_VALUE_ERROR  = 2;
    static const uint16_t   STABLE_MIN_VALUE    = 3;
    static const ValueType  REVERSE_POLARITY_MIN_VALUE = 1000;

    AnalogInputs(const DefaultValues * inputs_P);

    //get the average (measured) value
    ValueType getValue(Name name) const             { return avrValue_[name]; }
    //get real value (usable) - average, after calibration
    ValueType getRealValue(Name name) const        { return real_[name]; }
    //get the measured value - in this particular moment
    ValueType getMeasuredValue(Name name) const    { return measured_[name]; }

    ValueType calibrateValue(Name name, ValueType x) const;
    ValueType reverseCalibrateValue(Name name, ValueType y) const;

    void measureValue(Name name);
    void doMeasurement(uint16_t count = PHYSICAL_INPUTS);
    void doFullMeasurement();
    void doVirtualCalculations();
    void doCalculations();
    void doDeltaCalculations();

    void clearAvr();
    void reset();
    void resetMeasurement();
    void resetDelta();
    void powerOn();
    void powerOff() { on_ = false; }
    bool isPowerOn() { return on_; }


    void restoreDefault();
    static void getCalibrationPoint(CalibrationPoint &p, Name name, uint8_t i);
    static void setCalibrationPoint(Name name, uint8_t i, const CalibrationPoint &p);

    int getConnectedBalancePorts() const;
    bool isConnected(Name name) const;

    template<uint8_t analogInput>
    static uint16_t analogValue() {
        // TODO: think it over
        ::analogRead(analogInput);
        delayMicroseconds(30);
        return MAKE_ANALOG_INPUTS_RESOLUTION(::analogRead(analogInput), STD_ANALOG_INPUTS_RESOLUTION);
    }

    uint16_t getCalculationCount() { return calculationCount_; }
    void printRealValue(Name name, uint8_t dig) const;
    void printMeasuredValue(Name name, uint8_t dig) const;
    static Type getType(Name name);
    uint16_t getStableCount(Name name) const { return stableCount_[name]; };
    bool isStable(Name name) const { return stableCount_[name] >= STABLE_MIN_VALUE; };
    void resetStable();
    bool isReversePolarity();

//protected:
    void setReal(Name name, ValueType real);
    bool on_;
    const DefaultValues * inputsP_;
    uint16_t avrCount_;
    uint32_t avrSum_[PHYSICAL_INPUTS];
    Name currentInput_;
    ValueType avrValue_[PHYSICAL_INPUTS];
    ValueType measured_[PHYSICAL_INPUTS];
    ValueType real_[ALL_INPUTS];
    uint16_t stableCount_[ALL_INPUTS];

    uint16_t calculationCount_;

    uint16_t    deltaCount_;
    uint16_t    deltaAvrCount_;
    uint32_t    deltaAvrSumVout_;
    uint32_t    deltaAvrSumTextern_;
    ValueType   deltaLastT_;
    uint32_t    deltaStartTime_;
};

template<class T>
inline T absDiff(T x, T y)
{
    if(x > y) return x - y;
    return y - x;
}


#endif /* ANALOGINPUTS_H_ */
