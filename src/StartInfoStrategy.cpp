#include "Hardware.h"
#include "StartInfoStrategy.h"
#include "ProgramData.h"
#include "Screen.h"
#include "Buzzer.h"


StartInfoStrategy startInfoStrategy;

void StartInfoStrategy::powerOn()
{
    hardware::setBatteryOutput(true);
    analogInputs.powerOn();
    screen.startBlinkOn(7);
    buzzer.begin();
    ok_ = 3;
}

void StartInfoStrategy::powerOff()
{
    analogInputs.powerOff();
    hardware::setBatteryOutput(false);
    screen.stopBlink();
    buzzer.soundOff();
}

Strategy::statusType StartInfoStrategy::doStrategy()
{
    bool c,b,v;

    c = b = false;
    v = (!analogInputs.isConnected(AnalogInputs::Vout));

    if(balancePort_) {
        uint8_t is_cells, should_be_cells;
        is_cells = analogInputs.getConnectedBalancePorts();
        should_be_cells = ProgramData::currentProgramData.battery.cells;
        c = (should_be_cells != is_cells);
        b = (is_cells == 0);

        if(should_be_cells == 1 && is_cells == 0)  {
            //one cell
            c = false;
            b = false;
        }
    }

    screen.blinkIndex_ = 7;
    if(c)     screen.blinkIndex_ -= 4;
    if(b)     screen.blinkIndex_ -= 2;
    if(v)     screen.blinkIndex_ -= 1;

    if(c || b || v) {
        buzzer.soundInfo();
    } else {
        buzzer.soundOff();
    }

    if(keyboard.getPressed() == BUTTON_NONE)
        ok_ = 0;
    if(!c && !b && !v && keyboard.getPressed() == BUTTON_START) {
        ok_++;
    }
    if(ok_ == 2) {
        buzzer.soundStartProgram();
        return Strategy::COMPLETE_AND_EXIT;
    }
    return Strategy::RUNNING;
}
