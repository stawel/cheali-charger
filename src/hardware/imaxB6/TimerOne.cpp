#include "TimerOne.h"
#include <avr/interrupt.h>

namespace {
    volatile unsigned int value=0;
    volatile unsigned int sumValue=0;
    volatile uint8_t whichPin=0;

    void setOCR() {
        //modulate the PWM
        //(modulation of a modulation)
        uint8_t oldSREG;
        uint16_t v;
        sumValue+=value;
        v=(sumValue>>TIMERONE_PRECISION);
        sumValue-=v<<TIMERONE_PRECISION;

        oldSREG = SREG;
        cli();
        if(whichPin == 14) OCR1A = v;
        if(whichPin == 13) OCR1B = v;
        SREG = oldSREG;
    }
}

ISR(TIMER1_OVF_vect)
{
    setOCR(); //modulate the PWM
}

void TimerOne::setPWM(char pin, uint16_t val)  // expects duty cycle to be 10 bit (1024)
{
  value = val;
  whichPin = pin;
  TIMSK |= _BV(TOIE1);
  setOCR();
  if(pin == 14) {
    TCCR1A |= _BV(COM1A1);                                 // activates the output pin
  } else if(pin == 13) {
    TCCR1A |= _BV(COM1B1);
  }
}

void TimerOne::initialize()
{
    TCCR1A = 0;                 // clear control register A
    TCCR1B = _BV(WGM13);        // set mode 8: phase and frequency correct pwm, stop the timer

    uint8_t oldSREG = SREG;
    cli();
    ICR1 = TIMERONE_PERIOD;
    SREG = oldSREG;

    TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
    TCCR1B |= _BV(CS10);
}

void TimerOne::disablePWM(char pin)
{
  if(pin == 14)       TCCR1A &= ~_BV(COM1A1);   // clear the bit that enables pwm on PB1
  else if(pin == 13) TCCR1A &= ~_BV(COM1B1);   // clear the bit that enables pwm on PB2
}

