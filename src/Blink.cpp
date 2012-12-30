#include "Timer.h"
#include "Blink.h"

bool Blink::getBlinkOff() const
{
	if(blinkIndex_ >= 0) {
		uint8_t mili = blinkTime_;
		mili/=getBlinkTime();
		if((mili+1)%2) return true;
	}
	return false;
}

bool Blink::getBlinkChanged() const
{
	if(blinkIndex_ >= 0) {
		uint8_t mili1 = blinkTime_-1;
		mili1/=getBlinkTime();
		uint8_t mili2 = blinkTime_;
		mili2/=getBlinkTime();
		return mili1 != mili2;
	}
	return false;
}


