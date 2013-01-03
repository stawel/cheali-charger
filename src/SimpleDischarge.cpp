#include "SimpleDischarge.h"
#include "Hardware.h"
#include "ProgramData.h"


SimpleDischarge simpleDischarge;

void SimpleDischarge::powerOff()
{
    balancer.powerOff();
    discharger.powerOff();
}

void SimpleDischarge::powerOn()
{
    discharger.powerOn();
    balancer.powerOn();

    discharger.setRealValue(I_);
}


Strategy::statusType SimpleDischarge::doStrategy()
{
    if(isMinVout()) {
        discharger.powerOff(Discharger::DISCHARGING_COMPLETE);
        return COMPLETE;
    }
    return RUNNING;
}


bool SimpleDischarge::isMinVout() const
{
    AnalogInputs::ValueType Vc = V_;
    AnalogInputs::ValueType Vc_per_cell = balancer.calculatePerCell(Vc);

    return Vc >= discharger.getVout() || balancer.isMinVout(Vc_per_cell);
}


bool SimpleDischarge::isStable() const
{
    return analogInputs.isStable(discharger.VName) && analogInputs.isStable(discharger.IName) && balancer.isStable();
}
