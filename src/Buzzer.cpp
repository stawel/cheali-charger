#include "Hardware.h"
#include "Buzzer.h"



Buzzer buzzer;


void Buzzer::begin()
{
    begin_time_ = timer.getInterrupts();
}


void Buzzer::doInterrupt()
{

    uint32_t v = getSound(timer.getInterrupts() - begin_time_);
    v *= volume_;
    v /= 100;
    uint16_t v2 = v;
    if(v2 != last_value_) {
        last_value_ = v2;
        hardware::setBuzzer(last_value_);
    }
}

uint16_t Buzzer::getSound(uint16_t time)
{
    switch(sound_) {
    case Keyboard:             return getKeyboard(time);
    case Info:                 return getInfo(time);
    case Save:                 return getSave(time);
    case Select:             return getSelect(time);
    case ProgramComplete:     return getProgramComplete(time);
    case StartProgram:         return getStartProgram(time);
    case ReversedPolarity:     return getReversedPolarity(time);
    case Error:             return getError(time);
    case Off:
    default:
                    return getOff(time);
    }
}


uint16_t wave(int time, int size)
{
    if(time > 2*size)
        return 0;
    if(time > size)
        return 2*size - time;
    if(time > 0)
        return time;
    return 0;
}



uint16_t Buzzer::getOff(uint16_t time)
{
    return 0;
}
uint16_t Buzzer::setOff()
{
    soundOff();
    return 0;
}


uint16_t Buzzer::getKeyboard(uint16_t time)
{
    time/=2;
    if(time > 20)
        return setOff();
    return wave(time, 10);
}


uint16_t Buzzer::getSelect(uint16_t time)
{
    int v = time/4;
    if(v > 200)
        return setOff();
    return wave(v, 10) + wave(v-40, 10);
}


uint16_t Buzzer::getInfo(uint16_t time)
{
    uint16_t v = time%2000;
    return wave(v, 10);
}

uint16_t Buzzer::getSave(uint16_t time)
{
    uint16_t v = time/10;

    if(v > 140)
        return setOff();

    return wave(v, 20);
}

uint16_t Buzzer::getStartProgram(uint16_t time)
{
    return getSelect(time);
}


uint16_t Buzzer::getProgramComplete(uint16_t time)
{
    uint16_t val = time/10;
    uint16_t v = val%40;
    uint16_t t = val/40;

    if(t > 10)
        return setOff();
    return wave(v, 20);
}

uint16_t Buzzer::getReversedPolarity(uint16_t time)
{
    uint16_t val = time/10;
    uint16_t v = val%40;

    if(val > 80)
        return setOff();
    return wave(v, 20);
}

uint16_t Buzzer::getError(uint16_t time)
{
    uint16_t val = time/10;
    uint16_t v = val%40;
    uint16_t t = val/40;

    if(t > 10)
        return setOff();
    return wave(v, 20);
}

