#include "SimpleCharge.h"
#include "ProgramData.h"
#include "Screen.h"
#include "Hardware.h"


void SimpleCharge::powerOn(SMPS &s)
{
	s.setRealValue(ProgramData::currentProgramData.I);
}

void SimpleCharge::doStrategy(SMPS &s)
{
	if(s.getIout() 	>= ProgramData::currentProgramData.I) {
		s.setError(PSTR("Error: Maximum"),PSTR("current exceeded"));
		return;
	}
	if(s.getVout() 	>= ProgramData::currentProgramData.getVoltage(ProgramData::VCharge)) {
		s.powerOff(SMPS::CHARGING_COMPLETE);
		return;
	}
	if(s.getCharge() > ProgramData::currentProgramData.C) {
		s.powerOff(SMPS::CHARGING_COMPLETE);
		return;
	}
}


bool SimpleCharge::isStable(SMPS &s)
{
	return analogInputs.isStable(s.Vm_) && analogInputs.isStable(s.Im_);
}


