
#include "Utils.h"

uint8_t selectIndexWithKeyboard(uint8_t &x, const uint8_t tab_size,  bool exactly)
{
	return setNumberWithKeyboard<uint8_t>(x, 0, tab_size - 1, 1, exactly);
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
