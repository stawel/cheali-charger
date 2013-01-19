#include "SimpleCharge.h"
#include "ProgramData.h"
#include "Screen.h"
#include "Hardware.h"
#include "TheveninMethod.h"


SimpleCharge simpleCharge;


void SimpleCharge::powerOn()
{
    smps.powerOn();
    AnalogInputs::ValueType I = ProgramData::currentProgramData.battery.Ic;
    AnalogInputs::ValueType Vc = ProgramData::currentProgramData.getVoltage(ProgramData::VCharge);
    I/=5; //start charging with 0.2CmAh
    uint16_t value = analogInputs.reverseCalibrateValue(AnalogInputs::IsmpsValue, I);
    theveninMethod.setVI(Vc, value);
    theveninMethod.init();
    smps.setValue(value);
    Program::iName_ = AnalogInputs::IsmpsValue;
}

void SimpleCharge::powerOff()
{
    smps.powerOff();
}

void SimpleCharge::calculateThevenin() const
{
    if(isStable()) theveninMethod.calculateRthVth(smps.getValue());
}

/*
Strategy::statusType SimpleCharge::doStrategy()
{
    calculateThevenin();
    smps.setRealValue(ProgramData::currentProgramData.battery.Ic);
    if(smps.getIcharge() >= ProgramData::currentProgramData.battery.Ic) {
        smps.powerOff(SMPS::ERROR);
        return ERROR;
    }

    if(testVout_ && smps.getVout() >= ProgramData::currentProgramData.getVoltage(ProgramData::VCharge)) {
        smps.powerOff(SMPS::CHARGING_COMPLETE);
        return COMPLETE;
    }

    if(smps.getCharge() > ProgramData::currentProgramData.battery.C) {
        smps.powerOff(SMPS::CHARGING_COMPLETE);
        return COMPLETE;
    }
    return RUNNING;
}
*/

bool SimpleCharge::isStable() const
{
    return analogInputs.isStable(smps.VName) && analogInputs.isStable(smps.IName) && balancer.isStable();
}


