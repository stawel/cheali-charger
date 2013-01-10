#include "DeltaChargeStrategy.h"
#include "Hardware.h"
#include "ProgramData.h"
#include "Screen.h"
#include "TheveninMethod.h"

DeltaChargeStrategy deltaChargeStrategy;

Strategy::statusType DeltaChargeStrategy::doStrategy()
{
    calculateThevenin();

    if(testDeltaV_ && analogInputs.deltaCount_ > 1) {
        int x = analogInputs.getRealValue(AnalogInputs::deltaVout);
        x=-x;
        if(x > ProgramData::currentProgramData.getDeltaVLimit()) {
            screen.reason_ = 'V';
            return COMPLETE;
        }
    }
    if(testDeltaT_ &&  analogInputs.deltaCount_ > 1) {
        int x = analogInputs.getRealValue(AnalogInputs::deltaTextern);
        if(x > ProgramData::currentProgramData.getDeltaTLimit()) {
            screen.reason_ = 'T';
            return COMPLETE;
        }
    }

    return RUNNING;
}


