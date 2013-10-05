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


Program::ProgramType Program::programType_;
Program::ProgramState Program::programState_ = Program::Done;
const char * Program::stopReason_;

namespace {
    const Screen::ScreenType deltaChargeScreens[] PROGMEM = {
      Screen::ScreenFirst,
      Screen::ScreenDeltaFirst,
      Screen::ScreenDeltaVout,
      Screen::ScreenDeltaTextern,
      Screen::ScreenR,
      Screen::ScreenVout,
      Screen::ScreenVinput,
      Screen::ScreenTime,
      Screen::ScreenTemperature,
      Screen::ScreenCIVlimits,
      Screen::ScreenEnd
    };
    const Screen::ScreenType NiXXDischargeScreens[] PROGMEM = {
      Screen::ScreenFirst,
      Screen::ScreenDeltaTextern,
      Screen::ScreenR,
      Screen::ScreenVout,
      Screen::ScreenVinput,
      Screen::ScreenTime,
      Screen::ScreenTemperature,
      Screen::ScreenCIVlimits,
      Screen::ScreenEnd
    };

    const Screen::ScreenType theveninScreens[] PROGMEM = {
      Screen::ScreenFirst,
      Screen::ScreenBalancer1_3,            Screen::ScreenBalancer4_6,
      Screen::ScreenBalancer1_3Rth,         Screen::ScreenBalancer4_6Rth,
      Screen::ScreenR,
      Screen::ScreenVout,
      Screen::ScreenVinput,
      Screen::ScreenTime,
      Screen::ScreenTemperature,
      Screen::ScreenCIVlimits,
      Screen::ScreenEnd

    };
    const Screen::ScreenType balanceScreens[] PROGMEM = {
      Screen::ScreenBalancer1_3,            Screen::ScreenBalancer4_6,
      Screen::ScreenTime,
      Screen::ScreenTemperature,
      Screen::ScreenEnd
    };
    const Screen::ScreenType dischargeScreens[] PROGMEM = {
      Screen::ScreenFirst,
      Screen::ScreenBalancer1_3,            Screen::ScreenBalancer4_6,
      Screen::ScreenBalancer1_3Rth,         Screen::ScreenBalancer4_6Rth,
      Screen::ScreenR,
      Screen::ScreenVout,
      Screen::ScreenVinput,
      Screen::ScreenTime,
      Screen::ScreenTemperature,
      Screen::ScreenCIVlimits,
      Screen::ScreenEnd

    };
    const Screen::ScreenType storageScreens[] PROGMEM = {
      Screen::ScreenFirst,
      Screen::ScreenBalancer1_3,            Screen::ScreenBalancer4_6,
      Screen::ScreenBalancer1_3Rth,         Screen::ScreenBalancer4_6Rth,
      Screen::ScreenR,
      Screen::ScreenVout,
      Screen::ScreenVinput,
      Screen::ScreenTime,
      Screen::ScreenTemperature,
      Screen::ScreenCIVlimits,
      Screen::ScreenEnd

    };

    const Screen::ScreenType startInfoBalanceScreens[] PROGMEM = {
      Screen::ScreenStartInfo,
      Screen::ScreenBalancer1_3,            Screen::ScreenBalancer4_6,
      Screen::ScreenTemperature,
      Screen::ScreenEnd
    };

    const Screen::ScreenType startInfoScreens[] PROGMEM = {
      Screen::ScreenStartInfo,
      Screen::ScreenTemperature,
      Screen::ScreenEnd
    };

    Strategy::statusType doStrategy(const Screen::ScreenType chargeScreens[], bool exitImmediately = false){

        Strategy::statusType status = Strategy::doStrategy(chargeScreens, exitImmediately);
        Program::programState_ = Program::Done;
        if(status == Strategy::ERROR) {
            Program::programState_ = Program::Error;
        }
        return status;
    }


} //namespace {

bool Program::startInfo()
{
    bool balancer = false;
    const Screen::ScreenType * screen = startInfoScreens;
    Strategy::strategy_ = &StartInfoStrategy::vtable;
    if(ProgramData::currentProgramData.isLiXX()) {
        //usues balance port
        balancer = true;
        screen = startInfoBalanceScreens;
    }
    StartInfoStrategy::setBalancePort(balancer);
    programState_ = Info;
    return doStrategy(startInfoBalanceScreens, true) == Strategy::COMPLETE;
}

Strategy::statusType Program::runStorage(bool balance)
{
    StorageStrategy::setDoBalance(balance);
    StorageStrategy::setVII(ProgramData::currentProgramData.getVoltage(ProgramData::VStorage),
            ProgramData::currentProgramData.battery.Ic, ProgramData::currentProgramData.battery.Id);
    Strategy::strategy_ = &StorageStrategy::vtable;
    programState_ = Storage;
    return doStrategy(storageScreens);
}
Strategy::statusType Program::runTheveninCharge(int minChargeC)
{
    TheveninChargeStrategy::setVIB(ProgramData::currentProgramData.getVoltage(ProgramData::VCharge),
            ProgramData::currentProgramData.battery.Ic, false);
    TheveninChargeStrategy::setMinI(ProgramData::currentProgramData.battery.Ic/minChargeC);
    Strategy::strategy_ = &TheveninChargeStrategy::vtable;
    programState_ = Charging;
    return doStrategy(theveninScreens);
}

Strategy::statusType Program::runTheveninChargeBalance()
{
    TheveninChargeStrategy::setVIB(ProgramData::currentProgramData.getVoltage(ProgramData::VCharge),
            ProgramData::currentProgramData.battery.Ic, true);
    Strategy::strategy_ = &TheveninChargeStrategy::vtable;
    programState_ = ChargingBalancing;
    return doStrategy(theveninScreens);
}


Strategy::statusType Program::runDeltaCharge()
{
    DeltaChargeStrategy::setTestTV(settings.externT_, true);
    Strategy::strategy_ = &DeltaChargeStrategy::vtable;
    programState_ = Charging;
    return doStrategy(deltaChargeScreens);
}

Strategy::statusType Program::runDischarge()
{
    TheveninDischargeStrategy::setVI(ProgramData::currentProgramData.getVoltage(ProgramData::VDischarge), ProgramData::currentProgramData.battery.Id);
    Strategy::strategy_ = &TheveninDischargeStrategy::vtable;
    programState_ = Discharging;
    return doStrategy(dischargeScreens);
}

Strategy::statusType Program::runNiXXDischarge()
{
    TheveninDischargeStrategy::setVI(ProgramData::currentProgramData.getVoltage(ProgramData::VDischarge), ProgramData::currentProgramData.battery.Id);
    Strategy::strategy_ = &TheveninDischargeStrategy::vtable;
    programState_ = Discharging;
    return doStrategy(NiXXDischargeScreens);
}

Strategy::statusType Program::runBalance()
{
    Strategy::strategy_ = &Balancer::vtable;
    programState_ = Balancing;
    return doStrategy(balanceScreens);
}

void Program::run(ProgramType prog)
{
    programType_ = prog;
    stopReason_ = PSTR("");

    if(startInfo()) {
        Buzzer::soundStartProgram();

        switch(prog) {
        case Program::ChargeLiXX:
            runTheveninCharge(10);
            break;
        case Program::Balance:
            runBalance();
            break;
        case Program::DischargeLiXX:
            runDischarge();
            break;
        case Program::FastChargeLiXX:
            runTheveninCharge(5);
            break;
        case Program::StorageLiXX:
            runStorage(false);
        case Program::StorageLiXX_Balance:
            runStorage(true);
            break;
        case Program::ChargeNiXX:
            runDeltaCharge();
            break;
        case Program::DischargeNiXX:
            runNiXXDischarge();
            break;
        case Program::ChargeLiXX_Balance:
            runTheveninChargeBalance();
            break;
        default:
            //TODO:
            Screen::runNotImplemented();
            break;
        }
    }
}



// Program selection depending on the battery type

namespace {

    const char charge_str[] PROGMEM = "charge";
    const char chaBal_str[] PROGMEM = "charge+balance";
    const char balanc_str[] PROGMEM = "balance";
    const char discha_str[] PROGMEM = "discharge";
    const char fastCh_str[] PROGMEM = "fast charge";
    const char storag_str[] PROGMEM = "storage";
    const char stoBal_str[] PROGMEM = "storage+balanc";
    const char CDcycl_str[] PROGMEM = "c>d cycle";
    const char DCcycl_str[] PROGMEM = "d>c cycle";
    const char edBatt_str[] PROGMEM = "edit battery";

    const char * const programLiXXMenu[] PROGMEM =
    { charge_str,
      chaBal_str,
      balanc_str,
      discha_str,
      fastCh_str,
      storag_str,
      stoBal_str,
      edBatt_str
    };

    const Program::ProgramType programLiXXMenuType[] PROGMEM =
    { Program::ChargeLiXX,
      Program::ChargeLiXX_Balance,
      Program::Balance,
      Program::DischargeLiXX,
      Program::FastChargeLiXX,
      Program::StorageLiXX,
      Program::StorageLiXX_Balance,
      Program::EditBattery
    };

    const char * const programNiZnMenu[] PROGMEM =
    { charge_str,
      chaBal_str,
      balanc_str,
      discha_str,
      fastCh_str,
      edBatt_str
    };

    const Program::ProgramType programNiZnMenuType[] PROGMEM =
    { Program::ChargeLiXX,
      Program::ChargeLiXX_Balance,
      Program::Balance,
      Program::DischargeLiXX,
      Program::FastChargeLiXX,
      Program::EditBattery
    };


    const char * const programNiXXMenu[] PROGMEM =
    { charge_str,
      discha_str,
      CDcycl_str,
      DCcycl_str,
      edBatt_str
    };

    const Program::ProgramType programNiXXMenuType[] PROGMEM =
    { Program::ChargeNiXX,
      Program::DischargeNiXX,
      Program::CDcycleNiXX,
      Program::DCcycleNiXX,
      Program::EditBattery
    };

    const char * const programPbMenu[] PROGMEM =
    { charge_str,
      discha_str,
      edBatt_str
    };

    const Program::ProgramType programPbMenuType[] PROGMEM =
    { Program::ChargePb,
      Program::DischargePb,
      Program::EditBattery
    };


    StaticMenu selectLiXXMenu(programLiXXMenu, sizeOfArray(programLiXXMenu));
    StaticMenu selectNiXXMenu(programNiXXMenu, sizeOfArray(programNiXXMenu));
    StaticMenu selectNiZnMenu(programNiZnMenu, sizeOfArray(programNiZnMenu));
    StaticMenu selectPbMenu(programPbMenu, sizeOfArray(programPbMenu));

    StaticMenu * getSelectProgramMenu() {
        if(ProgramData::currentProgramData.battery.type == ProgramData::NiZn)
            return &selectNiZnMenu;
        if(ProgramData::currentProgramData.isLiXX())
            return &selectLiXXMenu;
        else if(ProgramData::currentProgramData.isNiXX())
            return &selectNiXXMenu;
        else return &selectPbMenu;
    }
    Program::ProgramType getSelectProgramType(uint8_t index) {
        const Program::ProgramType * address;
        if(ProgramData::currentProgramData.battery.type == ProgramData::NiZn)
            address = &programNiZnMenuType[index];
        else if(ProgramData::currentProgramData.isLiXX())
            address = &programLiXXMenuType[index];
        else if(ProgramData::currentProgramData.isNiXX())
            address = &programNiXXMenuType[index];
        else address = &programPbMenuType[index];
        return pgm::read(address);
    }
}

void Program::selectProgram(int index)
{
    ProgramData::loadProgramData(index);
    StaticMenu * selectPrograms = getSelectProgramMenu();
    int8_t menuIndex;
    do {
        menuIndex = selectPrograms->runSimple();
        if(menuIndex >= 0)  {
            Program::ProgramType prog = getSelectProgramType(menuIndex);
            if(prog == Program::EditBattery) {
                ProgramData::currentProgramData.edit(index);
                ProgramData::saveProgramData(index);
                selectPrograms = getSelectProgramMenu();
            } else {
                Program::run(prog);
            }
        }
    } while(menuIndex >= 0);
}




