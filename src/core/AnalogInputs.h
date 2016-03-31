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
#include "cpu_config.h"

#define ANALOG_INPUTS_MAX_CALIBRATION_POINTS    2
#define ANALOG_INPUTS_DELTA_TIME_MILISECONDS    30000
#define ANALOG_INPUTS_RESOLUTION                16  // bits

#define ANALOG_INPUTS_MAX_ADC_VALUE      (((1<<(ANALOG_INPUTS_ADC_RESOLUTION_BITS))-1) << ((ANALOG_INPUTS_RESOLUTION) - (ANALOG_INPUTS_ADC_RESOLUTION_BITS)))

#ifdef SDCC_COMPILER
//TODO: sdcc
#define ANALOG_INPUTS_FOR_ALL_PHY(iterator) enum Name iterator; for(iterator = 0; iterator < ANALOG_INPUTS_PHYSICAL_INPUTS; iterator++)
#define ANALOG_INPUTS_FOR_ALL(iterator)     enum Name iterator; for(iterator = 0; iterator < ANALOG_INPUTS_ALL_INPUTS;      iterator++)
#else
#define ANALOG_INPUTS_FOR_ALL_PHY(iterator) for(AnalogInputs::Name iterator = AnalogInputs::Name(0); iterator < ANALOG_INPUTS_PHYSICAL_INPUTS; iterator = AnalogInputs::Name(iterator + 1) )
#define ANALOG_INPUTS_FOR_ALL(iterator)     for(AnalogInputs::Name iterator = AnalogInputs::Name(0); iterator < ANALOG_INPUTS_ALL_INPUTS;      iterator = AnalogInputs::Name(iterator + 1) )
#endif

#define ANALOG_INPUTS_PHYSICAL_INPUTS               11 + MAX_BANANCE_CELLS
#define ANALOG_INPUTS_ALL_INPUTS                    14 + MAX_BANANCE_CELLS + ANALOG_INPUTS_PHYSICAL_INPUTS
#define ANALOG_INPUTS_REVERSE_POLARITY_MIN_VOLTAGE  ANALOG_VOLT(1.000)
#define ANALOG_INPUTS_CONNECTED_MIN_VOLTAGE         ANALOG_VOLT(0.400)

namespace AnalogInputs {

    struct CalibrationPoint {
        ValueType x;
        ValueType y;
    } CHEALI_EEPROM_PACKED;

    struct DefaultValues {
        struct CalibrationPoint p0;
        struct CalibrationPoint p1;
    } CHEALI_EEPROM_PACKED;

    struct Calibration {
        struct CalibrationPoint p[ANALOG_INPUTS_MAX_CALIBRATION_POINTS];
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

    //get the average ADC value
    ValueType getAvrADCValue(enum Name name);
    //get real value (usable) - average, after calibration
    ValueType getRealValue(enum Name name);
    //get the ADC (measured) value - in this particular moment
    ValueType getADCValue(enum Name name);

    // get battery voltage, can be Vout or balance port voltage (when connected)
    ValueType getVbattery();
    // get voltage on output
    ValueType getVout();
    ValueType getIout();
    ValueType getDeltaLastT();
    ValueType getDeltaCount();
    ValueType getCharge();
    ValueType getEout();
    void enableDeltaVoutMax(bool enable);

    uint16_t getConnectedBalancePorts();
    uint8_t getConnectedBalancePortsCount();

    uint16_t getFullMeasurementCount();
    uint16_t getStableCount(enum Name name);

    enum Type getType(enum Name name);

    bool isOutStable();
    bool isStable(enum Name name);
    bool isConnected(enum Name name);
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

    ValueType calibrateValue(enum Name name, ValueType x);
    ValueType reverseCalibrateValue(enum Name name, ValueType y);

//init
    void initialize();

    void printRealValue(enum Name name, uint8_t dig);
}


inline uint16_t absDiff(uint16_t x, uint16_t y)
{
    if(x > y) return x - y;
    return y - x;
}


#endif /* ANALOGINPUTS_H_ */
