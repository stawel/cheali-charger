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
#include "ProgramDataMenu.h"

// Program selection depending on the battery type

namespace ProgramMenus {

    const Program::ProgramType programNoneMenu[] PROGMEM = {
            Program::EditBattery,
    };

    const Program::ProgramType programLiXXMenu[] PROGMEM = {
            Program::Charge,
            Program::ChargeBalance,
            Program::Balance,
            Program::Discharge,
            Program::FastCharge,
            Program::Storage,
            Program::StorageBalance,
#ifdef ENABLE_PROGRAM_MENUS_LIXX_CYCLING
            Program::DischargeChargeCycle,
#endif
            Program::CapacityCheck,
            Program::EditBattery,
    };

    const Program::ProgramType programNiZnMenu[] PROGMEM = {
            Program::Charge,
            Program::ChargeBalance,
            Program::Balance,
            Program::Discharge,
            Program::FastCharge,
#ifdef ENABLE_PROGRAM_MENUS_NIZN_CYCLING
            Program::DischargeChargeCycle,
#endif
            Program::CapacityCheck,
            Program::EditBattery,
    };

    const Program::ProgramType programNiXXMenu[] PROGMEM = {
            Program::Charge,
            Program::Discharge,
            Program::DischargeChargeCycle,
            Program::CapacityCheck,
            Program::EditBattery,
    };

    const Program::ProgramType programPbMenu[] PROGMEM = {
            Program::Charge,
            Program::Discharge,
            Program::FastCharge,
            Program::DischargeChargeCycle,
            Program::CapacityCheck,
            Program::EditBattery,
    };

    const Program::ProgramType programLEDMenu[] PROGMEM = {
            Program::Charge,
            Program::EditBattery,
    };

// ProgramType -> strings
    const char * const programMenus_strings[] PROGMEM = {
            string_charge,
            string_chargeAndBalance,
            string_balance,
            string_discharge,
            string_fastCharge,
            string_storage,
            string_storageAndBalance,
            string_dcCycle,
            string_capacityCheck,
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
            STATIC_ASSERT(sizeOfArray(programMenus_strings)-1 == Program::EditBattery);

            lcdPrint_P(programMenus_strings, getProgramType(i));
        }

        static uint8_t countElements(const Program::ProgramType * typeMenu) {
            uint8_t retu = 0;
            while(pgm::read(&typeMenu[retu++]) != Program::EditBattery);
            return retu;
        }
    };

    ProgramTypeMenu selectNoneMenu(programNoneMenu);
    ProgramTypeMenu selectLiXXMenu(programLiXXMenu);
    ProgramTypeMenu selectNiXXMenu(programNiXXMenu);
    ProgramTypeMenu selectNiZnMenu(programNiZnMenu);
    ProgramTypeMenu selectPbMenu(programPbMenu);
    ProgramTypeMenu selectLEDMenu(programLEDMenu);

    ProgramTypeMenu * getSelectProgramMenu() {
        STATIC_ASSERT(ProgramData::LAST_BATTERY_CLASS == 6);

        ProgramData::BatteryClass bc = ProgramData::getBatteryClass();
        if(ProgramData::battery.type == ProgramData::None)
            return &selectNoneMenu;
        if(bc == ProgramData::ClassNiZn)
            return &selectNiZnMenu;
        if(bc == ProgramData::ClassLiXX)
            return &selectLiXXMenu;
        if(bc == ProgramData::ClassNiXX)
            return &selectNiXXMenu;
        if(bc == ProgramData::ClassLED)
            return &selectLEDMenu;
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
                ProgramDataMenu::run();
                ProgramData::saveProgramData(index);
                selectPrograms = getSelectProgramMenu();
            } else {
                Program::run(prog);
            }
        }
    } while(menuIndex >= 0);
}
