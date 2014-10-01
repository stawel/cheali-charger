/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2014  Paweł Stawicki. All right reserved.

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
#include "ProgramMenus.h"
#include "StaticMenu.h"
#include "eeprom.h"

// Program selection depending on the battery type
using namespace ProgramMenus;

namespace {

    const char * const programLiXXMenu[] PROGMEM = {
            string_charge,
            string_chargeAndBalance,
            string_balance,
            string_discharge,
            string_fastCharge,
            string_storage,
            string_storageAndBalance,
            string_dcCycle,
            string_editBattery,
            NULL
    };

    const Program::ProgramType programLiXXMenuType[] PROGMEM = {
            Program::ChargeLiXX,
            Program::ChargeLiXX_Balance,
            Program::Balance,
            Program::DischargeLiXX,
            Program::FastChargeLiXX,
            Program::StorageLiXX,
            Program::StorageLiXX_Balance,
            Program::DCcycleLiXX,
            Program::EditBattery
    };

    const char * const programNiZnMenu[] PROGMEM = {
            string_charge,
            string_chargeAndBalance,
            string_balance,
            string_discharge,
            string_fastCharge,
            string_dcCycle,
            string_editBattery,
            NULL
    };

    const Program::ProgramType programNiZnMenuType[] PROGMEM = {
            Program::ChargeLiXX,
            Program::ChargeLiXX_Balance,
            Program::Balance,
            Program::DischargeLiXX,
            Program::FastChargeLiXX,   //TODO: check
            Program::DCcycleLiXX,      //TODO: check
            Program::EditBattery
    };


    const char * const programNiXXMenu[] PROGMEM = {
            string_charge,
            string_discharge,
            string_dcCycle,
            string_editBattery,
            NULL
    };

    const Program::ProgramType programNiXXMenuType[] PROGMEM = {
            Program::ChargeNiXX,
            Program::DischargeNiXX,
            Program::DCcycleNiXX,
            Program::EditBattery
    };

    const char * const programPbMenu[] PROGMEM = {
            string_charge,
            string_discharge,
            string_fastCharge,
            string_dcCycle,
            string_editBattery,
            NULL
    };

    const Program::ProgramType programPbMenuType[] PROGMEM = {
            Program::ChargePb,
            Program::DischargePb,
            Program::FastChargePb,   //TODO: check
            Program::DCcyclePb,      //TODO: check
            Program::EditBattery
    };


    StaticMenu selectLiXXMenu(programLiXXMenu);
    StaticMenu selectNiXXMenu(programNiXXMenu);
    StaticMenu selectNiZnMenu(programNiZnMenu);
    StaticMenu selectPbMenu(programPbMenu);

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

void ProgramMenus::selectProgram(int index)
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
                eeprom::restoreProgramDataCRC();
                selectPrograms = getSelectProgramMenu();
            } else {
                Program::run(prog);
            }
        }
    } while(menuIndex >= 0);
}
