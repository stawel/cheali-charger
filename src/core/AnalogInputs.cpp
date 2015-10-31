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
#define __STDC_LIMIT_MACROS
#include "Hardware.h"
#include "AnalogInputsPrivate.h"
#include "memory.h"
#include "LcdPrint.h"
#include "SerialLog.h"
#include "eeprom.h"
#include "atomic.h"
#include "Balancer.h"


#define ANALOG_INPUTS_ADC_MEASUREMENTS_COUNT (ANALOG_INPUTS_ADC_ROUND_MAX_COUNT*ANALOG_INPUTS_ADC_BURST_COUNT)

#if (1<<ANALOG_INPUTS_RESOLUTION) * ANALOG_INPUTS_ADC_MEASUREMENTS_COUNT > UINT32_MAX
#error "avr sum don't fit into uint32_t"
#endif

//TODO: 120?? we take not more then 60 measurements into account
#if ((1<<ANALOG_INPUTS_RESOLUTION)>>ANALOG_INPUTS_ADC_DELTA_SHIFT) * ANALOG_INPUTS_ADC_MEASUREMENTS_COUNT * 120 > UINT32_MAX
#error "delta avr sum don't fit into uint32_t"
#endif


#define RETURN_ATOMIC(x)  \
    ValueType v; \
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {\
        v = x;\
    }\
    return v;\



namespace AnalogInputs {

    volatile bool on_;
    volatile bool ignoreLastResult_;
    volatile uint16_t  i_avrCount_;
    volatile uint32_t  i_avrSum_[PHYSICAL_INPUTS];
    volatile ValueType i_adc_[PHYSICAL_INPUTS];

    ValueType avrAdc_[PHYSICAL_INPUTS];
    ValueType real_[ALL_INPUTS];
    uint16_t stableCount_[ALL_INPUTS];

    uint16_t calculationCount_;
    uint32_t tmp_time_;
    uint32_t tmp_time_last_;

    uint16_t    i_deltaAvrCount_;
    uint32_t    i_deltaAvrSumVoutPlus_;
    uint32_t    i_deltaAvrSumVoutMinus_;
    uint32_t    i_deltaAvrSumTextern_;

    uint16_t    deltaCount_;
    ValueType   deltaLastT_;
    uint16_t    deltaStartTimeU16_;
    bool        enable_deltaVoutMax_;

    uint32_t    i_charge_;

    void _resetAvr();
    void _resetDeltaAvr();
    void resetADC();
    void reset();
    void resetDelta();
    void resetStable();


    ValueType getAvrADCValue(Name name)     { return avrAdc_[name];   }
    ValueType getRealValue(Name name)       { return real_[name]; }
    ValueType getADCValue(Name name)        { RETURN_ATOMIC(i_adc_[name]) }
    bool isPowerOn() { return on_; }
    uint16_t getFullMeasurementCount()      { return calculationCount_; }
    ValueType getDeltaLastT()               { return deltaLastT_;}
    ValueType getDeltaCount()               { return deltaCount_;}
    void enableDeltaVoutMax(bool enable)    { enable_deltaVoutMax_ = enable; }

    uint16_t getStableCount(Name name)      { return stableCount_[name]; };
    bool isStable(Name name)                { return getStableCount(name) >= STABLE_MIN_VALUE; };
    void setReal(Name name, ValueType real);
    void setRealBasedOnAvr(AnalogInputs::Name name);

    void finalizeDeltaMeasurement();
    void finalizeFullMeasurement();
    void finalizeFullVirtualMeasurement();


} // namespace AnalogInputs

//this method depends on the ADC implementation
void AnalogInputs::doFullMeasurement()
{
    resetMeasurement();
    uint16_t c = getFullMeasurementCount();
    while(c == getFullMeasurementCount())
        Time::delayDoIdle(10);
}


void AnalogInputs::restoreDefault()
{
    CalibrationPoint p;
    ANALOG_INPUTS_FOR_ALL_PHY(name) {
        p = pgm::read<CalibrationPoint>(&inputsP_[name].p0);
        setCalibrationPoint(name, 0, p);
        p = pgm::read<CalibrationPoint>(&inputsP_[name].p1);
        setCalibrationPoint(name, 1, p);
    }
    eeprom::restoreCalibrationCRC();
}

void AnalogInputs::getCalibrationPoint(CalibrationPoint &x, Name name, uint8_t i)
{
    if(name >= PHYSICAL_INPUTS || i >= ANALOG_INPUTS_MAX_CALIBRATION_POINTS) {
        x.x = x.y = 1;
        return;
    }
    eeprom::read<CalibrationPoint>(x,&eeprom::data.calibration[name].p[i]);
}
void AnalogInputs::setCalibrationPoint(Name name, uint8_t i, const CalibrationPoint &x)
{
    if(name >= PHYSICAL_INPUTS || i >= ANALOG_INPUTS_MAX_CALIBRATION_POINTS) return;
    eeprom::write<CalibrationPoint>(&eeprom::data.calibration[name].p[i], x);
}

uint16_t AnalogInputs::getConnectedBalancePorts()
{
    uint16_t ports = 0, port = 1;
    for(uint8_t i=0; i < MAX_BANANCE_CELLS; i++){
        if(isConnected(Name(Vb1+i))) {
            ports |= port;
        }
        port <<= 1;
    }
    return ports;
}

uint8_t AnalogInputs::getConnectedBalancePortsCount()
{
     return countBits(getConnectedBalancePorts());
}


bool AnalogInputs::isConnected(Name name)
{
    if(name == Vbalancer) {
        return getRealValue(VobInfo) == Vbalancer;
    }
    AnalogInputs::ValueType x = getRealValue(name);
    switch(getType(name)) {
    case Current:
        return x > CONNECTED_MIN_CURRENT;
    case Voltage:
        return x > CONNECTED_MIN_VOLTAGE;
    default:
        return true;
    }
}

bool AnalogInputs::isBalancePortConnected()
{
    return isConnected(Vbalancer);
}


AnalogInputs::ValueType AnalogInputs::getVbattery()
{
    return getRealValue(VoutBalancer);
}

AnalogInputs::ValueType AnalogInputs::getVout()
{
    return getRealValue(Vout);
}

AnalogInputs::ValueType AnalogInputs::getIout()
{
    return getRealValue(Iout);
}

bool AnalogInputs::isOutStable()
{
    return isStable(AnalogInputs::VoutBalancer) && isStable(AnalogInputs::Iout) && Balancer::isStable();
}

void AnalogInputs::_resetAvr()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ANALOG_INPUTS_FOR_ALL_PHY(name) {
            i_avrSum_[name] = 0;
        }
        i_avrCount_ = ANALOG_INPUTS_ADC_ROUND_MAX_COUNT;
        ignoreLastResult_ = false;
    }
}

void AnalogInputs::_resetDeltaAvr()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        i_deltaAvrCount_ = 0;
        i_deltaAvrSumVoutPlus_ = 0;
        i_deltaAvrSumVoutMinus_ = 0;
        i_deltaAvrSumTextern_ = 0;
        deltaStartTimeU16_ = Time::getMilisecondsU16();
    }
}

void AnalogInputs::resetDelta()
{
    _resetDeltaAvr();
    deltaCount_ = 0;
    deltaLastT_ = 0;
}

void AnalogInputs::resetStable()
{
    ANALOG_INPUTS_FOR_ALL(name) {
        stableCount_[name] = 0;
    }
}


void AnalogInputs::resetMeasurement()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        i_avrCount_ = 1; //TODO:??
        ignoreLastResult_ = true;
        resetStable();
    }
}

void AnalogInputs::resetAccumulatedMeasurements()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        i_charge_ = 0;
    }
    resetMeasurement();
    resetDelta();
    setReal(Cout, 0);
    setReal(deltaVout, 0);
    setReal(deltaVoutMax, 0);
    setReal(deltaTextern, 0);
}

void AnalogInputs::reset()
{
    calculationCount_ = 0;
    resetAccumulatedMeasurements();
}

void AnalogInputs::powerOn(bool enableBatteryOutput)
{
    if(!on_) {
        hardware::setBatteryOutput(enableBatteryOutput);
        reset();
        on_ = true;
        doFullMeasurement();
    }
}

void AnalogInputs::powerOff()
{
    on_ = false;
    hardware::setBatteryOutput(false);
}

bool AnalogInputs::isReversePolarity()
{
    AnalogInputs::ValueType vm = getADCValue(Vout_minus_pin);
    AnalogInputs::ValueType vp = getADCValue(Vout_plus_pin);
    if(vm > vp) vm -=  vp;
    else vm = 0;

    return vm > REVERSE_POLARITY_MIN_VOLTAGE;
}

AnalogInputs::ValueType AnalogInputs::calibrateValue(Name name, ValueType x)
{
    //TODO: do this with more points
    if (x == 0) return 0;
    CalibrationPoint p0, p1;
    getCalibrationPoint(p0, name, 0);
    getCalibrationPoint(p1, name, 1);
    int32_t y,a;
    y  = p1.y; y -= p0.y;
    a  =  x;   a -= p0.x;
    y *= a;
    a  = p1.x; a -= p0.x;
    y /= a;
    y += p0.y;

    if(y < 0) y = 0;
    return y;
}

AnalogInputs::ValueType AnalogInputs::reverseCalibrateValue(Name name, ValueType y)
{
    if (y == 0) return 0;
    //TODO: do this with more points
    CalibrationPoint p0, p1;
    getCalibrationPoint(p0, name, 0);
    getCalibrationPoint(p1, name, 1);
    int32_t x,a;
    x  = p1.x; x -= p0.x;
    a  =  y;   a -= p0.y;
    x *= a;
    a  = p1.y; a -= p0.y;
    x /= a;
    x += p0.x;

    if(x < 0) x = 0;
    return x;
}




void AnalogInputs::initialize()
{
    reset();
}

AnalogInputs::Type AnalogInputs::getType(Name name)
{
    switch(name){
    case Iout:
    case Ismps:
    case IsmpsSet:
    case Idischarge:
    case IdischargeSet:
        return Current;
    case Tintern:
    case Textern:
        return Temperature;
    case Pout:
        return Power;
    case Eout:
        return Work;
    default:
        return Voltage;
    }
}

void AnalogInputs::printRealValue(Name name, uint8_t dig)
{
    ValueType x = getRealValue(name);
    Type t = getType(name);
    lcdPrintAnalog(x, dig, t);
}

uint16_t AnalogInputs::getCharge()
{
    uint32_t retu;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        retu = i_charge_;
    }
#if TIMER_INTERRUPT_PERIOD_MICROSECONDS == 500
    retu /= 1000000/TIMER_INTERRUPT_PERIOD_MICROSECONDS/16;
    retu /= 3600/TIMER_SLOW_INTERRUPT_INTERVAL*16;
#else
#warning "TIMER_INTERRUPT_PERIOD_MICROSECONDS != 500"
    retu /= 1000000/TIMER_INTERRUPT_PERIOD_MICROSECONDS;
    retu /= 3600/TIMER_SLOW_INTERRUPT_INTERVAL;
#endif
    return retu;
}

void AnalogInputs::doSlowInterrupt()
{
    i_charge_ += getIout();
}

// finalize Measurement

void AnalogInputs::intterruptFinalizeMeasurement()
{
    if(i_avrCount_>0)
        i_avrCount_--;
}


void AnalogInputs::doIdle()
{
    finalizeFullMeasurement();
}

void AnalogInputs::setRealBasedOnAvr(AnalogInputs::Name name)
{
    avrAdc_[name] = i_avrSum_[name] / ANALOG_INPUTS_ADC_MEASUREMENTS_COUNT;
    ValueType real = calibrateValue(name, avrAdc_[name]);
    setReal(name, real);
}

void AnalogInputs::finalizeFullMeasurement()
{
    uint16_t avrCount;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        avrCount = i_avrCount_;
    }

    if(avrCount == 0) {
        if(!ignoreLastResult_) {
            if(isPowerOn()) {
                calculationCount_++;

                i_deltaAvrSumVoutPlus_    += i_avrSum_[Vout_plus_pin] >> ANALOG_INPUTS_ADC_DELTA_SHIFT;
                i_deltaAvrSumVoutMinus_   += i_avrSum_[Vout_minus_pin] >> ANALOG_INPUTS_ADC_DELTA_SHIFT;
                i_deltaAvrSumTextern_     += i_avrSum_[Textern] >> ANALOG_INPUTS_ADC_DELTA_SHIFT;
                i_deltaAvrCount_ ++;
                finalizeDeltaMeasurement();

                ANALOG_INPUTS_FOR_ALL_PHY(name) {
                    setRealBasedOnAvr(name);
                }
                finalizeFullVirtualMeasurement();
            } else {
                //we need internal temperature all the time to control the fan
                setRealBasedOnAvr(AnalogInputs::Tintern);
            }
        }
        _resetAvr();
    }
}


void AnalogInputs::finalizeDeltaMeasurement()
{
    if(Time::diffU16(deltaStartTimeU16_, Time::getMilisecondsU16()) > ANALOG_INPUTS_DELTA_TIME_MILISECONDS) {
        uint32_t deltaAvrCount;
        uint32_t deltaAvrSumVoutPlus;
        uint32_t deltaAvrSumVoutMinus;
        uint32_t deltaAvrSumTextern;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            deltaAvrCount = i_deltaAvrCount_;
            deltaAvrCount *= ANALOG_INPUTS_ADC_MEASUREMENTS_COUNT;
            deltaAvrCount >>= ANALOG_INPUTS_ADC_DELTA_SHIFT;
            deltaAvrSumVoutPlus  = i_deltaAvrSumVoutPlus_;
            deltaAvrSumVoutMinus = i_deltaAvrSumVoutMinus_;
            deltaAvrSumTextern   = i_deltaAvrSumTextern_;
        }
        _resetDeltaAvr();
        deltaCount_++;

        uint16_t x;
        ValueType real, old, VoutPlus, VoutMinus;

        //calculate deltaVout
        deltaAvrSumVoutPlus /= deltaAvrCount;
        deltaAvrSumVoutMinus /= deltaAvrCount;


        VoutPlus  = calibrateValue(Vout_plus_pin,  deltaAvrSumVoutPlus);
        VoutMinus = calibrateValue(Vout_minus_pin, deltaAvrSumVoutMinus);
        real = 0;
        if(VoutPlus > VoutMinus)
            real = VoutPlus - VoutMinus;

        old = getRealValue(deltaVoutMax);
        if(real >= old || (!enable_deltaVoutMax_)) {
            setReal(deltaVoutMax, real);
        }
        setReal(deltaVout, real - old);

        //calculate deltaTextern
        uint16_t dc = 2;
#if ANALOG_INPUTS_DELTA_TIME_MILISECONDS != 30000
#error "ANALOG_INPUTS_DELTA_TIME_MILISECONDS != 30000"
#endif
        deltaAvrSumTextern /= deltaAvrCount;
        x = deltaAvrSumTextern;
        real = calibrateValue(Textern, x);
        old = deltaLastT_;
        deltaLastT_ = real;
        real -= old;
        real *= dc;
        setReal(deltaTextern, real);
        setReal(deltaLastCount, deltaAvrCount);
    }
}

void AnalogInputs::finalizeFullVirtualMeasurement()
{
    AnalogInputs::ValueType balancer = 0;
    AnalogInputs::ValueType out_p = real_[Vout_plus_pin];
    AnalogInputs::ValueType out_m = real_[Vout_minus_pin];
    AnalogInputs::ValueType out = 0;
    if(out_m < out_p)
        out = out_p - out_m;
    setReal(Vout, out);

#ifdef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
    AnalogInputs::ValueType vb0_p = getRealValue(Vb0_pin);
    AnalogInputs::ValueType vb1_p = getRealValue(Vb1_pin);
    AnalogInputs::ValueType vb2_p = getRealValue(Vb2_pin);
    AnalogInputs::ValueType balance1 = 0, balance2 =0;
    if(vb1_p > vb0_p) balance1 = vb1_p - vb0_p;
    if(vb2_p > vb1_p) balance2 = vb2_p - vb1_p;
    setReal(Vb1, balance1);
    setReal(Vb2, balance2);
    for(uint8_t i=2; i < MAX_BANANCE_CELLS; i++) {
        setReal(Name(Vb1+i), getRealValue(Name(Vb1_pin+i)));
    }
#else
    for(uint8_t i=0; i < MAX_BANANCE_CELLS; i++) {
        setReal(Name(Vb1+i), getRealValue(Name(Vb1_pin+i)));
    }
#endif

    uint16_t connectedCells = getConnectedBalancePorts();

    for(uint8_t i = 0; i < MAX_BANANCE_CELLS; i++) {
        if(connectedCells & (1<<i))
            balancer += getRealValue(Name(Vb1+i));
    }

    setReal(Vbalancer, balancer);
    AnalogInputs::Name obInfo;
    if(balancer == 0 || absDiff(out, balancer) > ANALOG_VOLT(3.000)) {
        //balancer not connected or big error in calibration
        obInfo = Vout;
        connectedCells = 0;
    } else {
        out = balancer;
        obInfo = Vbalancer;
    }
    setReal(VoutBalancer, out);
    setReal(VbalanceInfo, connectedCells);
    setReal(VobInfo, obInfo);

    AnalogInputs::ValueType IoutValue = 0;
    AnalogInputs::ValueType CoutValue = getCharge();
    if(Discharger::isPowerOn()) {
        IoutValue = getRealValue(Idischarge);
    } else if (SMPS::isPowerOn()) {
        IoutValue = getRealValue(Ismps);
    }

    setReal(Iout, IoutValue);
    setReal(Cout, CoutValue);

    uint32_t P = IoutValue;
    P *= out;
    P /= 10000;
    setReal(Pout, P);

    //TODO: rewrite
    uint32_t E = CoutValue;
    E *= out;
    E /= 10000;
    setReal(Eout, E);
}

void AnalogInputs::setReal(Name name, ValueType real)
{
    if(absDiff(real_[name], real) > STABLE_VALUE_ERROR)
        stableCount_[name] = 0;
    else
        stableCount_[name]++;

    real_[name] = real;
}

