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
#include "Buzzer.h"


StartInfoStrategy startInfoStrategy;

void StartInfoStrategy::powerOn()
{
    discharger.powerOn();
    screen.startBlinkOn(7);
    Buzzer::begin();
    ok_ = 3;
}

void StartInfoStrategy::powerOff()
{
    discharger.powerOff();
    screen.stopBlink();
    Buzzer::soundOff();
}

Strategy::statusType StartInfoStrategy::doStrategy()
{
    bool cell_nr, v_balance, v_out;

    cell_nr = v_balance = false;
    v_out = (!analogInputs.isConnected(AnalogInputs::Vout));

    if(balancePort_) {
        uint8_t is_cells, should_be_cells;
        is_cells = analogInputs.getConnectedBalancePorts();
        should_be_cells = ProgramData::currentProgramData.battery.cells;
        cell_nr = (should_be_cells != is_cells);
        v_balance = (is_cells == 0);

        if(should_be_cells == 1 && is_cells == 0)  {
            //one cell
            cell_nr =   false;
            v_balance = false;
        }
    }
    if(analogInputs.isConnected(AnalogInputs::Vbalancer) &&
            absDiff(analogInputs.getRealValue(AnalogInputs::Vout),
               analogInputs.getRealValue(AnalogInputs::Vbalancer)) > ANALOG_VOLT(0.5)) v_out = true;

    screen.blinkIndex_ = 7;
    if(cell_nr)     screen.blinkIndex_ -= 4;
    if(v_balance)   screen.blinkIndex_ -= 2;
    if(v_out)       screen.blinkIndex_ -= 1;

    if(cell_nr || v_balance || v_out) {
        Buzzer::soundInfo();
    } else {
        Buzzer::soundOff();
    }

    if(keyboard.getPressed() == BUTTON_NONE)
        ok_ = 0;
    if(!cell_nr && !v_balance && !v_out && keyboard.getPressed() == BUTTON_START) {
        ok_++;
    }
    if(ok_ == 2) {
        return Strategy::COMPLETE;
    }
    return Strategy::RUNNING;
}
