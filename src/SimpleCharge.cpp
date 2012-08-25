#include "SimpleCharge.h"
#include "ProgramData.h"
#include "Screen.h"
#include "Hardware.h"



void SimpleCharge::powerOn()
{
	smps.powerOn();
	smps.setRealValue(ProgramData::currentProgramData.I);
}

void SimpleCharge::powerOff()
{
	smps.powerOff();
}

ChargingStrategy::statusType SimpleCharge::doStrategy()
{
	if(smps.getIout() 	>= ProgramData::currentProgramData.I) {
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


bool SimpleCharge::isStable()
{
	return analogInputs.isStable(smps.Vm_) && analogInputs.isStable(smps.Im_);
}


