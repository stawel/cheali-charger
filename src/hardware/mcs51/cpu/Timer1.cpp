
#include "Timer1.h"
#include "Hardware.h"
#include "atomic.h"

/*namespace {
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
}*/

void Timer1::setPWM(char pin, uint16_t val)  // expects duty cycle to be 10 bit (1024)
{
}

void Timer1::initialize()
{
}

void Timer1::disablePWM(char pin)
{
}

