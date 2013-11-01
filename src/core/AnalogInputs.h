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
#define ANALOG_WATT(x) ((unsigned long)((x)*1000L))

namespace AnalogInputs {
    typedef uint16_t ValueType;

    struct CalibrationPoint {
        ValueType x;
        ValueType y;
    };
    struct DefaultValues {
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

        Vb0_pin,
        Vb1_pin,
        Vb2_pin,
        Vb3_pin,
        Vb4_pin,
        Vb5_pin,
        Vb6_pin,

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

        Iout,
        Pout,
        Cout,
        Eout,

        deltaVout,
        deltaVoutMax,
        deltaTextern,
        deltaLastCount,

        Vb1,
        Vb2,
        Vb3,
        Vb4,
        Vb5,
        Vb6,

        LastInput,
    };
    static const uint8_t    PHYSICAL_INPUTS     = VirtualInputs - Vout;
    static const uint8_t    ALL_INPUTS          = LastInput - Vout;
    static const uint16_t   AVR_MAX_COUNT       = 100;
    static const ValueType  STABLE_VALUE_ERROR  = 6;
    static const uint16_t   STABLE_MIN_VALUE    = 3;
    static const ValueType  REVERSE_POLARITY_MIN_VALUE = ANALOG_VOLT(1.000);

    //get the average ADC value
    ValueType getAvrADCValue(Name name);
    //get real value (usable) - average, after calibration
    ValueType getRealValue(Name name);
    //get the ADC (measured) value - in this particular moment
    ValueType getADCValue(Name name);

    ValueType calibrateValue(Name name, ValueType x);
    ValueType reverseCalibrateValue(Name name, ValueType y);

    ValueType getVout();
    ValueType getIout();
    bool isOutStable();

    void doFullMeasurement();

    void finalizeMeasurement();
    void finalizeDeltaMeasurement();
    void finalizeFullMeasurement();
    void finalizeFullVirtualMeasurement();

    void clearAvr();
    void resetADC();
    void reset();
    void resetMeasurement();
    void resetDelta();
    void powerOn();
    void powerOff();
    bool isPowerOn();

    void initialize();
    void doSlowInterrupt();
    uint16_t getCharge();

    void restoreDefault();
    void getCalibrationPoint(CalibrationPoint &p, Name name, uint8_t i);
    void setCalibrationPoint(Name name, uint8_t i, const CalibrationPoint &p);

    uint8_t getConnectedBalancePorts();
    bool isConnected(Name name);

    uint16_t getFullMeasurementCount();
    void printRealValue(Name name, uint8_t dig);
    static Type getType(Name name);
    uint16_t getStableCount(Name name);
    bool isStable(Name name);
    void resetStable();
    bool isReversePolarity();


    extern const DefaultValues inputsP_[AnalogInputs::PHYSICAL_INPUTS];
    extern ValueType real_[ALL_INPUTS];
    extern ValueType adc_[PHYSICAL_INPUTS];
    extern bool on_;
    extern uint16_t calculationCount_;
    extern ValueType   deltaLastT_;
    extern uint16_t    deltaCount_;

};

template<class T>
inline T absDiff(T x, T y)
{
    if(x > y) return x - y;
    return y - x;
}


#endif /* ANALOGINPUTS_H_ */
