#ifndef CALIBRATE_H_
#define CALIBRATE_H_

#include "AnalogInputs.h"

#define ACCEPT_DELAY 3

class Calibrate {
public:
	uint16_t value_;
	uint8_t dispVal_;

	enum screenType { SCREEN_IOUT = 0, SCREEN_VOUT_VBAL = 99, SCREEN_IDISCHARGE = 100};

	Calibrate(){};

	void print_v(uint8_t dig = 3);
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

	void printCalibrate(int p);
	void info(int p);
	void infoDis();
	void infoTimeM();

	void run();
	void runInfo();
	bool calibrate(int p);
	bool calibrateDischarge();

	void copyVbalVout();
	void calibrateI(screenType screen, AnalogInputs::Name name1, AnalogInputs::Name name2);
};

#endif /* CALIBRATE_H_ */
