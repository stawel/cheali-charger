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
#include "LiquidCrystal.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "IO.h"
#include "Utils.h"
#include "Hardware.h"

#ifndef DummyLiquidCrystal_h

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).


LiquidCrystal::LiquidCrystal()
{
  init();
}

void LiquidCrystal::init()
{

  IO::pinMode(LCD_RS_PIN, OUTPUT);
  // we can save 1 pin by not using RW. Indicate by passing 255 instead of pin#
#ifdef LCD_RW_PIN
  IO::pinMode(LCD_RW_PIN, OUTPUT);
#endif  //LCD_RW_PIN

  IO::pinMode(LCD_ENABLE_PIN, OUTPUT);
  
#ifndef LCD_ENABLE_8BITMODE
    _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
#else
    _displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
#endif //LCD_ENABLE_8BITMODE

  begin(16, 1);  
}

void LiquidCrystal::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }
  _numlines = lines;
  _currline = 0;

  // for some 1 line displays you can select a 10 pixel high font
  if ((dotsize != 0) && (lines == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  }

  IO::pinMode(LCD_D0_PIN, OUTPUT);
  IO::pinMode(LCD_D1_PIN, OUTPUT);
  IO::pinMode(LCD_D2_PIN, OUTPUT);
  IO::pinMode(LCD_D3_PIN, OUTPUT);

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
  Utils::delayMicroseconds(50000); 
  // Now we pull both RS and R/W low to begin commands
  IO::digitalWrite(LCD_RS_PIN, LOW);
  IO::digitalWrite(LCD_ENABLE_PIN, LOW);
#ifdef LCD_RW_PIN
    IO::digitalWrite(_rw_pin, LOW);
#endif //LCD_RW_PIN
  
  //put the LCD into 4 bit or 8 bit mode
#ifndef  LCD_ENABLE_8BITMODE
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    write4bits(0x03);
    Utils::delayMicroseconds(4500); // wait min 4.1ms

    // second try
    write4bits(0x03);
    Utils::delayMicroseconds(4500); // wait min 4.1ms
    
    // third go!
    write4bits(0x03); 
    Utils::delayMicroseconds(150);

    // finally, set to 4-bit interface
    write4bits(0x02); 
#else
    // this is according to the hitachi HD44780 datasheet
    // page 45 figure 23

    // Send function set command sequence
    command(LCD_FUNCTIONSET | _displayfunction);
    Utils::delayMicroseconds(4500);  // wait more than 4.1ms

    // second try
    command(LCD_FUNCTIONSET | _displayfunction);
    Utils::delayMicroseconds(150);

    // third go
    command(LCD_FUNCTIONSET | _displayfunction);
#endif //LCD_ENABLE_8BITMODE

  // finally, set # lines, font size, etc.
  command(LCD_FUNCTIONSET | _displayfunction);  

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
  display();

  // clear it off
  clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  command(LCD_ENTRYMODESET | _displaymode);

}

/********** high level commands, for the user! */
void LiquidCrystal::clear()
{
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  Utils::delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidCrystal::home()
{
  command(LCD_RETURNHOME);  // set cursor position to zero
  Utils::delayMicroseconds(2000);  // this command takes a long time!
}

void LiquidCrystal::setCursor(uint8_t col, uint8_t row)
{
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  if ( row >= _numlines ) {
    row = _numlines-1;    // we count rows starting w/0
  }
  
  command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void LiquidCrystal::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystal::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystal::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidCrystal::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystal::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystal::leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LiquidCrystal::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LiquidCrystal::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystal::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

/*********** mid level commands, for sending data/cmds */

inline void LiquidCrystal::command(uint8_t value) {
  send(value, LOW);
}

inline uint8_t LiquidCrystal::write(uint8_t value) {
  send(value, HIGH);
  return 1; // assume sucess
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void LiquidCrystal::send(uint8_t value, uint8_t mode) {
  IO::digitalWrite(LCD_RS_PIN, mode);

  // if there is a RW pin indicated, set it low to Write
#ifdef LCD_RW_PIN
    IO::digitalWrite(LCD_RW_PIN, LOW);
#endif
  
#ifdef LCD_ENABLE_8BITMODE
    write8bits(value); 
#else
    write4bits(value>>4);
    write4bits(value);
#endif //LCD_ENABLE_8BITMODE
}

void LiquidCrystal::pulseEnable(void) {
  IO::digitalWrite(LCD_ENABLE_PIN, LOW);
  Utils::delayMicroseconds(1);    
  IO::digitalWrite(LCD_ENABLE_PIN, HIGH);
  Utils::delayMicroseconds(1);    // enable pulse must be >450ns
  IO::digitalWrite(LCD_ENABLE_PIN, LOW);
  Utils::delayMicroseconds(100);   // commands need > 37us to settle
}

void LiquidCrystal::write4bits(uint8_t value) {
  IO::digitalWrite(LCD_D0_PIN, value & 1);
  IO::digitalWrite(LCD_D1_PIN, value & 2);
  IO::digitalWrite(LCD_D2_PIN, value & 4);
  IO::digitalWrite(LCD_D3_PIN, value & 8);
  pulseEnable();
}

void LiquidCrystal::write8bits(uint8_t value) {
#ifdef LCD_ENABLE_8BITMODE
  IO::pinMode(LCD_D0_PIN, OUTPUT);
  IO::pinMode(LCD_D1_PIN, OUTPUT);
  IO::pinMode(LCD_D2_PIN, OUTPUT);
  IO::pinMode(LCD_D3_PIN, OUTPUT);
  IO::pinMode(LCD_D4_PIN, OUTPUT);
  IO::pinMode(LCD_D5_PIN, OUTPUT);
  IO::pinMode(LCD_D6_PIN, OUTPUT);
  IO::pinMode(LCD_D7_PIN, OUTPUT);
  IO::digitalWrite(LCD_D0_PIN, value & 1);
  IO::digitalWrite(LCD_D1_PIN, value & 2);
  IO::digitalWrite(LCD_D2_PIN, value & 4);
  IO::digitalWrite(LCD_D3_PIN, value & 8);
  IO::digitalWrite(LCD_D0_PIN, value & 16);
  IO::digitalWrite(LCD_D1_PIN, value & 32);
  IO::digitalWrite(LCD_D2_PIN, value & 64);
  IO::digitalWrite(LCD_D3_PIN, value & 128);
  pulseEnable();
#endif //LCD_ENABLE_8BITMODE
}

uint8_t LiquidCrystal::print(const char str[])
{
  return write(str);
}

uint8_t LiquidCrystal::print(char c)
{
  return write((uint8_t) c);
}

uint8_t LiquidCrystal::write(const uint8_t *buffer, uint8_t size)
{
  uint8_t n = 0;
  while (size--) {
    n += write(*buffer++);
  }
  return n;
}

#endif
