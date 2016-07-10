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
#include "eeprom.h"
#include "Menu.h"
#include "Utils.h"
#include "LcdPrint.h"
#include "ProgramDataMenu.h"
#include "memory.h"

// Program selection, depends on the battery type

namespace ProgramMenus {

    const Program::ProgramType * currentProgramMenu_;
    int8_t programMenuIndex_ = 0;


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

    inline const Program::ProgramType * getSelectProgramMenu() {
        STATIC_ASSERT(ProgramData::LAST_BATTERY_CLASS == 6);

        ProgramData::BatteryClass bc = ProgramData::getBatteryClass();
        if(ProgramData::battery.type == ProgramData::None)
            return programNoneMenu;
        if(bc == ProgramData::ClassNiZn)
            return programNiZnMenu;
        if(bc == ProgramData::ClassLiXX)
            return programLiXXMenu;
        if(bc == ProgramData::ClassNiXX)
            return programNiXXMenu;
        if(bc == ProgramData::ClassLED)
            return programLEDMenu;
        else return programPbMenu;
    }

    static uint8_t countElements() {
        uint8_t retu = 0;
        while(pgm::read(&currentProgramMenu_[retu++]) != Program::EditBattery);
        return retu;
    }

    inline Program::ProgramType getProgramType(uint8_t i) {
        return pgm::read(&currentProgramMenu_[i]);
    }


    void printItem(uint8_t i) {
        STATIC_ASSERT(sizeOfArray(programMenus_strings)-1 == Program::EditBattery);

        lcdPrint_P(programMenus_strings, getProgramType(i));
    }


    static void selectProgramMenu() {
        currentProgramMenu_ = getSelectProgramMenu();
        Menu::initialize(countElements());
        Menu::printMethod_ = printItem;
    }
}

void ProgramMenus::selectProgram(uint8_t index)
{
    int8_t i;
    ProgramData::loadProgramData(index);
    while(true) {
        selectProgramMenu();

        Menu::setIndex(programMenuIndex_);
        i = Menu::run();
        programMenuIndex_ = Menu::getIndex();
        if(i < 0) {
            break;
        } else {
            Program::ProgramType prog = getProgramType(programMenuIndex_);
            if(prog == Program::EditBattery) {
                ProgramDataMenu::run();
                ProgramData::saveProgramData(index);
            } else {
                Program::run(prog);
            }
        }
    }
}
