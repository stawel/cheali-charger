#ifndef BLINK_H
#define BLINK_H

#include "LcdPrint.h"

#define BLINK_INDEX(command) do { if(blink!=index) {command; } else { } }while(false)
//#define BLINK(command, n) do { if(_blink_blink !=_start_blink) {command; } else { lcdPrintSpaces(n); } }while(false)
//#define BLINK_START uint8_t _start_blink = 0, _blink_blink = getBlinkIndex()
//#define BLINK_TEST ( ((blink &0x7f) != _start_blink++) ? 0 : ((blink & BLINK_SPEED2) + 1) )
#define BLINK_SPEED_MS 500
//#define BLINK_END return _start_blink;
//#define BLINK_NON return 0;
#define BLINK_SPEED2 128

class Blink {
public:
	Blink():blinkIndex_(-1){}

	bool getBlink() const;
	int getBlinkIndex();
	void startBlinkOn(int index);
	void startBlinkOff(int index);
	void stopBlink();
	uint16_t getBlinkTime() const;

	int blinkIndex_;
	uint16_t blinkStartTime_;
};

#endif /* BLINK_H */
