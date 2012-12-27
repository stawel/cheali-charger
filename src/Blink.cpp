#include "Timer.h"
#include "Blink.h"

bool Blink::getBlink() const
{
	if(blinkIndex_ >= 0) {
		uint16_t mili = timer.getMiliseconds() - blinkStartTime_;
		mili/=getBlinkTime();
		if((mili+1)%2) return true;
	}
	return false;
}

uint16_t Blink::getBlinkTime() const
{
	if(blinkIndex_ & BLINK_SPEED2) {
		return BLINK_SPEED_MS/4;
	} else {
		return BLINK_SPEED_MS;
	}
}


void Blink::startBlinkOn(int8_t index)
{
	blinkIndex_ = index;
	blinkStartTime_ = timer.getMiliseconds() - getBlinkTime();
}
void Blink::startBlinkOff(int8_t index)
{
	blinkIndex_ = index;
	blinkStartTime_ = timer.getMiliseconds();
}

void Blink::stopBlink()
{
	blinkIndex_ = -1;
}


