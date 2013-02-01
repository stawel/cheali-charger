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
#ifndef SETTINGSMENU_H_
#define SETTINGSMENU_H_

#include "Settings.h"
#include "EditMenu.h"

class SettingsMenu : public EditMenu {
public:
    SettingsMenu(const Settings &p);
    virtual uint8_t printItem(uint8_t i);
    virtual void editItem(uint8_t i, uint8_t key);

    bool run();

    Settings p_;
private:

    void printViewType() const;
    void changeBacklight(int dir);
    void changeViewType(int dir);

    static void printTemp(AnalogInputs::ValueType t);
    static void printDeltaT(AnalogInputs::ValueType dt);
    static void printVolt(AnalogInputs::ValueType v);
    static void changeTemp(AnalogInputs::ValueType &v, int dir);
    static void changeDeltaTemp(AnalogInputs::ValueType &v, int dir);
    static void changeVolt(AnalogInputs::ValueType &v, int dir);
};

#endif /* SETTINGSMENU_H_ */
