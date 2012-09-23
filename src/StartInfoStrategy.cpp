#include "Hardware.h"
#include "StartInfoStrategy.h"
#include "ProgramData.h"
#include "Screen.h"


StartInfoStrategy startInfoStrategy;

void StartInfoStrategy::powerOn()
{
	hardware::setBatteryOutput(true);
	screens.startBlinkOn(7);
}

void StartInfoStrategy::powerOff()
{
	hardware::setBatteryOutput(false);
	screens.stopBlink();
}

Strategy::statusType StartInfoStrategy::doStrategy()
{
	bool c,b,v;
	uint8_t is_cells, should_be_cells;
	is_cells = analogInputs.getConnectedBalancePorts();
	should_be_cells = ProgramData::currentProgramData.cells;
	c = (should_be_cells != is_cells);
	b = (is_cells == 0);
	v = (analogInputs.getRealValue(AnalogInputs::Vout) < should_be_cells * ANALOG_VOLTS(1));

	screens.blinkIndex_ = 7;
	if(c) 	screens.blinkIndex_ -= 4;
	if(b) 	screens.blinkIndex_ -= 2;
	if(v) 	screens.blinkIndex_ -= 1;

	if(!c && !b && !v && keyboard.getPressed() == BUTTON_START)
		return Strategy::COMPLETE_AND_EXIT;
	return Strategy::RUNNING;
}
