#include "TimerOne.h"
#include "Hardware.h"
#include <avr/interrupt.h>
#include "atomic.h"

namespace {
    volatile unsigned int TimerOne_value=0;
    volatile unsigned int TimerOne_sumValue=0;
    volatile uint8_t whichPin=0;

    void setOCR() {
        //modulate the PWM - we modulate the PWM signal to get more precision.
        //(the PWM frequency stays at about 31kHz)
        uint16_t v;
        TimerOne_sumValue+=TimerOne_value;
        v=(TimerOne_sumValue>>TIMERONE_PRECISION);
        TimerOne_sumValue-=v<<TIMERONE_PRECISION;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            if(whichPin == 14) OCR1A = v;
            if(whichPin == 13) OCR1B = v;
        }
    }
}

ISR(TIMER1_OVF_vect)
{
    setOCR(); //modulate the PWM
    hardware::soundInterrupt();
}

void TimerOne::setPWM(char pin, uint16_t val)  // expects duty cycle to be 10 bit (1024)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {

        TimerOne_value = val;
        whichPin = pin;
//      the OCR1A or OCR1B will be updated at the TIMER1 interrupt
//      setOCR();
        if(pin == 14) {
            TCCR1A |= _BV(COM1A1);                                 // activates the output pin
        } else if(pin == 13) {
            TCCR1A |= _BV(COM1B1);
        }
    }
}

void TimerOne::initialize()
{
    TCCR1A = 0;                 // clear control register A
    TCCR1B = _BV(WGM13);        // set mode 8: phase and frequency correct pwm, stop the timer

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ICR1 = TIMERONE_PERIOD;
    }
    TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
    TCCR1B |= _BV(CS10);

    TIMSK |= _BV(TOIE1);
}

void TimerOne::disablePWM(char pin)
{
  if(pin == 14)       TCCR1A &= ~_BV(COM1A1);   // clear the bit that enables pwm on PB1
  else if(pin == 13) TCCR1A &= ~_BV(COM1B1);   // clear the bit that enables pwm on PB2
}

