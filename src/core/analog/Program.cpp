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

namespace Program {
    ProgramType programType_;
    ProgramState programState_ = Program::Done;
    const char * stopReason_;

    bool startInfo();

    Strategy::statusType runStorage(bool balance);
    Strategy::statusType runTheveninCharge(int minChargeC);
    Strategy::statusType runDischarge();
    Strategy::statusType runTheveninChargeBalance();
    Strategy::statusType runBalance();
    Strategy::statusType runDeltaCharge();

} //namespace Program

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

Strategy::statusType Program::runWithoutInfo(ProgramType prog)
{
    switch(prog) {
    case Program::Charge:
        if(ProgramData::currentProgramData.isNiXX()) {
            return runDeltaCharge();
        } else {
            return runTheveninCharge(settings.minIoutDiv_);   //(default end current = start current / 10)
        }
    case Program::ChargeBalance:
        return runTheveninChargeBalance();
    case Program::Balance:
        return runBalance();
    case Program::Discharge:
        return runDischarge();
    case Program::FastCharge:
        return runTheveninCharge(5);
    case Program::Storage:
        return runStorage(false);
    case Program::StorageBalance:
        return runStorage(true);
    case Program::DischargeChargeCycle:
        return ProgramDCcycle::runDCcycle();
    default:
        //TODO:
        Screen::runNotImplemented();
        return Strategy::ERROR;
    }
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

        runWithoutInfo(programType_);

        Monitor::powerOff();
    }
    AnalogInputs::powerOff();
    SerialLog::powerOff();
}
