/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013  Paweł Stawicki. All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <stdint.h>
#include "AnalogInputs.h"


namespace Settings {

	static const uint16_t UARTSpeeds = 5;
	static const AnalogInputs::ValueType TempDifference = ANALOG_CELCIUS(5.12);

	enum UARTType {Disabled, Normal,  Debug,  ExtDebug, ExtDebugAdc};
	enum FanOnType {FanDisabled, FanAlways, FanProgram, FanTemperature, FanProgramTemperature};

	enum UARTOutput {Software, HardwarePin7, HardwarePin38};
	enum MenuType  {MenuSimple, MenuAdvanced};

	typedef struct {

		uint16_t backlight;

		uint16_t fanOn;
		AnalogInputs::ValueType fanTempOn;
		AnalogInputs::ValueType dischargeTempOff;

		uint16_t audioBeep;
		AnalogInputs::ValueType minIc;
		AnalogInputs::ValueType maxIc;
		AnalogInputs::ValueType minId;
		AnalogInputs::ValueType maxId;

		AnalogInputs::ValueType inputVoltageLow;
		uint16_t adcNoise;
		uint16_t UART;
		uint16_t UARTspeed;
		uint16_t UARToutput;
		uint16_t menuType;

	} Data;


	extern const uint32_t UARTSpeedValue[];

	void apply();
	void setDefault();
	uint32_t getUARTspeed();

	void load();
	void save();
	void check();
	void restoreDefault();
}

extern Settings::Data settings;

#endif /* SETTINGS_H_ */
