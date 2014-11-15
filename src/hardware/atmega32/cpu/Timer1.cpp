#include <avr/interrupt.h>

#include "Timer1.h"
#include "Hardware.h"
#include "atomic.h"

namespace {
    volatile uint16_t Timer1_valueA=0;
    volatile uint16_t Timer1_valueB=0;
    volatile uint16_t Timer1_sumA=0;
    volatile uint16_t Timer1_sumB=0;

    void setOCR() {
        //modulate the PWM - we modulate the PWM signal to get more precision.
        //(the PWM frequency stays at about 31kHz)
        Timer1_sumA+=Timer1_valueA;
        Timer1_sumB+=Timer1_valueB;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            OCR1A = (Timer1_sumA>>TIMER1_PRECISION);
            OCR1B = (Timer1_sumB>>TIMER1_PRECISION);
        }
        Timer1_sumA&=(1<<TIMER1_PRECISION) - 1;
        Timer1_sumB&=(1<<TIMER1_PRECISION) - 1;
    }
}

ISR(TIMER1_OVF_vect)
{
    setOCR(); //modulate the PWM
    hardware::soundInterrupt();
}

void Timer1::setPWM(char pin, uint16_t val)  // expects duty cycle to be 10 bit (1024)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {

//      the OCR1A or OCR1B will be updated at the TIMER1 interrupt
//      setOCR();
        if(pin == 14) {
            Timer1_valueA = val;
            TCCR1A |= _BV(COM1A1);                                 // activates the output pin
        } else if(pin == 13) {
            Timer1_valueB = val;
            TCCR1A |= _BV(COM1B1);
        }
    }
}

void Timer1::initialize()
{
    ICR1 = TIMER1_PERIOD;
    TCCR1A = _BV(WGM11);
    TCCR1B = _BV(WGM13)| _BV(WGM12)             // set mode 14: Fast PWM
             | _BV(CS10);                       // No prescaling

    TIMSK |= _BV(TOIE1);
}

void Timer1::disablePWM(char pin)
{
  if(pin == 14)      TCCR1A &= ~_BV(COM1A1);    // clear the bit that enables pwm on PB1
  else if(pin == 13) TCCR1A &= ~_BV(COM1B1);    // clear the bit that enables pwm on PB2
}

