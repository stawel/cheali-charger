#ifndef CALIBRATE_H_
#define CALIBRATE_H_

#include "AnalogInputs.h"

#define ACCEPT_DELAY 3

class Calibrate {
public:
	uint16_t value_;
	uint8_t dispVal_;
	int8_t blink_;
	bool blinkOn_;

	enum screenType { SCREEN_IOUT = 0, SCREEN_VOUT = 1, SCREEN_B0_2 = 2, SCREEN_B3_5 = 3,
			SCREEN_VOUT_VBAL = 99, SCREEN_IDISCHARGE = 100,
			SCREEN_B0_2_BLINK = 101, SCREEN_B3_5_BLINK = 102};

	Calibrate(){};

	void print_v(uint8_t dig = 3);
	void print_d(AnalogInputs::Name name, int dig = 7);
	void print_m(const char *str, AnalogInputs::Name name, int dig = 7);
	void print_m_2(const char *str, AnalogInputs::Name name, int dig = 7);
	void print_m_1(const char *str, AnalogInputs::Name name, int dig = 7);
	void print_m_3(const char *str, AnalogInputs::Name name, int dig = 7);
	void printCalibrateI();
	void printCalibrateDis();
	void printCalibrateVout();
	void printCalibrateVoutVbal();
	void printCalibrateB0_2();
	void printCalibrateB3_5();
	void printCalibrateT();
	void printCalibrateVin();

	void printCalibrateB0_2_Blink();
	void printCalibrateB3_5_Blink();

	void printCalibrate(screenType screen);
	void info(screenType screen);
	void infoDis();
	void infoTimeM();

	void run();
	void runInfo();
	bool calibrate(screenType screen);
	bool calibrateBlink(screenType screen, int8_t maxBlink);
	bool calibrateDischarge();

	void setBlink(screenType screen);
	void copyVbalVout();
	void calibrateI(screenType screen, AnalogInputs::Name name1, AnalogInputs::Name name2);
	void setBalancer(AnalogInputs::Name firstName);
	bool setValue(uint8_t x, uint8_t y, AnalogInputs::ValueType &v, AnalogInputs::Type, uint8_t dig);

};

#endif /* CALIBRATE_H_ */
