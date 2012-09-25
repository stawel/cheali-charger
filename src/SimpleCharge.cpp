#include "SimpleCharge.h"
#include "ProgramData.h"
#include "Screen.h"
#include "Hardware.h"



void SimpleCharge::powerOn()
{
	smps.powerOn();
	smps.setRealValue(ProgramData::currentProgramData.Ic);
}

void SimpleCharge::powerOff()
{
	smps.powerOff();
}

Strategy::statusType SimpleCharge::doStrategy()
{
	if(smps.getIout() 	>= ProgramData::currentProgramData.Ic) {
		smps.setError(PSTR("Error: Maximum"),PSTR("current exceeded"));
		return ERROR;
	}
	if(smps.getVout() 	>= ProgramData::currentProgramData.getVoltage(ProgramData::VCharge)) {
		smps.powerOff(SMPS::CHARGING_COMPLETE);
		return COMPLETE;
	}
	if(smps.getCharge() > ProgramData::currentProgramData.C) {
		smps.powerOff(SMPS::CHARGING_COMPLETE);
		return COMPLETE;
	}
}


bool SimpleCharge::isStable() const
{
	return analogInputs.isStable(smps.VName) && analogInputs.isStable(smps.IName) && balancer.isStable();
}


