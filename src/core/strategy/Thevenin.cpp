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

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <stdlib.h>

#include "Thevenin.h"
#include "Balancer.h"
#include "Strategy.h"
#include "Utils.h"

namespace Thevenin {

    //Thevenin data is stored in T
    //T[0 .. MAX_BANANCE_CELLS-1]   - Thevenin data per cell
    //T[MAX_BANANCE_CELLS]          - Thevenin data for Vout
    struct Thevenin T[MAX_BANANCE_CELLS + 1];

    static AnalogInputs::ValueType calculateI(uint8_t idx, AnalogInputs::ValueType Vc);
    static void init(uint8_t idx, AnalogInputs::ValueType Vth,AnalogInputs::ValueType Vmax, AnalogInputs::ValueType i, bool charge);
    static void calculateRthVth(uint8_t idx, AnalogInputs::ValueType v, AnalogInputs::ValueType i);
    static void calculateRth(uint8_t idx, AnalogInputs::ValueType v, AnalogInputs::ValueType i);
    static void calculateVth(uint8_t idx, AnalogInputs::ValueType v, AnalogInputs::ValueType i);
    static void storeLast(uint8_t idx, AnalogInputs::ValueType VLast, AnalogInputs::ValueType ILast);

    AnalogInputs::ValueType getV(uint8_t idx);
    AnalogInputs::ValueType getEndV(uint8_t idx);
}  // namespace Thevenin

AnalogInputs::ValueType Thevenin::getReadableRth(int16_t iV, uint16_t uI)
{
    if(uI == 0)
        return 0;
    uint32_t R = abs(iV);
    R *= ANALOG_VOLT(1.0);
    R/=uI;
    return R;
}

AnalogInputs::ValueType Thevenin::getReadableRth(uint8_t idx)
{
    return getReadableRth(T[idx].Rth.iV, T[idx].Rth.uI);
}

AnalogInputs::ValueType Thevenin::calculateI(uint8_t idx, AnalogInputs::ValueType v)
{
    int32_t i;
    i  = v;
    i -= T[idx].Vth_;
    i *= T[idx].Rth.uI;
    if(T[idx].Rth.iV == 0) return UINT16_MAX;
    i /= T[idx].Rth.iV;
    if(i >  UINT16_MAX) return  UINT16_MAX;
    if(i < 0) return 0;
    return i;
}

AnalogInputs::ValueType Thevenin::getV(uint8_t idx)
{
    if(idx >= MAX_BANANCE_CELLS) {
        return AnalogInputs::getVbattery();
    } else {
        return Balancer::getPresumedV(idx);
    }
}

AnalogInputs::ValueType Thevenin::getEndV(uint8_t idx)
{
    if(idx >= MAX_BANANCE_CELLS) {
        return Strategy::endV;
    } else {
        return Strategy::endVperCell;
    }
}

AnalogInputs::ValueType Thevenin::calculateI()
{
    AnalogInputs::ValueType i = UINT16_MAX;
    uint16_t connected = Balancer::connectedCells | (1<<MAX_BANANCE_CELLS);

    for(uint8_t c = 0; c < MAX_BANANCE_CELLS + 1; c++) {
        if(connected & 1) {
            i = min(i, calculateI(c, getEndV(c)));
        }
        connected >>=1;
    }
    return i;
}


void Thevenin::init(uint8_t idx, AnalogInputs::ValueType Vth,AnalogInputs::ValueType Vmax, AnalogInputs::ValueType i, bool charge)
{
    AnalogInputs::ValueType Vfrom, Vto;
    if(charge) {
        //safety routine - important when one cell is overcharged
        Vfrom = min(Vth, Vmax);
        Vto = max(Vth, Vmax);
    } else {
        Vfrom = max(Vth, Vmax);
        Vto = min(Vth, Vmax);
    }
    T[idx].VLast_ = T[idx].Vth_ = Vfrom;
    T[idx].ILastDiff_ = T[idx].ILast_ = 0;

    T[idx].Rth.uI = i;
    T[idx].Rth.iV = Vto;  T[idx].Rth.iV -= Vfrom;
}

void Thevenin::initialize(bool charge)
{
    for(uint8_t c = 0; c < MAX_BANANCE_CELLS + 1; c++) {
        init(c, getV(c), getEndV(c), Strategy::minI, charge);
    }
}

void Thevenin::calculateRth(uint8_t idx, AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
    if(absDiff(i, T[idx].ILast_) > T[idx].ILastDiff_/2) {
        int16_t rth_v;
        uint16_t rth_i;
        if(i > T[idx].ILast_) {
            rth_i  = i;
            rth_i -= T[idx].ILast_;
            rth_v  = v;
            rth_v -= T[idx].VLast_;
        } else {
            rth_v  = T[idx].VLast_;
            rth_v -= v;
            rth_i  = T[idx].ILast_;
            rth_i  -= i;
        }
        if(sign(rth_v) == sign(T[idx].Rth.iV)) {
            T[idx].ILastDiff_ = rth_i;
            T[idx].Rth.iV = rth_v;
            T[idx].Rth.uI = rth_i;
        }
    }
}

void Thevenin::calculateVth(uint8_t idx, AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
    int32_t VRth;
    VRth = i;
    VRth *= T[idx].Rth.iV;
    VRth /= T[idx].Rth.uI;
    if(v < VRth) T[idx].Vth_ = 0;
    else T[idx].Vth_ = v - VRth;
}

void Thevenin::calculateRthVth(uint8_t idx, AnalogInputs::ValueType v, AnalogInputs::ValueType i)
{
    calculateRth(idx, v, i);
    calculateVth(idx, v, i);
}


void Thevenin::calculateRthVth(AnalogInputs::ValueType I)
{
    uint16_t connected = Balancer::connectedCells | (1<<MAX_BANANCE_CELLS);

    for(uint8_t c = 0; c < MAX_BANANCE_CELLS + 1; c++) {
        if(connected & 1) {
            calculateRthVth(c, Balancer::getV(c), I);
        }
        connected >>= 1;
    }
}

inline void Thevenin::storeLast(uint8_t idx, AnalogInputs::ValueType VLast, AnalogInputs::ValueType ILast)
{
    T[idx].VLast_ = VLast;
    T[idx].ILast_ = ILast;
}


void Thevenin::storeI(AnalogInputs::ValueType I)
{
    for(uint8_t i = 0; i <= MAX_BANANCE_CELLS; i++) {
        storeLast(i, getV(i), I);
    }
}
