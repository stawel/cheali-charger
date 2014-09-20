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
#ifndef VERSION_H_
#define VERSION_H_

#include "Utils.h"

#define CHEALI_CHARGER_VERSION                          0.33
#define CHEALI_CHARGER_EEPROM_CALIBRATION_VERSION       2
#define CHEALI_CHARGER_EEPROM_PROGRAMDATA_VERSION       2
#define CHEALI_CHARGER_EEPROM_SETTINGS_VERSION          4

#define CHEALI_CHARGER_VERSION_STRING           CHEALI_CHARGER_STRING(CHEALI_CHARGER_VERSION)
#define CHEALI_CHARGER_EPPROM_VERSION_STRING    \
                CHEALI_CHARGER_STRING(CHEALI_CHARGER_EEPROM_CALIBRATION_VERSION) "."\
                CHEALI_CHARGER_STRING(CHEALI_CHARGER_EEPROM_PROGRAMDATA_VERSION) "."\
                CHEALI_CHARGER_STRING(CHEALI_CHARGER_EEPROM_SETTINGS_VERSION) \

#endif /* VERSION_H_ */
