#include "ChargingStrategy.h"
#include "SMPS.h"

void ChargingStrategy::doStrategy(SMPS &s)
{
	s.powerOff(SMPS::CHARGING_COMPLETE);
}

