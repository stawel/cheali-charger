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

    const Program::ProgramType programLiXXMenu[] PROGMEM = {
            Program::Charge,
            Program::ChargeBalance,
            Program::Balance,
            Program::Discharge,
            Program::FastCharge,
            Program::Storage,
            Program::StorageBalance,
            Program::DischargeChargeCycle,
            Program::EditBattery,
    };

    const Program::ProgramType programNiZnMenu[] PROGMEM = {
            Program::Charge,
            Program::ChargeBalance,
            Program::Balance,
            Program::Discharge,
            Program::FastCharge,                //TODO: check
            Program::DischargeChargeCycle,      //TODO: check
            Program::EditBattery,
    };

    const Program::ProgramType programNiXXMenu[] PROGMEM = {
            Program::Charge,
            Program::Discharge,
            Program::DischargeChargeCycle,
            Program::EditBattery,
    };

    const Program::ProgramType programPbMenu[] PROGMEM = {
            Program::Charge,
            Program::Discharge,
            Program::FastCharge,                //TODO: check
            Program::DischargeChargeCycle,      //TODO: check
            Program::EditBattery,
    };


// ProgramType -> strings
    const char * const programMenus_strings[] PROGMEM = {
            NULL,
            string_charge,
            string_chargeAndBalance,
            string_balance,
            string_discharge,
            string_fastCharge,
            string_storage,
            string_storageAndBalance,
            string_dcCycle,
            string_editBattery,
    };

    class ProgramTypeMenu : public Menu {
    public:
        const Program::ProgramType * typeMenu_;
        ProgramTypeMenu(const Program::ProgramType  *typeMenu) :
            Menu(countElements(typeMenu)), typeMenu_(typeMenu){};

        Program::ProgramType getProgramType(uint8_t i) {
            return pgm::read(&typeMenu_[i]);
        }

        virtual void printItem(uint8_t i) {
            STATIC_ASSERT(sizeOfArray(programMenus_strings) == Program::LAST_PROGRAM_TYPE);

            lcdPrint_P(pgm::read(&programMenus_strings[getProgramType(i)]));
        }

        static uint8_t countElements(const Program::ProgramType * typeMenu) {
            uint8_t retu = 0;
            while(pgm::read(&typeMenu[retu++]) != Program::EditBattery);
            return retu;
        }
    };

    ProgramTypeMenu selectLiXXMenu(programLiXXMenu);
    ProgramTypeMenu selectNiXXMenu(programNiXXMenu);
    ProgramTypeMenu selectNiZnMenu(programNiZnMenu);
    ProgramTypeMenu selectPbMenu(programPbMenu);

    ProgramTypeMenu * getSelectProgramMenu() {
        if(ProgramData::currentProgramData.battery.type == ProgramData::NiZn)
            return &selectNiZnMenu;
        if(ProgramData::currentProgramData.isLiXX())
            return &selectLiXXMenu;
        else if(ProgramData::currentProgramData.isNiXX())
            return &selectNiXXMenu;
        else return &selectPbMenu;
    }
}

void ProgramMenus::selectProgram(int index)
{
    ProgramData::loadProgramData(index);
    ProgramTypeMenu * selectPrograms = getSelectProgramMenu();
    int8_t menuIndex;
    do {
        menuIndex = selectPrograms->runSimple();
        if(menuIndex >= 0)  {
            Program::ProgramType prog = selectPrograms->getProgramType(menuIndex);
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
