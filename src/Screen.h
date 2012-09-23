#ifndef SCREEN_H_
#define SCREEN_H_

#include <inttypes.h>
#include "Program.h"

class Screen {
public:
	uint16_t Rth_;
	uint16_t Vth_;
	uint16_t valueTh_;
	double charge_;
	Program::ProgramType programType_;

	Screen() {};
	enum VariableType { Voltage, Current, Charge, Temperature, Unknown };
	enum ScreenType { Screen1, ScreenCIVlimits, ScreenTime, ScreenTemperature,
		ScreenBalancer0_2, ScreenBalancer3_5,
		ScreenBalancer0_2M, ScreenBalancer3_5M,
			ScreenRthVth,
			ScreenStartInfo};

	void display(ScreenType screen);

	void displayScreen1();
	void displayScreenCIVlimits();
	void displayScreenBalancer0_2();
	void displayScreenBalancer3_5();
	void displayScreenBalancer0_2R();
	void displayScreenBalancer3_5R();
	void displayScreenTime();
	void displayScreenTemperature();

	void displayChargeEnded();
	void displayRthVth();

	void displayMonitorError();
	void displayStartInfo();


	void displayStrings(const char *s1, const char *s2) const;

	static void notImplemented();

};

extern Screen screens;

#endif /* SCREEN_H_ */
