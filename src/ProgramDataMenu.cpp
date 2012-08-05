#include "ProgramDataMenu.h"
#include "LcdPrint.h"
#include "GTPowerA610.h"

#define BLINK(command) do { if(blink!=index) {command; } else {} }while(false)

bool ProgramDataMenu::getBlink() const
{
	if(blinkIndex_ >= 0) {
		uint16_t mili = timer.getMiliseconds() - blinkStartTime_;
		mili/=BLINK_SPEED_MS/4;
		if(!(blinkIndex_ & BLINK_SPEED2))
			mili/=4;
		if((mili+blinkStartOff_)%2) return true;
	}
	return false;
}

int ProgramDataMenu::getBlinkIndex()
{
	if(getBlink()) {
		blinkOff_ = true;
		return blinkIndex_;
	}
	blinkOff_ = false;
	return -1;
}


#define SAVE_POS 8
void ProgramDataMenu::printItem(int index)
{
	int blink = getBlinkIndex();
	switch(index) {
	case 0:	lcdPrint_P(PSTR("Bat:  "));     BLINK(p_.printBatteryString()); break;
	case 1:	lcdPrint_P(PSTR("V:  ")); 		BLINK(p_.printVoltageString());	break;
	case 2:	lcdPrint_P(PSTR("Ic: ")); 		BLINK(p_.printChargeString()); 	break;
	case 3:	lcdPrint_P(PSTR("Imax: ")); 	BLINK(p_.printCurrentString()); break;
	case 4:	lcdPrint_P(PSTR("Prog: ")); 	BLINK(p_.printProgramString()); break;
	case 5:	lcdPrint_P(PSTR("Bal: ")); 	BLINK(p_.printBalanceString()); break;
	case 6:	lcdPrint_P(PSTR("Vm:  ")); 	BLINK(p_.printVoltageInputString()); break;
	case 7:	lcdPrint_P(PSTR("Im:  ")); 	BLINK(p_.printCurrentInputString()); break;

	case SAVE_POS:	lcdPrint_P(PSTR("     save")); 						break;
	}
}

bool ProgramDataMenu::editItem(int index, uint8_t key)
{
	int dir = -1;
	if(key == BUTTON_INC) dir = 1;
	dir *= keyboard.getSpeedFactor();

	switch(index) {
	case 0:	p_.changeBattery(dir); 	break;
	case 1:	p_.changeVoltage(dir); 	break;
	case 2:	p_.changeCharge(dir); 		break;
	case 3: p_.changeCurrent(dir); 		break;
	case 4:	p_.changeProgram(dir); 		break;
	case 5:	p_.changeBalance(dir); 		break;
	case 6:	p_.changeVoltageInput(dir); 		break;
	case 7:	p_.changeCurrentInput(dir); 		break;
	default:
		return false;
	}
	return true;
}

void ProgramDataMenu::startBlink(int index, bool off)
{
	blinkIndex_ = index;
	blinkStartTime_ = timer.getMiliseconds();
	blinkOff_ = false;
	blinkStartOff_ = off;
}

void ProgramDataMenu::stopBlink()
{
	blinkIndex_ = -1;
}

void ProgramDataMenu::blinkDisplay()
{
	if(getBlink() != blinkOff_)
		display();
}

void ProgramDataMenu::editIndex(int index)
{
	startBlink(index, true);
	ProgramData undo(p_);
	uint8_t key;
	do {
		key =  keyboard.getPressedWithSpeed();
		if(key == BUTTON_DEC || key == BUTTON_INC) {
			editItem(index, key);
			startBlink(index, false);
			display();
		}
		blinkDisplay();
	} while(key != BUTTON_STOP && key != BUTTON_START);

	stopBlink();
	if(key == BUTTON_STOP)
		p_ = undo;
	p_.check();
	display();
}

bool ProgramDataMenu::edit() {
	bool release = true;
	uint8_t key;
	render();
	do {
		key = run();
		if(key == BUTTON_NONE) release = false;

		if(!release && key == BUTTON_START)  {
			if(getIndex() == SAVE_POS) return true;
			editIndex(getIndex());
		}
	} while(key != BUTTON_STOP || release);
	return false;
}

