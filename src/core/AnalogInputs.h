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

#include "AnalogInputsTypes.h"
#include "HardwareConfig.h"
#include "memory.h"

#define ANALOG_INPUTS_MAX_CALIBRATION_POINTS    2
#define ANALOG_INPUTS_DELTA_TIME_MILISECONDS    30000
#define ANALOG_INPUTS_RESOLUTION                16  // bits

#define ANALOG_INPUTS_MAX_ADC_VALUE      (((1<<(ANALOG_INPUTS_ADC_RESOLUTION_BITS))-1) << ((ANALOG_INPUTS_RESOLUTION) - (ANALOG_INPUTS_ADC_RESOLUTION_BITS)))

#define ANALOG_INPUTS_FOR_ALL_PHY(iterator) for(AnalogInputs::Name iterator = AnalogInputs::Name(0); iterator < AnalogInputs::PHYSICAL_INPUTS; iterator = AnalogInputs::Name(iterator + 1) )
#define ANALOG_INPUTS_FOR_ALL(iterator)     for(AnalogInputs::Name iterator = AnalogInputs::Name(0); iterator < AnalogInputs::ALL_INPUTS;      iterator = AnalogInputs::Name(iterator + 1) )

namespace AnalogInputs {

    extern uint32_t tmp_time_;
    extern uint32_t tmp_time_last_;

    struct CalibrationPoint {
        ValueType x;
        ValueType y;
    } CHEALI_EEPROM_PACKED;

    struct DefaultValues {
        CalibrationPoint p0;
        CalibrationPoint p1;
    } CHEALI_EEPROM_PACKED;

    struct Calibration {
        CalibrationPoint p[ANALOG_INPUTS_MAX_CALIBRATION_POINTS];
    } CHEALI_EEPROM_PACKED;

    enum Name {
        Vout_plus_pin,
        Vout_minus_pin,
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

#if MAX_BANANCE_CELLS > 6
        Vb7_pin,
        Vb8_pin,
#endif

        IsmpsSet,
        IdischargeSet,

        VirtualInputs,
        Vout,
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

#if MAX_BANANCE_CELLS > 6
        Vb7,
        Vb8,
#endif

        LastInput,
    };
    static const uint8_t    PHYSICAL_INPUTS     = VirtualInputs - Vout_plus_pin;
    static const uint8_t    ALL_INPUTS          = LastInput - Vout_plus_pin;
    static const ValueType  REVERSE_POLARITY_MIN_VOLTAGE = ANALOG_VOLT(1.000);
    static const ValueType  CONNECTED_MIN_VOLTAGE = ANALOG_VOLT(0.600);
    static const ValueType  CONNECTED_MIN_CURRENT = ANALOG_AMP(0.050);

    //get the average ADC value
    ValueType getAvrADCValue(Name name);
    //get real value (usable) - average, after calibration
    ValueType getRealValue(Name name);
    //get the ADC (measured) value - in this particular moment
    ValueType getADCValue(Name name);

    // get battery voltage, can be Vout or balance port voltage (when connected)
    ValueType getVbattery();
    // get voltage on output
    ValueType getVout();
    ValueType getIout();
    ValueType getDeltaLastT();
    ValueType getDeltaCount();
    uint16_t getCharge();
    void enableDeltaVoutMax(bool enable);

    uint16_t getConnectedBalancePorts();
    uint8_t getConnectedBalancePortsCount();

    uint16_t getFullMeasurementCount();
    uint16_t getStableCount(Name name);

    Type getType(Name name);

    bool isOutStable();
    bool isStable(Name name);
    bool isConnected(Name name);
    bool isBalancePortConnected();
    bool isReversePolarity();
    bool isPowerOn();

    void doFullMeasurement();

    void resetMeasurement();
    void resetAccumulatedMeasurements();
    void powerOn(bool enableBatteryOutput = true);
    void powerOff();


//calibration
    void restoreDefault();

    ValueType calibrateValue(Name name, ValueType x);
    ValueType reverseCalibrateValue(Name name, ValueType y);

//init
    void initialize();

    void printRealValue(Name name, uint8_t dig);
};

template<class T>
inline T absDiff(T x, T y)
{
    if(x > y) return x - y;
    return y - x;
}


#endif /* ANALOGINPUTS_H_ */
