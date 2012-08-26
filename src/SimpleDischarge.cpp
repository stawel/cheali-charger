#include "SimpleDischarge.h"
#include "Hardware.h"
#include "ProgramData.h"

void SimpleDischarge::powerOff()
{
	discharger.powerOff();
}

void SimpleDischarge::powerOn()
{
	discharger.powerOn();
	balancer.powerOn();

	//TODO: implement discharge current
	discharger.setRealValue(ProgramData::currentProgramData.I);
}


ChargingStrategy::statusType SimpleDischarge::doStrategy()
{
	if(isMinVout()) {
		discharger.powerOff(Discharger::DISCHARGING_COMPLETE);
		return COMPLETE;
	}
	return RUNNING;
}


bool SimpleDischarge::isMinVout() const
{
	AnalogInputs::ValueType Vc = ProgramData::currentProgramData.getVoltage(ProgramData::VDischarge);
	AnalogInputs::ValueType Vc_per_cell = ProgramData::currentProgramData.getVoltagePerCell(ProgramData::VDischarge);

	return Vc >= discharger.getVout() || balancer.isMinVout(Vc_per_cell);
}
