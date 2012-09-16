#ifndef CALIBRATE_H_
#define CALIBRATE_H_

#include "AnalogInputs.h"

class Calibrate {
public:
	uint16_t value;
	uint16_t dispVal;

	void print_v(uint8_t dig = 3);
	void print_m(const char *str, AnalogInputs::Name name, int dig = 7);
	void print_m_2(const char *str, AnalogInputs::Name name, int dig = 7);
	void print_m_1(const char *str, AnalogInputs::Name name, int dig = 7);
	void print_m_3(const char *str, AnalogInputs::Name name, int dig = 7);
	void printCalibrateI();
	void printCalibrateDis();
	void printCalibrateVout();
	void printCalibrateB0_2();
	void printCalibrateB3_5();
	void printCalibrateT();
	void printCalibrateVin();

	void printCalibrate(int p);
	void calibrate(int p);
	void calibrateDis();
	void timeM();

	void run();
	void runInfo();
};

#endif /* CALIBRATE_H_ */
