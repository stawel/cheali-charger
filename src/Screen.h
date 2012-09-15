#ifndef SCREEN_H_
#define SCREEN_H_

#include <inttypes.h>

class Screen {
public:
	uint16_t Rth_;
	uint16_t Vth_;
	uint16_t I_;

	Screen() {};
	enum VariableType { Voltage, Current, Charge, Temperature, Unknown };
	enum ScreenType { Screen1, ScreenCIVlimits, ScreenTime, ScreenTemperature,
		ScreenBalancer0_2, ScreenBalancer3_5,
		ScreenBalancer0_2M, ScreenBalancer3_5M,
			ScreenRthVth};

	uint8_t display(ScreenType screen, uint8_t blink = -1);

	uint8_t displayScreen1(uint8_t blink = -1);
	uint8_t displayScreenCIVlimits(uint8_t blink = -1);
	uint8_t displayScreenBalancer0_2(uint8_t blink = -1);
	uint8_t displayScreenBalancer3_5(uint8_t blink = -1);
	uint8_t displayScreenBalancer0_2R(uint8_t blink = -1);
	uint8_t displayScreenBalancer3_5R(uint8_t blink = -1);
	uint8_t displayScreenTime(uint8_t blink = -1);
	uint8_t displayScreenTemperature(uint8_t blink = -1);

	uint8_t displayChargeEnded(uint8_t blink = -1);
	uint8_t displayRthVth(uint8_t blink = -1);

	void displayMonitorError();

	void displayStrings(const char *s1, const char *s2) const;
};

extern Screen screens;

#endif /* SCREEN_H_ */
