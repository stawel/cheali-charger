#ifndef UTILS_H_
#define UTILS_H_

#include "Hardware.h"

template<class digit_type>
uint8_t setNumberWithKeyboard(digit_type &x, const digit_type x_min,
		const digit_type x_max, const digit_type x_step = 1, bool exactly = true)
{
	uint8_t key =  keyboard.getPressedWithSpeed();
	if(exactly) {
		if(key == BUTTON_INC && x < x_max) x+=x_step;
		if(key == BUTTON_DEC && x > x_min) x-=x_step;
	} else {
		if(key & BUTTON_INC && x < x_max) x+=x_step;
		if(key & BUTTON_DEC && x > x_min) x-=x_step;
	}
	if(x > x_max) {
		x = x_max;
	} else if( x < x_min) {
		x = x_min;
	}
	return key;
}

uint8_t setTypedValueWithKeyboard(double &x, uint8_t type);
uint8_t selectIndexWithKeyboard(uint8_t &x, const uint8_t tab_size, bool exactly = true);

#define UNKNOWN_VALUE_TYPE	0
#define V_VALUE_TYPE 		1
#define I_VALUE_TYPE 		2
#define C_VALUE_TYPE 		3

#ifndef NULL
#define NULL 0
#endif

#define sizeOfArray(x) (sizeof(x)/sizeof((x)[0]))
#endif /* UTILS_H_ */
