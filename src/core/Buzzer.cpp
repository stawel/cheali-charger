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
#include "Buzzer.h"



namespace Buzzer {
    uint16_t begin_time_;
    uint16_t last_value_;
    SoundType sound_;


    uint8_t getKeyboard(uint16_t time);
    uint8_t getInfo(uint16_t time);
    uint8_t getSave(uint16_t time);
    uint8_t getSelect(uint16_t time);
    uint8_t getStartProgram(uint16_t time);
    uint8_t getProgramComplete(uint16_t time);
    uint8_t getReversedPolarity(uint16_t time);
    uint8_t getError(uint16_t time);
    uint8_t getOff(uint16_t time);

    uint8_t getSound(uint16_t time);
    uint8_t setOff();


} // namespace Buzzer


void Buzzer::sound(Buzzer::SoundType s)
{
      if(sound_ == s)
          return;
      sound_ = s;
      begin();
      Timer::delayIdle(50);
}

void Buzzer::soundIfSilence(Buzzer::SoundType s)
{
    if(sound_ == Off)
        sound(s);
}


void Buzzer::begin()
{
    begin_time_ = Timer::getInterrupts();
}


void Buzzer::doIdle()
{
    uint8_t v2 = getSound(Timer::getInterrupts() - begin_time_);
    if(v2 != last_value_) {
        last_value_ = v2;
        hardware::setBuzzer(last_value_);
    }
}

uint8_t Buzzer::getSound(uint16_t time)
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


uint8_t wave(int time, uint8_t size)
{
    if(time > 2*size)
        return 0;
    if(time > size)
        return 2*size - time;
    if(time > 0)
        return time;
    return 0;
}



uint8_t Buzzer::getOff(uint16_t time)
{
    return 0;
}
uint8_t Buzzer::setOff()
{
    soundOff();
    return 0;
}


uint8_t Buzzer::getKeyboard(uint16_t time)
{
    time/=2;
    if(time > 20)
        return setOff();
    return wave(time, 10);
}


uint8_t Buzzer::getSelect(uint16_t time)
{
    int v = time/4;
    if(v > 200)
        return setOff();
    return wave(v, 10) + wave(v-40, 10);
}


uint8_t Buzzer::getInfo(uint16_t time)
{
    uint16_t v = time%2000;
    return wave(v, 10);
}

uint8_t Buzzer::getSave(uint16_t time)
{
    uint16_t v = time/10;

    if(v > 140)
        return setOff();

    return wave(v, 20);
}

uint8_t Buzzer::getStartProgram(uint16_t time)
{
    return getSelect(time);
}


uint8_t Buzzer::getProgramComplete(uint16_t time)
{
    uint16_t val = time/10;
    uint16_t v = val%40;
    uint16_t t = val/40;

    if(t > 10)
        return setOff();
    return wave(v, 20);
}

uint8_t Buzzer::getReversedPolarity(uint16_t time)
{
    uint16_t val = time/10;
    uint16_t v = val%40;

    if(val > 80)
        return setOff();
    return wave(v, 20);
}

uint8_t Buzzer::getError(uint16_t time)
{
    uint16_t val = time/10;
    uint16_t v = val%40;
    uint16_t t = val/40;

    if(t > 10)
        return setOff();
    return wave(v, 20);
}

