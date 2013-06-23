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

class Buzzer {
public:
    enum SoundType { Keyboard, Info, Save, Select, StartProgram, ProgramComplete, Error, ReversedPolarity, Off };
    Buzzer(): sound_(Off){}
    uint16_t begin_time_;
    uint16_t last_value_;

    SoundType sound_;


    void soundKeyboard()                     {sound_ = Keyboard;         begin(); }
    void soundInfo()                        {if(sound_ != Info)            {sound_ = Info;      begin(); }}
    void soundSave()                        {sound_ = Save;             begin(); }
    void soundSelect()                    {sound_ = Select;             begin(); }
    void soundProgramComplete()             {sound_ = ProgramComplete;     begin(); }
    void soundStartProgram()                 {sound_ = StartProgram;     begin(); }
    void soundReversedPolarity()            {if(sound_ != ReversedPolarity)        {sound_ = ReversedPolarity;     begin(); }}
    void soundError()                         {if(sound_ != Error)        {sound_ = Error;     begin(); }}
    void soundOff()                        {sound_ = Off; }
    void begin();

    void doInterrupt();
private:
    uint16_t getKeyboard(uint16_t time);
    uint16_t getInfo(uint16_t time);
    uint16_t getSave(uint16_t time);
    uint16_t getSelect(uint16_t time);
    uint16_t getStartProgram(uint16_t time);
    uint16_t getProgramComplete(uint16_t time);
    uint16_t getReversedPolarity(uint16_t time);
    uint16_t getError(uint16_t time);
    uint16_t getOff(uint16_t time);

    uint16_t getSound(uint16_t time);
    uint16_t setOff();
};

extern Buzzer buzzer;

#endif /* BUZZER_H_ */
