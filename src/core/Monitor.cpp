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

//TODO_NJ
#include "LcdPrint.h"     
#include "Screen.h"
#include "TheveninMethod.h"
   

namespace Monitor {

#if defined(ENABLE_FAN) && defined(ENABLE_T_INTERNAL)
#define MONITOR_T_INTERNAL_FAN
#endif

uint16_t VoutMaxMesured_;
uint16_t VoutMinMesured_;

#ifdef MONITOR_T_INTERNAL_FAN
    AnalogInputs::ValueType monitor_on_T;
    AnalogInputs::ValueType monitor_off_T;
#endif
} // namespace Monitor



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
    monitor_off_T = AnalogInputs::reverseCalibrateValue(AnalogInputs::Tintern, settings.fanTempOn_ - Settings::TempDifference);
    monitor_on_T  = AnalogInputs::reverseCalibrateValue(AnalogInputs::Tintern, settings.fanTempOn_);
#endif
}

void Monitor::powerOn() {
    VoutMaxMesured_ = AnalogInputs::reverseCalibrateValue(AnalogInputs::Vout_plus_pin, MAX_CHARGE_V+ANALOG_VOLT(3.000));
    VoutMinMesured_ = AnalogInputs::reverseCalibrateValue(AnalogInputs::Vout_plus_pin, AnalogInputs::CONNECTED_MIN_VOLTAGE);
    update();
}


Strategy::statusType Monitor::run()
{
#ifdef ENABLE_T_INTERNAL
    AnalogInputs::ValueType t = AnalogInputs::getRealValue(AnalogInputs::Tintern);

    if(t > settings.dischargeTempOff_+Settings::TempDifference) {
        Program::stopReason_ = PSTR("INT T");
        return Strategy::ERROR;
    }
#endif

    AnalogInputs::ValueType VMout = AnalogInputs::getADCValue(AnalogInputs::Vout_plus_pin);
    if(VoutMaxMesured_ < VMout || (VMout < VoutMinMesured_ && Discharger::isPowerOn())) {
        Program::stopReason_ = PSTR("BAT disc");
        return Strategy::ERROR;
    }


    //TODO: NJ if disconnected balancer
    if (settings.forceBalancePort_ && SMPS::isPowerOn && ProgramData::currentProgramData.isLiXX()) 
        {
        bool checkBal = AnalogInputs::isConnected(AnalogInputs::Name(AnalogInputs::Vb1));
        if(!checkBal) 
        {  
          Program::stopReason_ =   PSTR("BAT break");
            return Strategy::ERROR;
        }
    }


    if (SMPS::isPowerOn()) 
        {
        
        if((TheveninMethod::Vend_ + ANALOG_VOLT(0.500)) < AnalogInputs::Vout) 
        {  
          Program::stopReason_ =   PSTR("OVERLOAD err");
            return Strategy::ERROR;
        }
    }



    //charger hardware failure (smps q2 short)
    AnalogInputs::ValueType v = ANALOG_AMP(0.000);
    if (SMPS::isPowerOn()) {v = ProgramData::currentProgramData.getMaxIc();}
    if (Discharger::isPowerOn()) {v = ProgramData::currentProgramData.getMaxId();}
    if (v + ANALOG_AMP(1.000) <  AnalogInputs::Iout ) 
    {
        Program::stopReason_ = PSTR("HW FAILURE");
        AnalogInputs::powerOff();   //disconnect the battery (pin12 off)
        return Strategy::ERROR;               
    }




    AnalogInputs::ValueType Vin = AnalogInputs::getRealValue(AnalogInputs::Vin);
    if(AnalogInputs::isConnected(AnalogInputs::Vin) && Vin < settings.inputVoltageLow_) {
        Program::stopReason_ = PSTR("INPUT V");
        return Strategy::ERROR;
    }

    AnalogInputs::ValueType c = AnalogInputs::getRealValue(AnalogInputs::Cout);
    AnalogInputs::ValueType c_limit  = ProgramData::currentProgramData.getCapacityLimit();
    if(c_limit != PROGRAM_DATA_MAX_CHARGE && c > c_limit) {
        Program::stopReason_ = PSTR("CAP COFF");
        return Strategy::ERROR;
    }
    
#ifdef ENABLE_TIME_LIMIT      
//TODO_NJ timelimit     
    if (ProgramData::currentProgramData.getTimeLimit() < 1000)  //unlimited
    {
        uint16_t chargeMin = Screen::getTotalChargDischargeTime();
        uint16_t time_limit  = ProgramData::currentProgramData.getTimeLimit();
        if(chargeMin >= time_limit) {
            Program::stopReason_ = PSTR("T limit");
            return Strategy::ERROR;
        }               
    }
//timelimit end      
#endif
    
    if(settings.externT_) {
        AnalogInputs::ValueType Textern = AnalogInputs::getRealValue(AnalogInputs::Textern);
        if(Textern > settings.externTCO_) {
            Program::stopReason_ = PSTR("EXT TCOF");
            return Strategy::ERROR;
        }
    }

    return Strategy::RUNNING;
}

