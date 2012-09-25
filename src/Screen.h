#ifndef SCREEN_H_
#define SCREEN_H_

#include <inttypes.h>
#include "Program.h"
#include "Blink.h"

class Screen : public Blink {
public:
	int16_t Rth_V_;
	int16_t Rth_I_;
	AnalogInputs::ValueType Vth_;
	AnalogInputs::ValueType valueTh_;
	double charge_;
	Program::ProgramType programType_;

	Screen() {};
	enum VariableType { Voltage, Current, Charge, Temperature, Unknown };
	enum ScreenType { Screen1, ScreenCIVlimits, ScreenTime, ScreenTemperature,
		ScreenBalancer0_2, ScreenBalancer3_5,
		ScreenBalancer0_2M, ScreenBalancer3_5M,
		ScreenBalancer0_2RthV, ScreenBalancer3_5RthV,
		ScreenBalancer0_2RthI, ScreenBalancer3_5RthI,
		ScreenBalancer0_2Rth, ScreenBalancer3_5Rth,

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
