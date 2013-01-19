#include "EditName.h"
#include "LcdPrint.h"
#include "Hardware.h"
#include "Buzzer.h"


void EditName::mainScreen()
{
    lcdSetCursor0_0();
    lcdPrint_P(printStringP_);
    lcdSetCursor0_1();
    lcdPrint(name_, size_);
}


void EditName::incSL()
{
    if(cursorSL_< LCD_COLUMNS*LCD_LINES -1) cursorSL_++;
    else beginSL_++;
}
void EditName::decSL()
{
    if(cursorSL_> 0) cursorSL_--;
    else beginSL_--;
}
uint8_t EditName::getSL()
{
    return beginSL_ + cursorSL_;
}

void EditName::selectLetterScreen()
{
    uint8_t begin = beginSL_;
    for(uint8_t j=0;j<LCD_LINES;j++) {
        lcdSetCursor(0,j);
        for(uint8_t i=0;i<LCD_COLUMNS;i++)
            lcdPrintChar((char)begin++);
    }
}

void EditName::selectLetter()
{
    uint8_t key, begin=-1;
    lcd.blink();
    do {
        if(begin != beginSL_) {
            selectLetterScreen();
            begin = beginSL_;
        }
        lcdSetCursor(cursorSL_&(LCD_COLUMNS-1), cursorSL_ >> 4);
        key = keyboard.getPressedWithSpeed();
        switch(key) {
        case BUTTON_STOP: return;
        case BUTTON_INC: incSL(); break;
        case BUTTON_DEC: decSL(); break;
        }
        if(key == BUTTON_STOP)
            return;
    } while(key != BUTTON_START);

    name_[cursor_] = getSL();
}

EditName::EditName(char * name, int size, const char *printStringP)
{
        name_ = name;
        size_ = size;
        printStringP_ = printStringP;
        cursor_ = 0;
        beginSL_ = ' ';
        cursorSL_ = 0;
};



bool EditName::run(){
    lcd.blink();
    uint8_t key;
    do {
        lcdClear();
        mainScreen();
        do {
            lcdSetCursor(cursor_, 1);
            key = keyboard.getPressedWithSpeed();
            switch(key) {
            case BUTTON_STOP:
                lcd.noBlink();
                return false;
            case BUTTON_INC:
                if(cursor_ < size_ - 1) cursor_++;
                break;
            case BUTTON_DEC:
                if (cursor_ > 0) cursor_--;
                break;
            }
        } while(key != BUTTON_START);

        if(keyboard.getPressTime()>1) {
            save();
            lcd.noBlink();
            return true;
        }

        selectLetter();

    } while(1);
}

void EditName::save()
{
    buzzer.soundSelect();
}
