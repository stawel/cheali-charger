/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2014  Paweł Stawicki. All right reserved.

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
#ifndef SCREEN_BALANCER_H_
#define SCREEN_BALANCER_H_


namespace Screen { namespace Balancer {

    void displayVoltage1_3();
    void displayVoltage4_6();
    void displayVoltage7_9();

    void displayResistance1_3();
    void displayResistance4_6();
    void displayResistance7_9();

} };

#endif /* SCREEN_BALANCER_H_ */
