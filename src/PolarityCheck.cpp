#include "PolarityCheck.h"
#include "Hardware.h"
#include "Screen.h"
#include "Buzzer.h"


bool PolarityCheck::isReversedPolarity_ = false;

bool PolarityCheck::runReversedPolarityInfo()
{
    bool reversed = analogInputs.isReversePolarity();
    if(reversed) {
        if(!isReversedPolarity_)
            Screen::displayScreenReversedPolarity();

        buzzer.soundReversedPolarity();
    }
    isReversedPolarity_ = reversed;
    return reversed;
}
