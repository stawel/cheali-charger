#include "DeltaChargeStrategy.h"
#include "Hardware.h"
#include "ProgramData.h"

DeltaChargeStrategy deltaChargeStrategy;

Strategy::statusType DeltaChargeStrategy::doStrategy()
{
    if(testDeltaV_ && analogInputs.deltaCount_ > 1) {
        int x = analogInputs.getRealValue(AnalogInputs::deltaVout);
        x=-x;
        if(x > ProgramData::currentProgramData.getDeltaVLimit())
            return COMPLETE;
    }
    if(testDeltaT_ &&  analogInputs.deltaCount_ > 2 &&
        analogInputs.getRealValue(AnalogInputs::deltaTextern) > ProgramData::currentProgramData.getDeltaTLimit()) {
        return COMPLETE;
    }

    return RUNNING;
}


