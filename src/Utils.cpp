
#include "Utils.h"


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

uint8_t selectIndexWithKeyboard(uint8_t &x, const uint8_t tab_size)
{
	return setNumberWithKeyboard<uint8_t>(x, 0, tab_size - 1, 1);
}


bool testTinern(bool &more, AnalogInputs::ValueType off, AnalogInputs::ValueType on)
{
	AnalogInputs::ValueType t = analogInputs.getRealValue(AnalogInputs::Tintern);
	bool retu = false;
	if(t < off) {
		retu = true;
		more =  false;
	} else if(t > on) {
		retu = true;
		more = true;
	}
	return retu;
}

uint16_t pow10(uint8_t n)
{
	uint16_t retu = 1;
	while(n--) {
		retu*=10;
	}
	return retu;
}

int8_t sign(int16_t x)
{
	if(x>=0) return 1;
	return -1;
}

uint8_t digits(uint16_t x)
{
	uint8_t retu = 0;
	if(x == 0)
		x=1;
	for(;x!=0; x/=10)
		retu++;
	return retu;
}


void changeMax(uint16_t &v, int direc, uint16_t max)
{
	uint16_t r;
	int step = 1;
	bool direction = direc > 0;

	uint8_t dv = digits(v);
	if(dv>1) step = pow10(dv-2);

	r = v%step;

	if(r) {
		if(direction) step -=r;
		else step = r;
	}
	if(direction) ADD_MAX(v, step, max);
	else SUB_MIN(v, step ,1);
}

