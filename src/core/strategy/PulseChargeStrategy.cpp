/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2013  Pawel Stawicki. All right reserved.

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

#include "PulseChargeStrategy.h"
#include "TheveninChargeStrategy.h"
#include "Time.h"
#include "memory.h"
#include "Strategy.h"
#include "Buzzer.h"

#include "SMPS.h"
#include "Hardware.h"
#include "ProgramData.h"
#include "Screen.h"
#include "TheveninMethod.h"
#include "Balancer.h"

#include "Utils.h"

// #define ENABLE_DEBUG
#include "debug.h"

#ifdef ENABLE_PULSE_CHARGE_STRATEGY

namespace PulseChargeStrategy{
#define MAX_PERIOD 65
    const Strategy::VTable vtable PROGMEM = {
        powerOn,
        powerOff,
        doStrategy
    };
	
	uint16_t start_time_msec_U16_ = 0;	// time in msec
	AnalogInputs::ValueType pauseVoltage = 0;
	uint16_t pause_ = 0;			// time in msec
	uint16_t work_ = 0;			// time in msec
	AnalogInputs::ValueType newI_; // Current "I"
	int16_t incrementI_;
	uint16_t currentValue;
//	uint8_t period = 20;
	uint8_t duty;	// 1..99
	
	PulseChargeStrategy::ActionType action = NONE;
}

void PulseChargeStrategy::switchMode()
{
	PulseChargeStrategy::ActionType lastAction;
	if (action != NONE){
		lastAction = action;
		powerOff();
	}else{
		// at first run
		lastAction = PAUSE;
		start_time_msec_U16_ = Time::getMilisecondsU16()-pause_;
		TheveninMethod::initialize(true);
		uint16_t mult = 100 / duty;
		uint16_t pulseCurrent = ProgramData::currentProgramData.battery.Ic * mult;
		if (pulseCurrent > MAX_CHARGE_I)
			pulseCurrent = MAX_CHARGE_I;
		Strategy::maxI = pulseCurrent;
		newI_ = ProgramData::currentProgramData.battery.Ic;
		currentValue = AnalogInputs::reverseCalibrateValue(AnalogInputs::IsmpsSet, newI_);
		SMPS::setValue(currentValue);
	}
	uint16_t time = Time::getMilisecondsU16();
	uint16_t overTime;
	if (lastAction == PulseChargeStrategy::PAUSE){
		// Power On
		pauseVoltage = AnalogInputs::getVbattery();
		Balancer::powerOn();
		SMPS::powerOn();
    	// Strategy::setVI(ProgramData::VCharge, true);
    	// TheveninMethod::initialize(true);
		time = Time::getMilisecondsU16();
		STATIC_ASSERT(currentValue == AnalogInputs::reverseCalibrateValue(AnalogInputs::IsmpsSet, newI_));
    	SMPS::setValue(currentValue);
		action = PulseChargeStrategy::WORKING;
		overTime = Time::diffU16(start_time_msec_U16_, time) - pause_;
	}else{
		action = PulseChargeStrategy::PAUSE;
		overTime = Time::diffU16(start_time_msec_U16_, time) - work_;
	}
	STATIC_ASSERT(overTime >= 0);
	start_time_msec_U16_ = time - overTime;
}

void PulseChargeStrategy::powerOn(){
	STATIC_ASSERT(pause_ && work_);
	switchMode();
}

void PulseChargeStrategy::powerOff()
{
	if (action == PulseChargeStrategy::WORKING){
		currentValue = SMPS::getValue();
		SMPS::powerOff();
		Balancer::powerOff();
	}
	action = NONE;
}

bool PulseChargeStrategy::isPause()
{
	return (action == PulseChargeStrategy::PAUSE);
}

Strategy::statusType PulseChargeStrategy::tuneCurrent()
{
    // bool updateI = true;
    if(/*updateI*/true) {
		AnalogInputs::ValueType currentV = AnalogInputs::getVbattery();
		int16_t diffV = currentV - Strategy::endV;
		uint16_t divider = Strategy::endV / (diffV>=0? diffV: -diffV);
		// int16_t newIncrementI = 0;
		if (diffV > 0){// Overvoltage
			incrementI_ = -(newI_/divider);
		}else if (diffV < 0){
			incrementI_ = newI_/divider;
		}
		newI_ += incrementI_;
        // incrementI_ = newIncrementI;

        if (newI_ > Strategy::maxI)
			newI_ = Strategy::maxI;

		LogDebug("dropV=", currentV - pauseVoltage, "; newI=", newI_);
		if (incrementI_!=0)
			SMPS::setValue(AnalogInputs::reverseCalibrateValue(AnalogInputs::IsmpsSet, newI_));
    }
	if (newI_ <= ProgramData::currentProgramData.battery.C / duty)
		return Strategy::COMPLETE;
	
    return Strategy::RUNNING;
}

Strategy::statusType PulseChargeStrategy::doStrategy()
{
	if (action == WORKING){
		uint16_t pauseWorkTime = Time::diffU16(start_time_msec_U16_, Time::getMilisecondsU16());
		if(pauseWorkTime >= work_) {
			Strategy::statusType status = tuneCurrent();
			if (status != Strategy::RUNNING)
				return status;
        	switchMode();
		}
	}else if (action == PAUSE){
		uint16_t pauseWorkTime = Time::diffU16(start_time_msec_U16_, Time::getMilisecondsU16());
		if(pauseWorkTime >= pause_) {
        	switchMode();
		}
	}
    return Strategy::RUNNING;
}

void PulseChargeStrategy::setPeriodAndDuty(uint8_t seconds, uint8_t duty_)
{
	if (seconds > MAX_PERIOD)
		seconds = MAX_PERIOD;
	else if (seconds < 2)
		seconds = 2;
	
	if (duty_ >= 100)
		duty_ = 99;
	else if (duty_ == 0)
		duty_ = 1;
	
//	period = seconds;
	duty = duty_;

	uint16_t periodInMsec = seconds * 1000;
	work_ = periodInMsec / 100 * duty_;
	pause_ = periodInMsec - work_;
}

#endif
