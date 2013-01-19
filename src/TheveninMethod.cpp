#include "TheveninCharge.h"
#include "SMPS.h"
#include "Hardware.h"
#include "ProgramData.h"
#include "Screen.h"


TheveninMethod theveninMethod;


void TheveninMethod::setVI(AnalogInputs::ValueType Vend, AnalogInputs::ValueType i)
{
    Vend_ = Vend;
    maxValue_ = i;
    minValue_ = i/10;
}

void TheveninMethod::init()
{
    AnalogInputs::ValueType Vout = getVout();
    tVout_.init(Vout, Vend_, minValue_);

    cells_ = balancer.getCells();
    AnalogInputs::ValueType Vend_per_cell = balancer.calculatePerCell(Vend_);

    for(uint8_t c = 0; c < cells_; c++) {
        AnalogInputs::ValueType v = balancer.getPresumedV(c);
        tBal_[c].init(v, Vend_per_cell, minValue_);
    }

    Ifalling_ = NotFalling;
    fullCount_ = 0;
}


bool TheveninMethod::isComlete(bool isEndVout, AnalogInputs::ValueType value)
{
    if(value <= minValue_ && isEndVout) {
        if(fullCount_++ >= 10) {
            return true;
        }
    } else {
        fullCount_ = 0;
    }
    return false;
}


AnalogInputs::ValueType TheveninMethod::calculateNewValue(bool isEndVout, AnalogInputs::ValueType oldValue)
{
    AnalogInputs::ValueType i;

    //test for maximum output voltage reached
    if(isEndVout) {
        switch(Ifalling_) {
        case NotFalling:
            Ifalling_ = LastRthMesurment;
            //temporarily turn off
            storeOldValue(oldValue);
            return 0;
        default:
            Ifalling_ = Falling;
            break;
        }
    }

    calculateRthVth(oldValue);
    i = calculateI();
    return normalizeI(i, oldValue);
}


void TheveninMethod::calculateRthVth(AnalogInputs::ValueType oldValue)
{
    tVout_.calculateRthVth(getVout(),oldValue);

    for(uint8_t c = 0; c < cells_; c++) {
        tBal_[c].calculateRthVth(balancer.getPresumedV(c),oldValue);
    }
}

AnalogInputs::ValueType TheveninMethod::calculateI()
{
    AnalogInputs::ValueType i = tVout_.calculateI(Vend_);
    AnalogInputs::ValueType Vend_per_cell = balancer.calculatePerCell(Vend_);
    for(uint8_t c = 0; c < cells_; c++) {
        i = min(i, tBal_[c].calculateI(Vend_per_cell));
    }
    return i;
}

AnalogInputs::ValueType TheveninMethod::normalizeI(AnalogInputs::ValueType value, AnalogInputs::ValueType oldValue)
{
    valueTh_ = value;
    if(value > maxValue_) {
        value = maxValue_;
    }
    if(value < minValue_) {
        value = minValue_;
    }

    if((oldValue != value && Ifalling_ != Falling) ||
            oldValue > value) {

        storeOldValue(oldValue);
        return value;
    }
    return oldValue;
}

AnalogInputs::ValueType TheveninMethod::storeOldValue(AnalogInputs::ValueType oldValue)
{
    tVout_.storeLast(getVout(), oldValue);

    for(uint8_t i = 0; i < cells_; i++) {
        AnalogInputs::ValueType vi = balancer.getPresumedV(i);
        tBal_[i].storeLast(vi, oldValue);
    }
}
