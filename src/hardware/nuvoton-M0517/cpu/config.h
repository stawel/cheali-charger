/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2016 Paweł Stawicki. All right reserved.

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
#ifndef CPU_CONFIG_H_
#define CPU_CONFIG_H_

#define CHEALI_CHARGER_ARCHITECTURE_CPU         0x4000
#define CHEALI_CHARGER_ARCHITECTURE_CPU_STRING  "nuvoton-M0517"

//#define CHEALI_EEPROM_PACKED
#define CHEALI_EEPROM_PACKED __attribute__((packed))

#endif /* CPU_CONFIG_H_ */
