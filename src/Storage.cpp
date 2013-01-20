#include "Hardware.h"
#include "Storage.h"
#include "TheveninCharge.h"
#include "TheveninDischarge.h"

Storage storage;

void Storage::powerOff()
{
    theveninCharge.powerOff();
    theveninDischarge.powerOff();
    balancer.powerOff();
}

void Storage::powerOn()
{
    if(balancer.isMinVout(balancer.calculatePerCell(V_))) {
        theveninCharge.powerOn();
        state = Charge;
    } else {
        theveninDischarge.powerOn();
        state = Discharge;
    }
    balancer.powerOn();
}


Strategy::statusType Storage::doStrategy()
{
    Strategy::statusType status;
    switch(state) {
        case Charge:
            status = theveninCharge.doStrategy();
            break;
        case Discharge:
            status = theveninDischarge.doStrategy();
            break;
        case Balance:
            status = balancer.doStrategy();
            if(status != RUNNING) {
                powerOff();
                return status;
            }
            break;
    }

    if(status == COMPLETE && doBalance_) {
        status = RUNNING;
        state = Balance;
    }

    return status;
}

void Storage::setVII(AnalogInputs::ValueType V, AnalogInputs::ValueType Ic, AnalogInputs::ValueType Id)
{
    V_ = V;
    theveninCharge.setVI(V, Ic);
    theveninDischarge.setVI(V, Id);
}

void Storage::setDoBalance(bool v)
{
    doBalance_ = v;
}
