#ifndef LCDPRINT_H_
#define LCDPRINT_H_

#include <stddef.h>
#include "Hardware.h"

uint8_t digits(unsigned long x);

void lcdPrint_P(const char *str, uint8_t n = LCD_COLUMNS);
void lcdPrint_E(const char *str, uint8_t n = LCD_COLUMNS);
void lcdPrint  (const char *str, uint8_t n);
void lcdPrintSpaces();
void lcdPrintSpaces(uint8_t n);


void lcdPrintEValue(uint16_t x, int8_t dig, bool dot = false, char prefix = ' ');
void lcdPrintEValueEnd(uint16_t x, int8_t dig, bool dot = false, const char *end = NULL);

void lcdPrintEValueU(uint16_t x, int8_t dig = 5, char prefix = ' ');
void lcdPrintEValueI(uint16_t x, int8_t dig, char prefix = ' ');
void lcdPrintEValueI(uint16_t x, int8_t dig, const char *end = NULL, uint8_t blink = 0);

#define BLINK_START uint8_t _start_blink = 0;
#define BLINK_TEST ( ((blink &0x7f) != _start_blink++) ? 0 : ((blink & BLINK_SPEED2) + 1) )
#define BLINK_SPEED_MS 500
#define BLINK_END return _start_blink;
#define BLINK_NON return 0;
#define BLINK_SPEED2 128

#endif /* LCDPRINT_H_ */
