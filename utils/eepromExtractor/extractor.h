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
#ifndef EEXTRACTOR_H_
#define EEXTRACTOR_H_

class Extractor {
public:
    virtual std::string getName() const = 0;
    virtual int getSize() const = 0;
    virtual void load(const void *, int size) = 0;
    virtual bool isValid() const = 0;
    virtual std::string toString() const = 0;
//  virtual void fromString(const std::string &from) = 0;
};

#endif //EEXTRACTOR_H_
