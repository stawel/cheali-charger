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


int Blink::getBlinkIndex()
{
	if(getBlink()) {
		return blinkIndex_;
	}
	return -1;
}

void Blink::startBlinkOn(int index)
{
	blinkIndex_ = index;
	blinkStartTime_ = timer.getMiliseconds() - getBlinkTime();
}
void Blink::startBlinkOff(int index)
{
	blinkIndex_ = index;
	blinkStartTime_ = timer.getMiliseconds();
}

void Blink::stopBlink()
{
	blinkIndex_ = -1;
}


