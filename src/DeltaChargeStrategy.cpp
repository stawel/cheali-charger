#include "DeltaChargeStrategy.h"
#include "Hardware.h"
#include "ProgramData.h"
#include "Screen.h"
#include "TheveninMethod.h"

DeltaChargeStrategy deltaChargeStrategy;

void DeltaChargeStrategy::powerOn()
{
    state_ = PreCharge;
    SimpleCharge::powerOn();
}


Strategy::statusType DeltaChargeStrategy::doStrategy()
{
    calculateThevenin();
    AnalogInputs::ValueType Vout = smps.getVout();

    if(state_ == PreCharge) {
        if(Vout > ProgramData::currentProgramData.getVoltage(ProgramData::VDischarge)) {
            state_ = RapidCharge;
            smps.setRealValue(ProgramData::currentProgramData.battery.Ic);
        }
    }

    if(isStable() && Vout > ProgramData::currentProgramData.getVoltage(ProgramData::VUpperLimit)) {
        Program::stopReason_ = PSTR("V limit");
        return COMPLETE;
    }

    if(analogInputs.deltaCount_ <= 1)
        return RUNNING;

    if(testDeltaV_) {
        int x = analogInputs.getRealValue(AnalogInputs::deltaVout);
        x=-x;
        if(x > ProgramData::currentProgramData.getDeltaVLimit()) {
            Program::stopReason_ = PSTR("-dV");
            return COMPLETE;
        }
    }
    if(testDeltaT_) {
        int x = analogInputs.getRealValue(AnalogInputs::deltaTextern);
        if(x > ProgramData::currentProgramData.getDeltaTLimit()) {
            Program::stopReason_ = PSTR("dT/dt");
            return COMPLETE;
        }
    }

    return RUNNING;
}


