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
#include "atomic.h"
#include "AnalogInputs.h"
#include "Hardware.h"

#include "Monitor.h"
#include "Utils.h"
#include "Settings.h"
#include "ProgramData.h"
#include "Program.h"
#include "memory.h"
#include "Balancer.h"

//TODO_NJ
#include "LcdPrint.h"     
#include "Screen.h"
#include "TheveninMethod.h"
   
#if defined(ENABLE_FAN) && defined(ENABLE_T_INTERNAL)
#define MONITOR_T_INTERNAL_FAN
#endif



namespace Monitor {

	uint16_t etaDeltaSec;
    uint16_t etaStartTimeCalc;

	bool on_;
    uint8_t procent_;
    uint16_t startTime_totalTime_U16_;
    uint32_t totalBalanceTime_;
    uint32_t totalChargDischargeTime_;

	uint16_t VoutMaxMesured_;
	uint16_t VoutMinMesured_;

#ifdef MONITOR_T_INTERNAL_FAN
    AnalogInputs::ValueType monitor_on_T;
    AnalogInputs::ValueType monitor_off_T;
#endif

    void calculateDeltaProcentTimeSec();

} // namespace Monitor

void Monitor::calculateDeltaProcentTimeSec()
{
    uint16_t etaSec;
    uint8_t procent = Monitor::getChargeProcent();
    if(procent_ < procent) {
        procent_ = procent;
        etaSec = Time::diffU16(Monitor::etaStartTimeCalc, Monitor::getTimeSec());
        etaStartTimeCalc = Monitor::getTimeSec();
        if (etaSec > etaDeltaSec)  {
            etaDeltaSec=etaSec; // find longer time for deltaprocent
        }
    }
}

uint16_t Monitor::getETATime()
{
    calculateDeltaProcentTimeSec();
    uint8_t kx = 105;
    if(!AnalogInputs::isConnected(AnalogInputs::Vbalancer)) {
        //balancer not connected
        kx=100;
    }

    //if (getChargeProcent()==99) {return (0);} //no avail more calc (or call secondary calculator)
    return (etaDeltaSec*(kx-procent_));
}

void Monitor::resetETA()
{
    etaStartTimeCalc=0;
    procent_ = getChargeProcent();
    etaDeltaSec = 0;
}

uint16_t Monitor::getTimeSec()
{
    uint16_t t = startTime_totalTime_U16_;
    if(on_) t = Time::diffU16(startTime_totalTime_U16_, Time::getSecondsU16());
    return t;
}

uint16_t Monitor::getTotalBalanceTimeSec() {
	return totalBalanceTime_/1000;
}

uint16_t Monitor::getTotalChargeDischargeTimeSec() {
	return totalChargDischargeTime_/1000;
}

uint16_t Monitor::getTotalChargeDischargeTimeMin() {
	return totalChargDischargeTime_/1000/60;
}



uint8_t Monitor::getChargeProcent() {
    uint16_t v1,v2, v;
    v2 = ProgramData::currentProgramData.getVoltage(ProgramData::VCharge);
    v1 = ProgramData::currentProgramData.getVoltage(ProgramData::ValidEmpty);
    v =  AnalogInputs::getRealValue(AnalogInputs::VoutBalancer);

    if(v >= v2) return 99;
    if(v <= v1) return 0;
    v-=v1;
    v2-=v1;
    v2/=100;
    v=  v/v2;
    if(v > 99) v=99; //not 101% with isCharge
    return v;
}

void Monitor::doIdle()
{
#ifdef MONITOR_T_INTERNAL_FAN

    AnalogInputs::ValueType t = AnalogInputs::getADCValue(AnalogInputs::Tintern);
    bool retu = false;
    if(t > monitor_off_T) {
        hardware::setFan(false);
    } else if(t < monitor_on_T) {
        hardware::setFan(true);
    }
#endif
}
void Monitor::update()
{
#ifdef MONITOR_T_INTERNAL_FAN
    monitor_off_T = AnalogInputs::reverseCalibrateValue(AnalogInputs::Tintern, settings.fanTempOn - Settings::TempDifference);
    monitor_on_T  = AnalogInputs::reverseCalibrateValue(AnalogInputs::Tintern, settings.fanTempOn);
#endif
}

void Monitor::powerOn() {
    VoutMaxMesured_ = AnalogInputs::reverseCalibrateValue(AnalogInputs::Vout_plus_pin, MAX_CHARGE_V+ANALOG_VOLT(3.000));
    VoutMinMesured_ = AnalogInputs::reverseCalibrateValue(AnalogInputs::Vout_plus_pin, AnalogInputs::CONNECTED_MIN_VOLTAGE);
    update();

    startTime_totalTime_U16_ = Time::getSecondsU16();
    totalBalanceTime_ = 0;
    totalChargDischargeTime_ = 0;
    on_ = true;
    resetETA();
}

void Monitor::powerOff()
{
    startTime_totalTime_U16_ = getTimeSec();
    on_ = false;
}

void Monitor::doSlowInterrupt()
{
   if(SMPS::isWorking() || Discharger::isWorking())
       totalChargDischargeTime_ += SLOW_INTERRUPT_PERIOD_MILISECONDS;

   if(Balancer::isWorking())
       totalBalanceTime_ += SLOW_INTERRUPT_PERIOD_MILISECONDS;
}


Strategy::statusType Monitor::run()
{
	//TODO: ??
	if(!on_) {
		return Strategy::RUNNING;
	}
#ifdef ENABLE_T_INTERNAL
    AnalogInputs::ValueType t = AnalogInputs::getRealValue(AnalogInputs::Tintern);

    if(t > settings.dischargeTempOff + Settings::TempDifference) {
        Program::stopReason_ = string_internalTemperatureToHigh;
        return Strategy::ERROR;
    }
#endif

    AnalogInputs::ValueType VMout = AnalogInputs::getADCValue(AnalogInputs::Vout_plus_pin);
    if(VoutMaxMesured_ < VMout || (VMout < VoutMinMesured_ && Discharger::isPowerOn())) {
        Program::stopReason_ = string_batteryDisconnected;
        return Strategy::ERROR;
    }

    //TODO: NJ if disconnected balancer
    if (settings.forceBalancePort && SMPS::isPowerOn() && ProgramData::currentProgramData.isLiXX() && (ProgramData::currentProgramData.battery.cells > 1)) 
    {
        if(!AnalogInputs::isConnected(AnalogInputs::Vb1)) {
            Program::stopReason_ = string_balancePortBreak;
            return Strategy::ERROR;
        }
    }

    //charger hardware failure (smps q2 short)
    AnalogInputs::ValueType v = ANALOG_AMP(0.000);
    if (SMPS::isPowerOn()) {v = ProgramData::currentProgramData.getMaxIc();}
    if (Discharger::isPowerOn()) {v = ProgramData::currentProgramData.getMaxId();}
    //TODO stawel: should be fixed
    if (v + ANALOG_AMP(1.000) <  AnalogInputs::Iout) {
        Program::stopReason_ = string_outputCurrentToHigh;
        AnalogInputs::powerOff();   //disconnect the battery (pin12 off)
        return Strategy::ERROR;               
    }

    AnalogInputs::ValueType Vin = AnalogInputs::getRealValue(AnalogInputs::Vin);
    if(AnalogInputs::isConnected(AnalogInputs::Vin) && Vin < settings.inputVoltageLow) {
        Program::stopReason_ = string_inputVoltageToLow;
        return Strategy::ERROR;
    }

    AnalogInputs::ValueType c = AnalogInputs::getRealValue(AnalogInputs::Cout);
    AnalogInputs::ValueType c_limit  = ProgramData::currentProgramData.getCapacityLimit();
    if(c_limit != PROGRAM_DATA_MAX_CHARGE && c > c_limit) {
        Program::stopReason_ = string_capacityLimit;
        return Strategy::COMPLETE;
    }

#ifdef ENABLE_TIME_LIMIT
//TODO_NJ timelimit
    if (ProgramData::currentProgramData.getTimeLimit() < 1000)  //unlimited
    {
        uint16_t chargeMin = getTotalChargeDischargeTimeMin();
        uint16_t time_limit  = ProgramData::currentProgramData.getTimeLimit();
        if(chargeMin >= time_limit) {
            Program::stopReason_ = string_timeLimit;
            return Strategy::COMPLETE;
        }
    }
//timelimit end
#endif

    if(settings.externT) {
        AnalogInputs::ValueType Textern = AnalogInputs::getRealValue(AnalogInputs::Textern);
        if(Textern > settings.externTCO) {
            Program::stopReason_ = string_externalTemperatureCutOff;
            return Strategy::ERROR;
        }
    }

    return Strategy::RUNNING;
}

