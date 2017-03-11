/*
    cheali-charger - open source firmware for a variety of LiPo chargers
    Copyright (C) 2016  Paweł Stawicki. All right reserved.

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
#include <inttypes.h>
#include "Hardware.h"
#include "LcdPrint.h"
#include "Menu.h"
#include "Blink.h"
#include "Utils.h"
#include "memory.h"

namespace Menu {

    uint8_t index_;
    uint8_t begin_;
    uint8_t size_;
    bool render_;
    bool waitRelease_;
    PrintMethod printMethod_;
    EditMethod editMethod_;
    const struct StaticMenu * staticMenu_;


    void printStatic(uint8_t i) {
            const char * string;
            pgm::read(string, &staticMenu_[i].string);
            lcdPrint_P(string);
    }

    void initialize(uint8_t size) {
        index_ = 0;
        begin_ = 0;
        size_ = size;
        render_ = true;
        waitRelease_ = true;
    }


    uint8_t countStaticElements() {
        uint8_t retu = 0;
        while(pgm::read(&staticMenu_[retu].string)) retu++;
        return retu;
    }

    static void initializeStatic(const struct StaticMenu * staticMenu) {
        staticMenu_ = staticMenu;
        initialize(countStaticElements());
        printMethod_ = printStatic;
    }

    int8_t runStatic(const struct StaticMenu * staticMenu) {
        int8_t i = 0;
        do {
            initializeStatic(staticMenu);
            setIndex(i);
            i = run();
            if(i >= 0) {
                callVoidMethod_P(&staticMenu_[i].call);
            }
        } while(i >= 0);
        return i;
    }

    void incIndex() {
        index_++;
    }

    void decIndex() {
        if(index_ > 0) {
            index_ --;
        }
    }

    void checkBegin(){
        const uint8_t lines = LCD_LINES;
        if(index_ >= size_) {
            index_ = size_ -1;
        }

        if(begin_ > index_){
            begin_ = index_;
        }
        if(begin_ + lines <= index_) {
            begin_ = index_ - lines + 1;
        }
    }

    void display() {
        checkBegin();
        for(uint8_t i = begin_; i < begin_ + LCD_LINES; i++) {
            lcdSetCursor(0, i - begin_);
            lcdPrintChar(i == index_ ? '>' : ' ');
            if(i < size_) printMethod_(i);
            lcdPrintSpaces();
        }
        render_ = false;
    }

    uint8_t run_() {
        uint8_t button = Keyboard::getPressedWithDelay();

        if(PolarityCheck::runReversedPolarityInfo()) {
            render_ = true;
            return POLARITY_CHECK_REVERSED_POLARITY;
        }

        switch (button) {
        case BUTTON_INC:
            incIndex();
            render_ = true;
            break;
        case BUTTON_DEC:
            decIndex();
            render_ = true;
            break;
        }

        if(render_)
            display();

        return button;
    }

    int8_t run(bool alwaysRefresh) {
        uint8_t key;
        render_ = true;
        do {
            key = run_();
            if(key == BUTTON_NONE) waitRelease_ = false;

            if(!waitRelease_ && key == BUTTON_START)  {
                waitRelease_ = true;
                return getIndex();
            }
            if(alwaysRefresh) render_ = true;
        } while(key != BUTTON_STOP || waitRelease_);
        return MENU_EXIT;
    }

    bool runEdit()
    {
        Blink::startBlinkOff(getIndex());
        uint8_t key;
        render_ = true;
        do {
            key =  Keyboard::getPressedWithDelay();
            if(key == BUTTON_DEC || key == BUTTON_INC) {
                editMethod_(getIndex(), key);
                Blink::startBlinkOn(getIndex());
                render_ = true;
            } else if(key == BUTTON_STOP || key == BUTTON_START) {
                break;
            }
            if(Blink::getBlinkChanged())
                render_ = true;
            if(render_)
                display();
            Blink::incBlinkTime();
        } while(true);

        Blink::stopBlink();
        waitRelease_ = true;
        render_ = true;
        return key == BUTTON_START;
    }


}
