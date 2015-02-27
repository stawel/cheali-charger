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
#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef ENABLE_DEBUG

#include "SerialLog.h"
#include "Time.h"
#include "memory.h"

namespace debug {
    inline void print(char x) { SerialLog::printChar(x); }
    inline void print(uint8_t x) { SerialLog::printUInt(x); }
    inline void print(uint16_t x) { SerialLog::printUInt(x); }
    inline void print(int16_t x) { SerialLog::printInt(x); }
    inline void print(uint32_t x) { SerialLog::printUInt(x); }
    inline void print(const char * x) { SerialLog::printString(x); }
    inline void print(void * x) { SerialLog::printULong((long)x); }
    inline void begin() { print('D'); print(Time::getMiliseconds()); print(':'); };
    inline void end() { SerialLog::printNL(); }
};

#define LogDebug_();
#define LogDebug_1(x) debug::print(x);
#define LogDebug_2(x, args...) debug::print(x); LogDebug_1(args);
#define LogDebug_3(x, args...) debug::print(x); LogDebug_2(args);
#define LogDebug_4(x, args...) debug::print(x); LogDebug_3(args);
#define LogDebug_5(x, args...) debug::print(x); LogDebug_4(args);
#define LogDebug_6(x, args...) debug::print(x); LogDebug_5(args);
#define LogDebug_7(x, args...) debug::print(x); LogDebug_6(args);
#define LogDebug_8(x, args...) debug::print(x); LogDebug_7(args);
#define LogDebug_9(x, args...) debug::print(x); LogDebug_8(args);
#define LogDebug_10(x, args...) debug::print(x); LogDebug_9(args);
#define LogDebug_11(x, args...) debug::print(x); LogDebug_10(args);

#define _NUM_ARGS2(X,X64,X63,X62,X61,X60,X59,X58,X57,X56,X55,X54,X53,X52,X51,X50,X49,X48,X47,X46,X45,X44,X43,X42,X41,X40,X39,X38,X37,X36,X35,X34,X33,X32,X31,X30,X29,X28,X27,X26,X25,X24,X23,X22,X21,X20,X19,X18,X17,X16,X15,X14,X13,X12,X11,X10,X9,X8,X7,X6,X5,X4,X3,X2,X1,N,...) N
#define NUM_ARGS(...) _NUM_ARGS2(0, __VA_ARGS__ ,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

#define LogDebug_N2(N, args...) LogDebug_ ## N(args)
#define LogDebug_N(N, args...) LogDebug_N2(N, args)
#define LogDebug(args...) do { debug::begin(); LogDebug_N(NUM_ARGS(args), args); debug::end(); } while(0)
#else

#define LogDebug(...)

#endif

#endif /* DEBUG_H_ */
