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
#ifndef BLINK_H
#define BLINK_H


#define BLINK_SPEED_TIME 2

class Blink {
public:
    Blink():blinkIndex_(-1){}

    bool getBlinkOff() const;
    bool getBlinkChanged() const;

    int8_t getBlinkIndex() const    {
        if(getBlinkOff()) {
            return blinkIndex_;
        }
        return -1;
    }

    void startBlinkOn(int8_t index) {
        blinkIndex_ = index;
        blinkTime_ = getBlinkTime();
    }
    void startBlinkOff(int8_t index) {
        blinkIndex_ = index;
        blinkTime_ = 0;
    }

    void stopBlink() {    blinkIndex_ = -1; }
    void incBlinkTime() { blinkTime_++; }
    uint8_t getBlinkTime() const { return BLINK_SPEED_TIME; }

    int8_t blinkIndex_;
    uint8_t blinkTime_;
};

#endif /* BLINK_H */
