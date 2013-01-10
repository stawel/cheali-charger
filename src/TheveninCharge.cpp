#include "TheveninCharge.h"
#include "SMPS.h"
#include "Hardware.h"
#include "ProgramData.h"
#include "Screen.h"
#include "TheveninMethod.h"

TheveninCharge theveninCharge;

void TheveninCharge::powerOff()
{
    smps.powerOff();
}


void TheveninCharge::powerOn()
{
    smps.powerOn();
    balancer.powerOn();
    theveninMethod.init();
    screen.iName_ = AnalogInputs::IsmpsValue;
}

void TheveninCharge::setVI(AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
       theveninMethod.setVI(v, analogInputs.reverseCalibrateValue(AnalogInputs::IsmpsValue, i));
}
void TheveninCharge::setMinI(AnalogInputs::ValueType i)
{
       theveninMethod.setMinI(analogInputs.reverseCalibrateValue(AnalogInputs::IsmpsValue, i));
}

Strategy::statusType TheveninCharge::doStrategy()
{
    bool stable;
    bool isendVout = isEndVout();
    uint16_t oldValue = smps.getValue();

    stable = isStable() || isendVout;
    //test for charge complete
    if(theveninMethod.isComlete(isendVout, oldValue)) {
        smps.powerOff(SMPS::CHARGING_COMPLETE);
        return COMPLETE;
    }

    if(stable) {
        uint16_t value = theveninMethod.calculateNewValue(isendVout, oldValue);
        if(value != oldValue)
            smps.setValue(value);
    }
    return RUNNING;
}


bool TheveninCharge::isEndVout() const
{
    AnalogInputs::ValueType Vc = theveninMethod.Vend_;
    AnalogInputs::ValueType Vc_per_cell = balancer.calculatePerCell(Vc);

    return Vc <= smps.getVout() || balancer.isMaxVout(Vc_per_cell);
}


