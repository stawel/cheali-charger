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
#include "Hardware.h"
#include "StartInfoStrategy.h"
#include "ProgramData.h"
#include "Screen.h"
#include "memory.h"
#include "Settings.h"
#include "Program.h"
#include "Utils.h"

namespace StartInfoStrategy {
    uint8_t ok_;

    const Strategy::VTable vtable PROGMEM = {
        powerOn,
        powerOff,
        doStrategy
    };

}


void StartInfoStrategy::powerOn()
{
    Screen::blink.startBlinkOn(7);
    Buzzer::begin();
    ok_ = 3;
}

void StartInfoStrategy::powerOff()
{
    Screen::blink.stopBlink();
    Buzzer::soundOff();
}

Strategy::statusType StartInfoStrategy::doStrategy()
{
    bool cell_nr, v_balance, v_out, balance;
    uint8_t is_cells, should_be_cells;

    cell_nr = v_balance = false;
    v_out = ! AnalogInputs::isConnected(AnalogInputs::Vout);

    if(ProgramData::battery.type == ProgramData::Unknown || ProgramData::battery.type == ProgramData::LED) {
        v_out = false;
    }

    is_cells = AnalogInputs::getConnectedBalancePortsCount();

    if(Strategy::doBalance) {
        should_be_cells = ProgramData::battery.cells;
        cell_nr = (should_be_cells != is_cells);
        v_balance = (is_cells == 0);

        if(should_be_cells == 1 && is_cells == 0)  {
            //one cell
            cell_nr =   false;
            v_balance = false;
        }
    }
    if(AnalogInputs::isBalancePortConnected() &&
            absDiff(AnalogInputs::getRealValue(AnalogInputs::Vout),
               AnalogInputs::getRealValue(AnalogInputs::Vbalancer)) > ANALOG_VOLT(0.5)) v_out = true;

    Screen::blink.blinkIndex_ = 7;
    if(cell_nr)     Screen::blink.blinkIndex_ -= 4;
    if(v_balance)   Screen::blink.blinkIndex_ -= 2;
    if(v_out)       Screen::blink.blinkIndex_ -= 1;

    if(cell_nr || v_balance || v_out) {
        Buzzer::soundInfo();
    } else {
        Buzzer::soundOff();
    }

    balance = (v_balance || cell_nr) && (is_cells != 0);

    if(Keyboard::getLast() == BUTTON_NONE)
        ok_ = 0;
    if(!balance && !v_out && Keyboard::getLast() == BUTTON_START) {
        ok_++;
    }
    if(ok_ == 2) {
        return Strategy::COMPLETE;
    }
    return Strategy::RUNNING;
}
