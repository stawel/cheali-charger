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
#include "Program.h"
#include "Hardware.h"
#include "ProgramData.h"
#include "LcdPrint.h"
#include "Utils.h"
#include "Screen.h"
#include "SimpleChargeStrategy.h"
#include "TheveninChargeStrategy.h"
#include "TheveninDischargeStrategy.h"
#include "DeltaChargeStrategy.h"
#include "StorageStrategy.h"
#include "Monitor.h"
#include "memory.h"
#include "StartInfoStrategy.h"
#include "Buzzer.h"
#include "StaticMenu.h"
#include "Settings.h"
#include "SerialLog.h"
#include "DelayStrategy.h"

#include "ProgramDCcycle.h"

Program::ProgramType Program::programType_;
Program::ProgramState Program::programState_ = Program::Done;
const char * Program::stopReason_;

namespace Program {
    //TODO: separate Screens from Programs
/*    const Screen::ScreenType deltaChargeScreens[] PROGMEM = {
      Screen::ScreenFirst,
      Screen::ScreenEnergy,
      Screen::ScreenDeltaFirst,
      Screen::ScreenDeltaVout,
      Screen::ScreenDeltaTextern,
      Screen::ScreenR,
      Screen::ScreenVout,
      Screen::ScreenVinput,
      Screen::ScreenTime,
      Screen::ScreenTemperature,
      Screen::ScreenCycles,
      Screen::ScreenCIVlimits,
      Screen::ScreenEnd
    };

    const Screen::ScreenType NiXXDischargeScreens[] PROGMEM = {
      Screen::ScreenFirst,
      Screen::ScreenEnergy,
      Screen::ScreenDeltaTextern,
      Screen::ScreenR,
      Screen::ScreenVout,
      Screen::ScreenVinput,
      Screen::ScreenTime,
      Screen::ScreenTemperature,
      Screen::ScreenCycles,
      Screen::ScreenCIVlimits,
      Screen::ScreenEnd
    };

    const Screen::ScreenType theveninScreens[] PROGMEM = {
      Screen::ScreenFirst,

      Screen::ScreenEnergy,
      Screen::ScreenBalancer1_3,            Screen::ScreenBalancer4_6,      BALANCER_PORTS_GT_6(Screen::ScreenBalancer7_9,)
      Screen::ScreenBalancer1_3Rth,         Screen::ScreenBalancer4_6Rth,   BALANCER_PORTS_GT_6(Screen::ScreenBalancer7_9Rth,)
      Screen::ScreenR,
      Screen::ScreenVout,
      Screen::ScreenVinput,
      Screen::ScreenTime,
      Screen::ScreenTemperature,
      Screen::ScreenCycles,
      Screen::ScreenCIVlimits,
      Screen::ScreenEnd

    };

    const Screen::ScreenType balanceScreens[] PROGMEM = {
      Screen::ScreenBalancer1_3,            Screen::ScreenBalancer4_6,      BALANCER_PORTS_GT_6(Screen::ScreenBalancer7_9,)
      Screen::ScreenTime,
      Screen::ScreenVinput,
      Screen::ScreenTemperature,
      Screen::ScreenEnd
    };

    const Screen::ScreenType dischargeScreens[] PROGMEM = {
      Screen::ScreenFirst,
      Screen::ScreenEnergy,
      Screen::ScreenBalancer1_3,            Screen::ScreenBalancer4_6,      BALANCER_PORTS_GT_6(Screen::ScreenBalancer7_9,)
      Screen::ScreenBalancer1_3Rth,         Screen::ScreenBalancer4_6Rth,   BALANCER_PORTS_GT_6(Screen::ScreenBalancer7_9Rth,)
      Screen::ScreenR,
      Screen::ScreenVout,
      Screen::ScreenVinput,
      Screen::ScreenTime,
      Screen::ScreenTemperature,
      Screen::ScreenCycles,
      Screen::ScreenCIVlimits,
      Screen::ScreenEnd
    };

    const Screen::ScreenType storageScreens[] PROGMEM = {
      Screen::ScreenFirst,
      Screen::ScreenEnergy,
      Screen::ScreenBalancer1_3,            Screen::ScreenBalancer4_6,      BALANCER_PORTS_GT_6(Screen::ScreenBalancer7_9,)
      Screen::ScreenBalancer1_3Rth,         Screen::ScreenBalancer4_6Rth,   BALANCER_PORTS_GT_6(Screen::ScreenBalancer7_9Rth,)
      Screen::ScreenR,
      Screen::ScreenVout,
      Screen::ScreenVinput,
      Screen::ScreenTime,
      Screen::ScreenTemperature,
      Screen::ScreenEnd

    };

    const Screen::ScreenType startInfoBalanceScreens[] PROGMEM = {
      Screen::ScreenStartInfo,
      Screen::ScreenBalancer1_3,            Screen::ScreenBalancer4_6,      BALANCER_PORTS_GT_6(Screen::ScreenBalancer7_9,)
      Screen::ScreenVinput,
      Screen::ScreenTemperature,
      Screen::ScreenEnd
    };

    const Screen::ScreenType startInfoScreens[] PROGMEM = {
      Screen::ScreenStartInfo,
      Screen::ScreenVinput,
      Screen::ScreenTemperature,
      Screen::ScreenEnd
    };
*/
}

bool Program::startInfo()
{
    bool balancer = false;
    Strategy::strategy = &StartInfoStrategy::vtable;
    Strategy::exitImmediately = true;
    if(ProgramData::currentProgramData.isLiXX()) {
        //usues balance port
        balancer = true;
    }
    StartInfoStrategy::setBalancePort(balancer);
    return Strategy::doStrategy() == Strategy::COMPLETE;
}

Strategy::statusType Program::runStorage(bool balance)
{
    StorageStrategy::setDoBalance(balance);
    StorageStrategy::setVII(ProgramData::currentProgramData.getVoltage(ProgramData::VStorage),
            ProgramData::currentProgramData.battery.Ic, ProgramData::currentProgramData.battery.Id);
    Strategy::strategy = &StorageStrategy::vtable;
    return Strategy::doStrategy();
}
Strategy::statusType Program::runTheveninCharge(int minChargeC)
{
    TheveninChargeStrategy::setVIB(ProgramData::currentProgramData.getVoltage(ProgramData::VCharge),
            ProgramData::currentProgramData.battery.Ic, false);
    TheveninChargeStrategy::setMinI(ProgramData::currentProgramData.battery.Ic/minChargeC);
    Strategy::strategy = &TheveninChargeStrategy::vtable;
    return Strategy::doStrategy();
}

Strategy::statusType Program::runTheveninChargeBalance()
{
    TheveninChargeStrategy::setVIB(ProgramData::currentProgramData.getVoltage(ProgramData::VCharge),
            ProgramData::currentProgramData.battery.Ic, true);
    Strategy::strategy = &TheveninChargeStrategy::vtable;
    return Strategy::doStrategy();
}


Strategy::statusType Program::runDeltaCharge()
{
    Strategy::strategy = &DeltaChargeStrategy::vtable;
    return Strategy::doStrategy();
}

Strategy::statusType Program::runDischarge()
{
    AnalogInputs::ValueType Voff = ProgramData::currentProgramData.getVoltage(ProgramData::VDischarge);
    if(ProgramData::currentProgramData.isLiXX()) {
        Voff += settings.dischargeOffset_LiXX_ * ProgramData::currentProgramData.battery.cells;
    }
    TheveninDischargeStrategy::setVI(Voff, ProgramData::currentProgramData.battery.Id);
    Strategy::strategy = &TheveninDischargeStrategy::vtable;
    return Strategy::doStrategy();
}

Strategy::statusType Program::runBalance()
{
    Strategy::strategy = &Balancer::vtable;
    return Strategy::doStrategy();
}

void Program::run(ProgramType prog)
{
    programType_ = prog;
    stopReason_ = NULL;

    programState_ = Info;
    SerialLog::powerOn();
    AnalogInputs::powerOn();


    if(startInfo()) {
        programState_ = InProgress;

        Monitor::powerOn();
        Strategy::exitImmediately = false;
        Buzzer::soundStartProgram();

        enum ProgramType {
            Calibrate,
            Charge, ChargeBalance, Balance, Discharge, FastCharge,
            Storage, StorageBalance, DischargeChargeCycle,
            EditBattery
        };

        switch(programType_) {
        case Program::Charge:
            if(ProgramData::currentProgramData.isNiXX()) {
                runDeltaCharge();
            } else {
                runTheveninCharge(settings.Lixx_Imin_);   //(default end current = start current / 10)
            }
            break;
        case Program::ChargeBalance:
            runTheveninChargeBalance();
            break;
        case Program::Balance:
            runBalance();
            break;
        case Program::Discharge:
            runDischarge();
            break;
        case Program::FastCharge:
            runTheveninCharge(5);
            break;
        case Program::Storage:
            runStorage(false);
            break;
        case Program::StorageBalance:
            runStorage(true);
            break;
        case Program::DischargeChargeCycle:
            if (settings.forceBalancePort_) {
                ProgramDCcycle::runDCcycle(ProgramDCcycle::LiXX);
            } else {
                Screen::runNeedForceBalance();
            }
            break;
/*        case Program::DCcycleNiXX:
            ProgramDCcycle::runDCcycle(ProgramDCcycle::NiXX);
            break;
        case Program::DCcyclePb:
            ProgramDCcycle::runDCcycle(ProgramDCcycle::Pb);
            break;
*/        default:
            //TODO:
            Screen::runNotImplemented();
            break;
        }
        Monitor::powerOff();
    }
    AnalogInputs::powerOff();
    SerialLog::powerOff();
}

Strategy::statusType Program::runDCRestTime()
{
    DelayStrategy::setDelay(settings.DCRestTime_);
    Strategy::strategy = &DelayStrategy::vtable;
    return Strategy::doStrategy();
}
