#include "Hardware.h"
#include "StartInfoStrategy.h"


StartInfoStrategy startInfoStrategy;

void StartInfoStrategy::powerOn()
{
	hardware::setBatteryOutput(true);
}

void StartInfoStrategy::powerOff()
{
	hardware::setBatteryOutput(false);
}

Strategy::statusType StartInfoStrategy::doStrategy()
{
	if(keyboard.getPressed() == BUTTON_START)
		return Strategy::COMPLETE_AND_EXIT;
	return Strategy::RUNNING;
}
