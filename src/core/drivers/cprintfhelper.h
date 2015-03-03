/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2015  Paweł Stawicki. All right reserved.

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
#ifndef CPRINTF_HELPER_H_
#define CPRINTF_HELPER_H_

#include "cprintf.h"

#include "AnalogInputs.h"
#include "AnalogInputsPrivate.h"
#define CPRINTF_ANALOG_REAL_V(name, size) {CP_TYPE_V, size, &AnalogInputs::real_[AnalogInputs::name]}


#define CPRINTF_METHOD(method)      {CP_TYPE_METHOD, 0, {.methodPtr = method}}
#define CPRINTF_STR(string)      {CP_TYPE_STRING, 0, {.charPtr = string_ ## string}}
#define CPRINTF_END                 {CP_TYPE_END, 0, 0}
#define CPRINTF_NEWLINE             {CP_TYPE_NEWLINE, 0, 0}
#define CPRINTF_VOLT(value, size)   {CP_TYPE_V, size, &value}



#define CPRINTF_1(x) CPRINTF_ ## x
#define CPRINTF_2(x, args...) CPRINTF_ ## x, CPRINTF_1(args)
#define CPRINTF_3(x, args...) CPRINTF_ ## x, CPRINTF_2(args)
#define CPRINTF_4(x, args...) CPRINTF_ ## x, CPRINTF_3(args)
#define CPRINTF_5(x, args...) CPRINTF_ ## x, CPRINTF_4(args)
#define CPRINTF_6(x, args...) CPRINTF_ ## x, CPRINTF_5(args)
#define CPRINTF_7(x, args...) CPRINTF_ ## x, CPRINTF_6(args)
#define CPRINTF_8(x, args...) CPRINTF_ ## x, CPRINTF_7(args)
#define CPRINTF_9(x, args...) CPRINTF_ ## x, CPRINTF_8(args)

#define _NUM_ARGS2(X,X64,X63,X62,X61,X60,X59,X58,X57,X56,X55,X54,X53,X52,X51,X50,X49,X48,X47,X46,X45,X44,X43,X42,X41,X40,X39,X38,X37,X36,X35,X34,X33,X32,X31,X30,X29,X28,X27,X26,X25,X24,X23,X22,X21,X20,X19,X18,X17,X16,X15,X14,X13,X12,X11,X10,X9,X8,X7,X6,X5,X4,X3,X2,X1,N,...) N
#define NUM_ARGS(...) _NUM_ARGS2(0, __VA_ARGS__ ,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

#define CPRINTF_N2(N, args...) CPRINTF_ ## N(args)
#define CPRINTF_N(N, args...) CPRINTF_N2(N, args)
#define CPRINTF(args...) do { static const cprintf::PrintData p[] PROGMEM = { CPRINTF_N(NUM_ARGS(args), args)}; \
                                cprintf::cprintf(p); } while(0)






#endif /* CPRINTF_HELPER_H_ */
