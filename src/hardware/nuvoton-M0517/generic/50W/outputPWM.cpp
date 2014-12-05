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

#include "LcdPrint.h"
#include "outputPWM.h"
#include "irq_priority.h"

extern "C" {
#include "M051Series.h"
}

#define PWM_GET_CNR(pwm, u32ChannelNum)  (*((__IO uint32_t *) ((((uint32_t)&((pwm)->CNR0)) + (u32ChannelNum) * 12))))


//based on http://www.nuvoton-m0.com/code/en/Init_M051/wiz/index.htm

namespace outputPWM {

//TODO: remove
volatile uint32_t pwm_n;

volatile uint32_t PWM_valueA = 0;
volatile uint32_t PWM_valueB = 0;
volatile uint32_t PWM_sumA = 0;
volatile uint32_t PWM_sumB = 0;

void setCMR() {
    //modulate the PWM - we modulate the PWM signal to get more precision.
    //(the PWM frequency stays at about 32kHz)
    PWM_sumA+=PWM_valueA;
    PWM_sumB+=PWM_valueB;

    PWM_SET_CMR(PWMA, PWM_CH1, PWM_sumA/OUTPUT_PWM_PRECISION_FACTOR);
    PWM_SET_CMR(PWMB, PWM_CH2, PWM_sumB/OUTPUT_PWM_PRECISION_FACTOR);

    PWM_sumA%=OUTPUT_PWM_PRECISION_FACTOR;
    PWM_sumB%=OUTPUT_PWM_PRECISION_FACTOR;
}

void initialize(void)
{
    CLK_EnableModuleClock(PWM01_MODULE);
    CLK_EnableModuleClock(PWM67_MODULE);

    /* Select PWM module clock source */
    CLK_SetModuleClock(PWM01_MODULE, CLK_CLKSEL1_PWM01_S_HCLK, 0);
    CLK_SetModuleClock(PWM67_MODULE, CLK_CLKSEL2_PWM67_S_HCLK, 0);

    /* Reset PWMA channel0~channel3 */
    SYS_ResetModule(PWM03_RST);
    SYS_ResetModule(PWM47_RST);

    /* set PWMA channel 1 output configuration */
    // 32kHz
    PWM_ConfigOutputChannel(PWMA, PWM_CH1, 32000, 0);
    PWM_ConfigOutputChannel(PWMB, PWM_CH2, 32000, 0);

    /* Enable PWM Output path for PWMA channel 1 */
    PWM_EnableOutput(PWMA, 1<< PWM_CH1);
    PWM_EnableOutput(PWMB, 1<< PWM_CH2);

    // Enable PWM channel 1 period interrupt
    PWMA->PIER = PWM_PIER_PWMIE1_Msk;
    NVIC_EnableIRQ(PWMA_IRQn);
    NVIC_SetPriority(PWMA_IRQn, OUTPUT_PWM_IRQ_PRIORITY);

    pwm_n = PWM_GET_CNR(PWMB, PWM_CH2);
    pwm_n = PWM_GET_CNR(PWMA, PWM_CH1);

    //Hm.... this is done in PWM_ConfigOutputChannel but we want to be sure
    PWM_SET_CNR(PWMA, PWM_CH1, OUTPUT_PWM_PERIOD);
    PWM_SET_CNR(PWMB, PWM_CH2, OUTPUT_PWM_PERIOD);

    // Start
    PWM_Start(PWMA, 1<< PWM_CH1);
    PWM_Start(PWMB, 1<< PWM_CH2);
}

void setPWM(uint8_t pin, uint32_t value)
{
    if(pin == 20) {
        PWM_valueA = value;
        SYS->P2_MFP |= SYS_MFP_P21_PWM1;
    } else if(pin == 26) {
        PWM_valueB = value;
        SYS->P2_MFP |= SYS_MFP_P26_PWM6;
    }
}

void disablePWM(uint8_t pin)
{
    if(pin == 20) {
        SYS->P2_MFP &= ~SYS_MFP_P21_PWM1;
    } else if (pin == 26) {
        SYS->P2_MFP &= ~SYS_MFP_P26_PWM6;
    }
}

} //namespace outputPWM

extern "C" {
void PWMA_IRQHandler(void)
{
    outputPWM::setCMR();
    PWM_ClearPeriodIntFlag(PWMA, PWM_CH1);
}
} //extern "C"


