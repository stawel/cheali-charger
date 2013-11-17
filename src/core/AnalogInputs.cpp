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
#include "Hardware.h"
#include "AnalogInputs.h"
#include "memory.h"
#include "LcdPrint.h"
#include "SerialLog.h"
#include "eeprom.h"

//TODO_NJ for testing
//#include "Buzzer.h"


namespace AnalogInputs {

    bool on_;
    uint16_t avrCount_;
    uint32_t avrSum_[PHYSICAL_INPUTS];
    ValueType avrAdc_[PHYSICAL_INPUTS];
    ValueType adc_[PHYSICAL_INPUTS];
    ValueType real_[ALL_INPUTS];
    uint16_t stableCount_[ALL_INPUTS];

    uint16_t calculationCount_;

    uint16_t    deltaCount_;
    uint16_t    deltaAvrCount_;
    uint32_t    deltaAvrSumVout_;
    uint32_t    deltaAvrSumTextern_;
    ValueType   deltaLastT_;
    uint32_t    deltaStartTime_;

    uint32_t    charge_;
    
    ValueType getAvrADCValue(Name name)     { return avrAdc_[name]; }
    ValueType getRealValue(Name name)       { return real_[name]; }
    ValueType getADCValue(Name name)        { return adc_[name]; }
    bool isPowerOn() { return on_; }
    uint16_t getFullMeasurementCount()  { return calculationCount_; }
    uint16_t getStableCount(Name name)   { return stableCount_[name]; };
    bool isStable(Name name)     { return stableCount_[name] >= STABLE_MIN_VALUE; };
    void setReal(Name name, ValueType real);
    
    //TODO_NJ for smooth-current
    uint16_t smoothCounter;
    uint16_t smoothCurrentOldValue;
    
    
    

} // namespace AnalogInputs


void AnalogInputs::restoreDefault()
{
    CalibrationPoint p;
    FOR_ALL_PHY_INPUTS(name) {
        p = pgm::read<CalibrationPoint>(&inputsP_[name].p0);
        setCalibrationPoint(name, 0, p);
        p = pgm::read<CalibrationPoint>(&inputsP_[name].p1);
        setCalibrationPoint(name, 1, p);
    }
}

void AnalogInputs::getCalibrationPoint(CalibrationPoint &x, Name name, uint8_t i)
{
    if(name >= PHYSICAL_INPUTS || i >= MAX_CALIBRATION_POINTS) {
        x.x = x.y = 1;
        return;
    }
    eeprom::read<CalibrationPoint>(x,&eeprom::data.calibration[name].p[i]);
}
void AnalogInputs::setCalibrationPoint(Name name, uint8_t i, const CalibrationPoint &x)
{
    if(name >= PHYSICAL_INPUTS || i >= MAX_CALIBRATION_POINTS) return;
    eeprom::write<CalibrationPoint>(&eeprom::data.calibration[name].p[i], x);
}

uint8_t AnalogInputs::getConnectedBalancePorts()
{
    for(uint8_t i=0; i < 6; i++){
        if(!isConnected(Name(Vb1+i))) return i;
    }
    return 6;
}
bool AnalogInputs::isConnected(Name name)
{
    AnalogInputs::ValueType x = getRealValue(name);
    switch(getType(name)) {
    case Current:
        return x > ANALOG_AMP(0.050);
    case Voltage:
        return x > ANALOG_VOLT(1);
    default:
        return true;
    }
}

void AnalogInputs::finalizeDeltaMeasurement()
{
    bool useVBalancer = real_[VobInfo] == Vbalancer;
    if(useVBalancer) {
        //when the balancer is connected use
        //its "real" voltage to calculate deltaVout
        deltaAvrSumVout_ += real_[VoutBalancer];
    } else {
        deltaAvrSumVout_ += adc_[Vout];
    }
    deltaAvrSumTextern_ += adc_[Textern];
    deltaAvrCount_++;
    if(Timer::getMiliseconds() - deltaStartTime_ > DELTA_TIME_MILISECONDS) {
        deltaCount_++;

        uint16_t x;
        ValueType real, old;

        //calculate deltaVout
        deltaAvrSumVout_ /= deltaAvrCount_;
        x = deltaAvrSumVout_;
        deltaAvrSumVout_ = 0;
        if(useVBalancer) {
            //we don't need to calibrate a "real" value
            real = x;
        } else {
            real = calibrateValue(Vout, x);
        }
        old = getRealValue(deltaVoutMax);
        if(real >= old)
            setReal(deltaVoutMax, real);
        setReal(deltaVout, real - old);

        //calculate deltaTextern
        uint16_t dc = deltaAvrCount_;
#if DELTA_TIME_MILISECONDS != 60000
#warning "DELTA_TIME_MILISECONDS != 60000"
        uint32_t dx2;
        dx2 = dc;
        dx2 /= 60000;
        dx2 *= DELTA_TIME_MILISECONDS;
        dc = dx2;
#endif
        deltaAvrSumTextern_ /= dc;
        x = deltaAvrSumTextern_;
        deltaAvrSumTextern_ = 0;
        real = calibrateValue(Textern, x);
        old = deltaLastT_;
        deltaLastT_ = real;
        real -= old;
        setReal(deltaTextern, real);

        setReal(deltaLastCount, deltaAvrCount_);
        deltaAvrCount_ = 0;
        deltaStartTime_ = Timer::getMiliseconds();
    }
}

void AnalogInputs::finalizeFullVirtualMeasurement()
{
    AnalogInputs::ValueType oneVolt = ANALOG_VOLT(1);
    AnalogInputs::ValueType balancer = 0;
    AnalogInputs::ValueType out = real_[Vout];

#ifdef ENABLE_SIMPLIFIED_VB0_VB2_CIRCUIT
    //when balance port is not connected then
    //it may happen that Vb0_pin > Vb1_pin or Vb1_pin > Vb2_pin
    //that's why we use "absDiff"
    setReal(Vb1, absDiff(getRealValue(Vb1_pin), getRealValue(Vb0_pin)));
    setReal(Vb2, absDiff(getRealValue(Vb2_pin), getRealValue(Vb1_pin)));
    for(uint8_t i=2; i < 6; i++) {
        setReal(Name(Vb1+i), getRealValue(Name(Vb1_pin+i)));
    }
#else
    for(uint8_t i=0; i < 6; i++) {
        setReal(Name(Vb1+i), getRealValue(Name(Vb1_pin+i)));
    }
#endif

    uint8_t ports = getConnectedBalancePorts();

    for(uint8_t i=0; i < ports; i++) {
        balancer += getRealValue(Name(Vb1+i));
    }

    setReal(Vbalancer, balancer);
    AnalogInputs::Name obInfo;
    if(balancer == 0 || (out > balancer && out - balancer > oneVolt)) {
        //balancer not connected or big error in calibration
        obInfo = Vout;
        ports = 0;
    } else {
        out = balancer;
        obInfo = Vbalancer;
    }
    setReal(VoutBalancer, out);
    setReal(VbalanceInfo, ports);
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

void AnalogInputs::doSlowInterrupt()
{
    charge_ += getIout();
}

uint16_t AnalogInputs::getCharge()
{
    uint32_t retu = charge_;
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


AnalogInputs::ValueType AnalogInputs::getVout()
{
    return getRealValue(VoutBalancer);
}

AnalogInputs::ValueType AnalogInputs::getIout()
{
    return getRealValue(Iout);
}

bool AnalogInputs::isOutStable()
{
    return isStable(AnalogInputs::VoutBalancer) && isStable(AnalogInputs::Iout) && Balancer::isStable();
}


void AnalogInputs::finalizeFullMeasurement()
{
    calculationCount_++;
    FOR_ALL_PHY_INPUTS(name) {
        avrAdc_[name] = avrSum_[name] / avrCount_;
        ValueType real = calibrateValue(name, avrAdc_[name]);
        setReal(name, real);
    }
    finalizeFullVirtualMeasurement();
#ifdef ENABLE_SERIAL_LOG
    SerialLog::send();
#endif //ENABLE_SERIAL_LOG
    clearAvr();
}

void AnalogInputs::setReal(Name name, ValueType real)
{
    if(absDiff(real_[name], real) > STABLE_VALUE_ERROR)
        stableCount_[name] = 0;
    else
        stableCount_[name]++;

    real_[name] = real;
}

void AnalogInputs::clearAvr()
{
    avrCount_ = 0;
    FOR_ALL_PHY_INPUTS(name) {
        avrSum_[name] = 0;
    }
}

void AnalogInputs::resetDelta()
{
    deltaAvrCount_ = 0;
    deltaAvrSumVout_ = 0;
    deltaAvrSumTextern_ = 0;
    deltaCount_ = 0;
    deltaLastT_ = 0;
    deltaStartTime_ = Timer::getMiliseconds();
}


void AnalogInputs::resetStable()
{
    FOR_ALL_INPUTS(name) {
        stableCount_[name] = 0;
    }
}


void AnalogInputs::resetMeasurement()
{
    clearAvr();
    resetStable();
}

void AnalogInputs::reset()
{

    calculationCount_ = 0;
    charge_ = 0;
    resetADC();
    resetMeasurement();
    resetDelta();
    FOR_ALL_INPUTS(name){
        real_[name] = 0;
    }
}

void AnalogInputs::powerOn()
{
    if(!on_) {
        hardware::setBatteryOutput(true);
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
    AnalogInputs::ValueType vr = getADCValue(VreversePolarity);
    AnalogInputs::ValueType vo = getADCValue(Vout);
    if(vr > vo) vr -=  vo;
    else vr = 0;

    return vr > REVERSE_POLARITY_MIN_VALUE;
}

void AnalogInputs::finalizeMeasurement()
{
    FOR_ALL_PHY_INPUTS(name) {
        avrSum_[name] += adc_[name];
    }
    avrCount_++;
    finalizeDeltaMeasurement();
    if(avrCount_ == AVR_MAX_COUNT) {
        finalizeFullMeasurement();
    }
}

AnalogInputs::ValueType AnalogInputs::calibrateValue(Name name, ValueType x)
{
    //TODO: do it with more points
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
    //TODO: do it with more points
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
    case VirtualInputs:
        return Unknown;
    case Iout:
    case Ismps:
    case IsmpsValue:
    case Idischarge:
    case IdischargeValue:
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
    lcdPrintAnalog(x, t, dig);
}






/*
void AnalogInputs::checkMaxPower()
{
//TODO_NJ
//outer calculation. part of slowinterrupt.

//disabled because causes other charging calculations
return;

uint16_t checkOverDriveValue = SMPS::getValue();;
uint16_t AbsMaximalValue = AnalogInputs::checkMaxPowerCvalue(checkOverDriveValue);

  if (checkOverDriveValue > AbsMaximalValue)
  {
     
     SMPS::setValue1(AbsMaximalValue);
     
     //TODO_NJ (testing for smoothcurrent)
     Buzzer::soundKeyboard();   //tick if outer routine limitly actual current.
     //smoothCounter=smoothCounter+1;       //smoothCurrentOldValue();  //not tested
     
     //SerialLog::debugSerial(checkOverDriveValue,smoothCounter);   //never use (long)
  
  
  
  
  
  
  
     
  }




}


uint16_t AnalogInputs::checkMaxPowerCvalue(uint16_t value)
{
  return value;
//disabled


//SerialLog::debugSerial(value,12345);
//return value;
//inner directly control. Call this routine always  before  call SMPS::setvalue()
//NJ




    uint16_t valueTemp1;
    
    if (value == 0) return value; 
    
    valueTemp1 = AnalogInputs::reverseCalibrateValue(AnalogInputs::IsmpsValue, AnalogInputs::maxIc1());
    
    if (valueTemp1 < value)
    {
      //TODO_NJ for testing
      //Buzzer::soundSelect();  //testing
      return valueTemp1;
    }
    return value;
   
}

uint16_t AnalogInputs::checkMaxPowerDvalue(uint16_t value)
{
return value;
//disabled

//inner directly control. Call this routine always before call Discharger::setvalue()
//NJ 



    uint16_t valueTemp1;

    if (value == 0) return value;
    
    valueTemp1 = AnalogInputs::reverseCalibrateValue(AnalogInputs::IdischargeValue, AnalogInputs::maxId1());
    if (valueTemp1 < value) return valueTemp1;
    
    return value;

}

uint16_t AnalogInputs::maxIc1()
{


//disabled



    // max Ic calc dependent actual voltage
    uint32_t i;
    uint16_t v;
    v = getRealValue(Vout);
    if (v == 0) return 0;          //protect division by zero
    i = MAX_CHARGE_P;
    i *= ANALOG_VOLT(1);
    i /= v;                 

    if(i > MAX_CHARGE_I)  
        i = MAX_CHARGE_I; 
    return i;

}

uint16_t AnalogInputs::maxId1()
{
//disabled


    // max Ic calc dependent actual voltage
    uint32_t i;
    uint16_t v;
    v = getRealValue(Vout);
    if (v == 0) return 0;      //protect division by zero
    i = MAX_DISCHARGE_P;
    i *= ANALOG_VOLT(1);
    i /= v;                 

    if(i > MAX_DISCHARGE_I)  
        i = MAX_DISCHARGE_I; 
    return i;

}

*/
