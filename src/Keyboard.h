#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <stdint.h>

#define BUTTON_NONE		0

#define BUTTON_STOP		1
#define BUTTON_DEC		2
#define BUTTON_INC		4
#define BUTTON_START	8

#define BUTTON_DELAY_TIMES	250
#define BUTTON_DELAY	1
#define BUTTON_CHANGE_DELAY		50


class Keyboard {
	uint8_t last_key_;
	uint8_t speed_;
	uint8_t this_speed_;
public:
	Keyboard();
	uint8_t getPressed() const;
	uint8_t getPressedWithSpeed();
	uint8_t getSpeed() {return speed_; };
	uint8_t getPressTime(uint8_t limit = 2);
	bool isLongPressTime() const { return speed_ > 1; }

	uint16_t getSpeedFactor() const;

private:
	uint8_t keyChanged(uint8_t key);
};


#endif /* KEYBOARD_H_ */
