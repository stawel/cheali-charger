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

#include "LcdPrint.h"
#include "Screen.h"
#include "TheveninMethod.h"

#if defined(ENABLE_FAN) && defined(ENABLE_T_INTERNAL)
#define MONITOR_T_INTERNAL_FAN
#endif



namespace Monitor {

    uint16_t etaDeltaSec;
    uint16_t etaStartTimeCalc;

    bool isBalancePortConnected;

    bool on_;
    uint8_t procent_;
    uint16_t startTime_totalTime_U16_;
    uint32_t totalBalanceTime_;
    uint32_t totalChargDischargeTime_;

    uint16_t Vout_plus_adcMinLimit_;
    uint16_t Vout_plus_adcMaxLimit_;

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
    if(!Monitor::isBalancePortConnected) {
        //balancer not connected
        kx=100;
    }

    //if (getChargeProcent()==99) {return (0);} //no avail more calc (or call secondary calculator)
    return (etaDeltaSec*(kx-procent_));
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
    v2 = ProgramData::getVoltage(ProgramData::VCharge);
    v1 = ProgramData::getVoltage(ProgramData::ValidEmpty);
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
    bool fan;
    if(settings.fanOn == Settings::FanAlways) {
        fan = true;
    } else if (settings.fanOn == Settings::FanDisabled
               || (settings.fanOn == Settings::FanProgramTemperature && on_ == false)) {
        fan = false;
    } else if (settings.fanOn == Settings::FanProgram) {
        fan = on_;
    } else if ((settings.fanOn == Settings::FanProgramTemperature && on_ == true)
               || settings.fanOn == Settings::FanTemperature) {
        AnalogInputs::ValueType t = AnalogInputs::getRealValue(AnalogInputs::Tintern);

        if (t < settings.fanTempOn - Settings::TempDifference) {
            fan = false;
        } else if (t > settings.fanTempOn) {
            fan = true;
        } else {
            // don't update fan (hysteresis)
            return;
        }
    }
    hardware::setFan(fan);
#endif
}

void Monitor::powerOn()
{

    Vout_plus_adcMinLimit_ = AnalogInputs::reverseCalibrateValue(AnalogInputs::Vout_plus_pin, AnalogInputs::CONNECTED_MIN_VOLTAGE);

    {
        //Make sure Vout_plus gets not higher VCharge + 3V (additional safety limit for protection ICs)
        AnalogInputs::ValueType Vmax = ProgramData::getVoltage(ProgramData::VCharge);
        Vmax += ANALOG_VOLT(3.000);
        if(Vmax > MAX_CHARGE_V) {
            Vmax = MAX_CHARGE_V;
        }
        hardware::setVoutCutoff(Vmax);

        Vout_plus_adcMaxLimit_ = AnalogInputs::reverseCalibrateValue(AnalogInputs::Vout_plus_pin, Vmax);
        if(Vout_plus_adcMaxLimit_ > ANALOG_INPUTS_MAX_ADC_Vout_plus_pin) {
            Vout_plus_adcMaxLimit_ = ANALOG_INPUTS_MAX_ADC_Vout_plus_pin;
        }
    }

    isBalancePortConnected = AnalogInputs::isBalancePortConnected();

    startTime_totalTime_U16_ = Time::getSecondsU16();
    resetAccumulatedMeasurements();
    on_ = true;
}

void Monitor::resetAccumulatedMeasurements()
{
    procent_ = getChargeProcent();
    etaStartTimeCalc = 0;
    etaDeltaSec = 0;

    totalBalanceTime_ = 0;
    totalChargDischargeTime_ = 0;
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
    if(!on_) {
        return Strategy::RUNNING;
    }
#ifdef ENABLE_T_INTERNAL
    AnalogInputs::ValueType t = AnalogInputs::getRealValue(AnalogInputs::Tintern);

    if(t > settings.dischargeTempOff + Settings::TempDifference) {
        Program::stopReason = string_internalTemperatureToHigh;
        return Strategy::ERROR;
    }
#endif

    AnalogInputs::ValueType VMout = AnalogInputs::getADCValue(AnalogInputs::Vout_plus_pin);
    if(Vout_plus_adcMaxLimit_ <= VMout || (VMout < Vout_plus_adcMinLimit_ && Discharger::isPowerOn())) {
        Program::stopReason = string_batteryDisconnected;
        return Strategy::ERROR;
    }

    if (isBalancePortConnected != AnalogInputs::isBalancePortConnected()) {
        Program::stopReason = string_balancePortDisconnected;
        return Strategy::ERROR;
    }

    AnalogInputs::ValueType i_limit = Strategy::maxI + ANALOG_AMP(1.000);
    if (i_limit < AnalogInputs::getIout()) {
        Program::stopReason = string_outputCurrentToHigh;
        return Strategy::ERROR;
    }

/*    AnalogInputs::ValueType Vin = AnalogInputs::getRealValue(AnalogInputs::Vin);
    if(Vin < settings.inputVoltageLow) {
        Program::stopReason = string_inputVoltageToLow;
        return Strategy::ERROR;
    }*/

    AnalogInputs::ValueType c = AnalogInputs::getRealValue(AnalogInputs::Cout);
    AnalogInputs::ValueType c_limit  = ProgramData::getCapacityLimit();
    if(c_limit != ANALOG_MAX_CHARGE && c_limit <= c) {
        Program::stopReason = string_capacityLimit;
        return Strategy::COMPLETE;
    }

#ifdef ENABLE_TIME_LIMIT
    if (ProgramData::getTimeLimit() < ANALOG_MAX_TIME_LIMIT)  //unlimited
    {
        uint16_t charge_time = getTotalChargeDischargeTimeMin();
        uint16_t time_limit  = ProgramData::getTimeLimit();
        if(time_limit <= charge_time) {
            Program::stopReason = string_timeLimit;
            return Strategy::COMPLETE;
        }
    }
#endif //ENABLE_TIME_LIMIT

    if(ProgramData::battery.enable_externT) {
        AnalogInputs::ValueType Textern = AnalogInputs::getRealValue(AnalogInputs::Textern);
        if(ProgramData::battery.externTCO < Textern) {
            Program::stopReason = string_externalTemperatureCutOff;
            return Strategy::ERROR;
        }
    }

    return Strategy::RUNNING;
}

