#include "Hardware.h"
#include "AnalogInputs.h"
#include "memory.h"
#include "LcdPrint.h"


AnalogInputs::Calibration calibration[AnalogInputs::PHYSICAL_INPUTS] EEMEM;


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
    eeprom::read<CalibrationPoint>(x,&calibration[name].p[i]);
}
void AnalogInputs::setCalibrationPoint(Name name, uint8_t i, const CalibrationPoint &x)
{
    if(name >= PHYSICAL_INPUTS || i >= MAX_CALIBRATION_POINTS) return;
    eeprom::write<CalibrationPoint>(&calibration[name].p[i], x);
}


void AnalogInputs::measureValue(Name name)
{
    if(name >=  PHYSICAL_INPUTS)
        return;
    MeasureFunction f = pgm::read<MeasureFunction>(&(inputsP_[name].f));
    measured_[name] = f();
}

void AnalogInputs::doFullMeasurement()
{
    clearAvr();
    for(uint16_t i=0; i < AVR_MAX_COUNT; i++)
        doMeasurement();
}

void AnalogInputs::clearAvr()
{
    avrCount_ = 0;
    currentInput_ = Name(0);
    FOR_ALL_PHY_INPUTS(name) {
        avrSum_[name] = 0;
    }
}

int AnalogInputs::getConnectedBalancePorts() const
{
    for(int i=0; i < 6; i++){
        if(!isConnected(Name(Vb0+i))) return i;
    }
    return 6;
}
bool AnalogInputs::isConnected(Name name) const
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

void AnalogInputs::resetDelta()
{
    deltaAvrCount_ = 0;
    deltaAvrSumVout_ = 0;
    deltaAvrSumTextern_ = 0;
    deltaCount_ = 0;
    deltaLastT_ = 0;
    deltaStartTime_ = timer.getMiliseconds();
}


void AnalogInputs::doDeltaCalculations()
{
    deltaAvrSumVout_ += measured_[Vout];
    deltaAvrSumTextern_ += measured_[Textern];
    deltaAvrCount_++;
    if(timer.getMiliseconds() - deltaStartTime_ > DELTA_TIME_MILISECONDS) {
        deltaCount_++;

        uint16_t x;
        ValueType real, old;

        //calculate deltaVout
        deltaAvrSumVout_ /= deltaAvrCount_;
        x = deltaAvrSumVout_;
        deltaAvrSumVout_ = 0;
        real = calibrateValue(Vout, x);
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
        deltaStartTime_ = timer.getMiliseconds();
    }
}

void AnalogInputs::doVirtualCalculations()
{
    int ports = getConnectedBalancePorts();
    AnalogInputs::ValueType oneVolt = ANALOG_VOLT(1);
    AnalogInputs::ValueType balancer = 0;
    AnalogInputs::ValueType out = real_[Vout];

    for(int i=0; i < ports; i++) {
        balancer += real_[Name(Vb0+i)];
    }

    setReal(Vbalacer, balancer);
    if(balancer == 0 || (out > balancer && out - balancer > oneVolt)) {
        //balancer not connected or big error in calibration
        setReal(VoutBalancer, out);
        setReal(VobInfo, Vout);
        setReal(VbalanceInfo, 0);
    } else {
        setReal(VoutBalancer, balancer);
        setReal(VobInfo, Vbalacer);
        setReal(VbalanceInfo, ports);
    }
    doDeltaCalculations();
}

void AnalogInputs::doCalculations()
{
    calculationCount_++;
    FOR_ALL_PHY_INPUTS(name) {
        x_[name] = avrSum_[name] / avrCount_;
        ValueType real = calibrateValue(name, x_[name]);
        setReal(name, real);
    }
    doVirtualCalculations();
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
    FOR_ALL_INPUTS(name) {
        setReal(name, 0);
    }
    resetMeasurement();
    resetDelta();
}




void AnalogInputs::doMeasurement(uint16_t count)
{
    while (count) {
        count--;
        measureValue(currentInput_);
        avrSum_[currentInput_] += measured_[currentInput_];
        currentInput_ = Name(currentInput_ + 1);
        if(currentInput_ == VirtualInputs) {
            currentInput_ = Name(0);
            avrCount_++;
            doDeltaCalculations();
            if(avrCount_ == AVR_MAX_COUNT) {
                doCalculations();
            }
        }
    }
}

AnalogInputs::ValueType AnalogInputs::getValue(Name name) const
{
    if(name >=  PHYSICAL_INPUTS)
        return 0;
    return x_[name];
}
AnalogInputs::ValueType AnalogInputs::getRealValue(Name name) const
{
    return real_[name];
}
AnalogInputs::ValueType AnalogInputs::getMeasuredValue(Name name) const
{
    if(name >=  PHYSICAL_INPUTS)
        return 0;
    return measured_[name];
}


AnalogInputs::ValueType AnalogInputs::calibrateValue(Name name, ValueType x) const
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
AnalogInputs::ValueType AnalogInputs::reverseCalibrateValue(Name name, ValueType y) const
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




AnalogInputs::AnalogInputs(const DefaultValues * inputs_P): inputsP_(inputs_P), avrCount_(0)
{
    calculationCount_ = 0;
    FOR_ALL_PHY_INPUTS(name) {
        measured_[name] = 0;
        avrSum_[name] = 0;
        x_[name] = 0;
        real_[name] = 0;
        stableCount_[name] = 0;
    }
    clearAvr();
}

AnalogInputs::Type AnalogInputs::getType(Name name)
{
    switch(name){
    case VirtualInputs:
        return Unknown;
    case Ismps:
    case IsmpsValue:
    case Idischarge:
    case IdischargeValue:
        return Current;
    case Tintern:
    case Textern:
        return Temperature;
    default:
        return Voltage;
    }
}

void AnalogInputs::printRealValue(Name name, uint8_t dig) const
{
    ValueType x = analogInputs.getRealValue(name);
    Type t = getType(name);
    lcdPrintAnalog(x, t, dig);
}
void AnalogInputs::printMeasuredValue(Name name, uint8_t dig) const
{
    ValueType x = calibrateValue(name, analogInputs.getMeasuredValue(name));
    Type t = getType(name);
    lcdPrintAnalog(x, t, dig);
}
