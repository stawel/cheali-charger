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
#include "Balancer.h"
#include "Monitor.h"
#include "memory.h"
#include "StartInfoStrategy.h"
#include "Buzzer.h"
#include "StaticMenu.h"
#include "Settings.h"
#include "SerialLog.h"
#include "DelayStrategy.h"
#include "ProgramDCcycle.h"
#include "Calibrate.h"

namespace Program {
    ProgramType programType;
    ProgramState programState = Program::Done;
    const char * stopReason;

    bool startInfo();

    void setupStorage();
    void setupTheveninCharge();
    void setupDischarge();
    void setupBalance();
    void setupDeltaCharge();
    void setupPowerSupplyCharge();
    void setupProgramType(ProgramType prog);

    void dischargeOutputCapacitor();

} //namespace Program

bool Program::startInfo()
{
    Screen::powerOn();
    Strategy::strategy = &StartInfoStrategy::vtable;
    Strategy::exitImmediately = true;
    Strategy::doBalance = false;
    if(ProgramData::isLiXX()) {
        //usues balance port
        Strategy::doBalance = true;
    }
    return Strategy::doStrategy() == Strategy::COMPLETE;
}

void Program::setupStorage()
{
    Strategy::strategy = &StorageStrategy::vtable;
}
void Program::setupTheveninCharge()
{
    Strategy::setVI(ProgramData::VCharged, true);
    Strategy::strategy = &TheveninChargeStrategy::vtable;
}

void Program::setupDeltaCharge()
{
    Strategy::setVI(ProgramData::VCharged, true);
    Strategy::strategy = &DeltaChargeStrategy::vtable;
}

void Program::setupDischarge()
{
    Strategy::setVI(ProgramData::VDischarged, false);
    Strategy::strategy = &TheveninDischargeStrategy::vtable;
}

void Program::setupPowerSupplyCharge()
{
    Strategy::setVI(ProgramData::VCharged, true);
    Strategy::strategy = &SimpleChargeStrategy::vtable;
}


void Program::setupBalance()
{
    Strategy::strategy = &Balancer::vtable;
}

void Program::setupProgramType(ProgramType prog) {
    Strategy::doBalance = false;

    switch(prog) {
    case Program::CapacityCheck:
    case Program::Charge:
        if(ProgramData::isNiXX()) {
            setupDeltaCharge();
        } else if (ProgramData::isPowerSupply()) {
            setupPowerSupplyCharge();
        } else {
            setupTheveninCharge();
        }
        break;
    case Program::ChargeBalance:
        Strategy::doBalance = true;
        setupTheveninCharge();
        break;
    case Program::Balance:
        setupBalance();
        break;
    case Program::DischargeChargeCycle:
    case Program::Discharge:
        setupDischarge();
        break;
    case Program::FastCharge:
        setupTheveninCharge();
        Strategy::minI = ProgramData::battery.Ic / 5;
        break;
    case Program::Storage:
        setupStorage();
        break;
    case Program::StorageBalance:
        Strategy::doBalance = true;
        setupStorage();
        break;
    default:
        break;
    }
}

void Program::resetAccumulatedMeasurements()
{
    Monitor::resetAccumulatedMeasurements();
    AnalogInputs::resetAccumulatedMeasurements();
}


Strategy::statusType Program::runWithoutInfo(ProgramType prog)
{
    resetAccumulatedMeasurements();

    setupProgramType(prog);
    switch(prog) {
        case Program::CapacityCheck:
            return ProgramDCcycle::runDCcycle(1, 3);
        case Program::DischargeChargeCycle:
            return ProgramDCcycle::runDCcycle(0, ProgramData::battery.DCcycles*2 - 1);
        default:
            return Strategy::doStrategy();
    }
}

void Program::dischargeOutputCapacitor()
{
    Discharger::powerOn();
    Discharger::trySetIout(DISCHARGE_OUTPUT_CAPACITOR_CURRENT);
    Time::delayDoIdle(100);
    Discharger::powerOff();
}


void Program::run(ProgramType prog)
{
    if(!Calibrate::check())
        return;

    dischargeOutputCapacitor();

    programType = prog;
    setupProgramType(prog);
    stopReason = NULL;

    programState = Info;
    SerialLog::powerOn();
    AnalogInputs::powerOn();

    if(startInfo()) {
        programState = InProgress;

        Monitor::powerOn();
        Screen::powerOn();

        Strategy::exitImmediately = false;
        Buzzer::soundStartProgram();

        runWithoutInfo(programType);

        Monitor::powerOff();
    }
    AnalogInputs::powerOff();
    SerialLog::powerOff();
    Screen::powerOff();
}
