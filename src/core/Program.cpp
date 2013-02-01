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
#include "SimpleCharge.h"
#include "TheveninCharge.h"
#include "TheveninDischarge.h"
#include "DeltaChargeStrategy.h"
#include "Monitor.h"
#include "Storage.h"
#include "memory.h"
#include "StartInfoStrategy.h"
#include "Buzzer.h"
#include "StaticMenu.h"
#include "Settings.h"



AnalogInputs::Name Program::iName_;
Program::ProgramType Program::programType_;
const char * Program::stopReason_;

namespace {
    const Screen::ScreenType deltaChargeScreens[] PROGMEM = {
      Screen::ScreenFirst,
      Screen::ScreenDeltaFirst,
      Screen::ScreenDebugRthVth,
      Screen::ScreenDeltaVout,
      Screen::ScreenDeltaTextern,
      Screen::ScreenDebugDelta,
      Screen::ScreenR,
      Screen::ScreenVout,
      Screen::ScreenVinput,
      Screen::ScreenDebugI,
      Screen::ScreenTime,
      Screen::ScreenTemperature,
      Screen::ScreenCIVlimits
    };
    const Screen::ScreenType NiXXDischargeScreens[] PROGMEM = {
      Screen::ScreenFirst,
      Screen::ScreenDebugRthVth,
      Screen::ScreenDeltaTextern,
      Screen::ScreenDebugDelta,
      Screen::ScreenR,
      Screen::ScreenVout,
      Screen::ScreenVinput,
      Screen::ScreenDebugI,
      Screen::ScreenTime,
      Screen::ScreenTemperature,
      Screen::ScreenCIVlimits
    };

    const Screen::ScreenType theveninScreens[] PROGMEM = {
      Screen::ScreenFirst,
      Screen::ScreenDebugRthVth,
      Screen::ScreenBalancer0_2,            Screen::ScreenBalancer3_5,
      Screen::ScreenBalancer0_2Rth,         Screen::ScreenBalancer3_5Rth,
      Screen::ScreenDebugBalancer0_2RthV,   Screen::ScreenDebugBalancer3_5RthV,
      Screen::ScreenDebugBalancer0_2RthI,   Screen::ScreenDebugBalancer3_5RthI,
      Screen::ScreenR,
      Screen::ScreenVout,
      Screen::ScreenVinput,
      Screen::ScreenDebugI,
      Screen::ScreenTime,
      Screen::ScreenTemperature,
      Screen::ScreenCIVlimits
    };
    const Screen::ScreenType balanceScreens[] PROGMEM = {
      Screen::ScreenBalancer0_2,            Screen::ScreenBalancer3_5,
//      Thevenin values are not evaluated when balancing
//      Screen::ScreenBalancer0_2Rth,         Screen::ScreenBalancer3_5Rth,
//      Screen::ScreenDebugBalancer0_2M,      Screen::ScreenDebugBalancer3_5M,
      Screen::ScreenTime,
      Screen::ScreenTemperature };
    const Screen::ScreenType dischargeScreens[] PROGMEM = {
      Screen::ScreenFirst,
      Screen::ScreenDebugRthVth,
      Screen::ScreenBalancer0_2,            Screen::ScreenBalancer3_5,
      Screen::ScreenBalancer0_2Rth,         Screen::ScreenBalancer3_5Rth,
      Screen::ScreenDebugBalancer0_2RthV,   Screen::ScreenDebugBalancer3_5RthV,
      Screen::ScreenDebugBalancer0_2RthI,   Screen::ScreenDebugBalancer3_5RthI,
      Screen::ScreenR,
      Screen::ScreenVout,
      Screen::ScreenVinput,
      Screen::ScreenDebugI,
      Screen::ScreenTime,
      Screen::ScreenTemperature,
      Screen::ScreenCIVlimits
    };
    const Screen::ScreenType storageScreens[] PROGMEM = {
      Screen::ScreenFirst,
      Screen::ScreenDebugRthVth,
      Screen::ScreenBalancer0_2,            Screen::ScreenBalancer3_5,
      Screen::ScreenBalancer0_2Rth,         Screen::ScreenBalancer3_5Rth,
      Screen::ScreenDebugBalancer0_2RthV,   Screen::ScreenDebugBalancer3_5RthV,
      Screen::ScreenDebugBalancer0_2RthI,   Screen::ScreenDebugBalancer3_5RthI,
      Screen::ScreenR,
      Screen::ScreenVout,
      Screen::ScreenVinput,
      Screen::ScreenDebugI,
      Screen::ScreenTime,
      Screen::ScreenTemperature,
      Screen::ScreenCIVlimits
    };

    const Screen::ScreenType startInfoBalanceScreens[] PROGMEM = {
      Screen::ScreenStartInfo,
      Screen::ScreenBalancer0_2,            Screen::ScreenBalancer3_5,
      Screen::ScreenTemperature };

    const Screen::ScreenType startInfoScreens[] PROGMEM = {
      Screen::ScreenStartInfo,
      Screen::ScreenTemperature };

    void chargingComplete() {
        lcdClear();
        screen.displayScreenProgramCompleted();
        buzzer.soundProgramComplete();
        waitButtonPressed();
        buzzer.soundOff();
    }

    void chargingMonitorError() {
        lcdClear();
        screen.displayMonitorError();
        buzzer.soundError();
        waitButtonPressed();
        buzzer.soundOff();
    }

    bool analizeStrategyStatus(Strategy &strategy,  Strategy::statusType status, bool exitImmediately) {
        bool run = true;
        if(status == Strategy::ERROR) {
            screen.powerOff();
            strategy.powerOff();
            chargingMonitorError();
            run = false;
        }

        if(status == Strategy::COMPLETE) {
            screen.powerOff();
            strategy.powerOff();
            if(!exitImmediately)
                chargingComplete();
            run = false;
        }
        return run;
    }

    Strategy::statusType doStrategy(Strategy &strategy, const Screen::ScreenType chargeScreens[]
                                                      , uint8_t screen_limit, bool exitImmediately = false)
    {
        uint8_t key;
        bool run = true;
        uint16_t newMesurmentData = 0;
        Strategy::statusType status = Strategy::RUNNING;
        strategy.powerOn();
        screen.powerOn();
        monitor.powerOn();
        lcdClear();
        uint8_t screen_nr = 0;
        screen_limit--;
        do {
            if(!PolarityCheck::runReversedPolarityInfo())
                screen.display(pgm::read(&chargeScreens[screen_nr]));

            {
                //change displayed screen
                key =  keyboard.getPressedWithSpeed();
                int8_t step = 0;
                if(key == BUTTON_INC && screen_nr < screen_limit)   step++;
                if(key == BUTTON_DEC && screen_nr > 0)              step--;
                do{
                    screen_nr+=step;
                } while(!settings.isDebug() && pgm::read(&chargeScreens[screen_nr]) & Screen::Debug);
            }
            if(run) {
                status = monitor.run();
                run = analizeStrategyStatus(strategy, status, exitImmediately);

                if(run && newMesurmentData != analogInputs.getCalculationCount()) {
                    newMesurmentData = analogInputs.getCalculationCount();
                    status = strategy.doStrategy();
                    run = analizeStrategyStatus(strategy, status, exitImmediately);
                }
            }
            if(!run && exitImmediately)
                return status;
        } while(key != BUTTON_STOP);

        screen.powerOff();
        strategy.powerOff();
        return status;
    }

} //namespace {

bool Program::startInfo()
{
    if(ProgramData::currentProgramData.isLiXX()) {
        //usues balance port
        startInfoStrategy.setBalancePort(true);
        return doStrategy(startInfoStrategy, startInfoBalanceScreens, sizeOfArray(startInfoBalanceScreens), true) == Strategy::COMPLETE;
    } else {
        startInfoStrategy.setBalancePort(false);
        return doStrategy(startInfoStrategy, startInfoScreens, sizeOfArray(startInfoScreens), true) == Strategy::COMPLETE;
    }
}

void Program::runStorage(bool balance)
{
    storage.setDoBalance(balance);
    storage.setVII(ProgramData::currentProgramData.getVoltage(ProgramData::VStorage),
            ProgramData::currentProgramData.battery.Ic, ProgramData::currentProgramData.battery.Id);
    doStrategy(storage, storageScreens, sizeOfArray(storageScreens));
}
void Program::runTheveninCharge(int minChargeC)
{
    theveninCharge.setVI(ProgramData::currentProgramData.getVoltage(ProgramData::VCharge), ProgramData::currentProgramData.battery.Ic);
    theveninCharge.setMinI(ProgramData::currentProgramData.battery.Ic/minChargeC);
    doStrategy(theveninCharge, theveninScreens, sizeOfArray(theveninScreens));
}

void Program::runDeltaCharge()
{
    deltaChargeStrategy.setTestTV(settings.externT_, true);
    doStrategy(deltaChargeStrategy, deltaChargeScreens, sizeOfArray(deltaChargeScreens));
}

void Program::runDischarge()
{
    theveninDischarge.setVI(ProgramData::currentProgramData.getVoltage(ProgramData::VDischarge), ProgramData::currentProgramData.battery.Id);
    doStrategy(theveninDischarge, dischargeScreens, sizeOfArray(dischargeScreens));
}

void Program::runNiXXDischarge()
{
    theveninDischarge.setVI(ProgramData::currentProgramData.getVoltage(ProgramData::VDischarge), ProgramData::currentProgramData.battery.Id);
    doStrategy(theveninDischarge, NiXXDischargeScreens, sizeOfArray(NiXXDischargeScreens));
}

void Program::runBalance()
{
    doStrategy(balancer, balanceScreens, sizeOfArray(balanceScreens));
}

void Program::run(ProgramType prog)
{
    programType_ = prog;
    stopReason_ = PSTR("");

    if(startInfo()) {
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
    StaticMenu selectPbMenu(programPbMenu, sizeOfArray(programPbMenu));

    StaticMenu * getSelectProgramMenu() {
        if(ProgramData::currentProgramData.isLiXX())
            return &selectLiXXMenu;
        else if(ProgramData::currentProgramData.isNiXX())
            return &selectNiXXMenu;
        else return &selectPbMenu;
    }
    Program::ProgramType getSelectProgramType(int index) {
        const Program::ProgramType * address;
        if(ProgramData::currentProgramData.isLiXX())
            address = &programLiXXMenuType[index];
        else if(ProgramData::currentProgramData.isNiXX())
            address = &programNiXXMenuType[index];
        else address = &programPbMenuType[index];
        return pgm::read(address);
    }
}

void Program::selectProgram(int index)
{
    uint8_t key;
    bool release = true;
    ProgramData::loadProgramData(index);
    StaticMenu * selectPrograms = getSelectProgramMenu();
    selectPrograms->render();

    do {
        key = selectPrograms->run();

        if(key == BUTTON_NONE)
            release = false;

        if(!release && key == BUTTON_START)  {
            int i = selectPrograms->getIndex();
            Program::ProgramType prog = getSelectProgramType(i);
            switch(prog) {
            case Program::EditBattery:
                if(ProgramData::currentProgramData.edit(index)) {
                    buzzer.soundSave();
                    ProgramData::saveProgramData(index);
                    selectPrograms = getSelectProgramMenu();
                }
                break;
            default:
                Program::run(prog);
                break;
            }
            selectPrograms->render();
            release = true;
        }
    } while(key != BUTTON_STOP || release);
}




