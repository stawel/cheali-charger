
#include "Utils.h"

uint8_t selectIndexWithKeyboard(uint8_t &x, const uint8_t tab_size,  bool exactly)
{
	return setNumberWithKeyboard<uint8_t>(x, 0, tab_size - 1, 1, exactly);
}

uint8_t setTypedValueWithKeyboard(double &x, uint8_t type)
{
	switch (type) {
		case V_VALUE_TYPE: return setNumberWithKeyboard<double>(x, 0, 30, 0.01, true);
		case I_VALUE_TYPE: return setNumberWithKeyboard<double>(x, 0, 10, 0.01, true);
		case C_VALUE_TYPE: return setNumberWithKeyboard<double>(x, 0, 20, 0.01, true);
	}
	return BUTTON_NONE;
}





