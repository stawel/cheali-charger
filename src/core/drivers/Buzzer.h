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
#ifndef BUZZER_H_
#define BUZZER_H_

namespace Buzzer {
    enum SoundType { Keyboard, Info, Save, Select, StartProgram, ProgramComplete, Error, ReversedPolarity, Off };

    void sound(SoundType s);
    void soundIfSilence(SoundType s);
    inline void soundKeyboard()             {soundIfSilence(Keyboard); }
    inline void soundInfo()                 {sound(Info); }
    inline void soundSave()                 {sound(Save); }
    inline void soundSelect()               {sound(Select); }
    inline void soundProgramComplete()      {sound(ProgramComplete); }
    inline void soundStartProgram()         {sound(StartProgram); }
    inline void soundReversedPolarity()     {sound(ReversedPolarity); }
    inline void soundError()                {sound(Error); }
    void soundOff();
    void begin();

    void doIdle();
};

#endif /* BUZZER_H_ */
