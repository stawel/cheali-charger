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

extern "C" {
#include "M051Series.h"
}

extern "C" {
void PWMA_IRQHandler(void)
{
	//TODO: not used
    PWM_ClearPeriodIntFlag(PWMA, 0);
}
} //extern "C"

uint32_t pwm_n;
namespace outputPWM {


#define PWM_GET_CNR(pwm, u32ChannelNum)  (*((__IO uint32_t *) ((((uint32_t)&((pwm)->CNR0)) + (u32ChannelNum) * 12))))


void initialize(void)
{
    CLK_EnableModuleClock(PWM01_MODULE);

    /* Select PWM module clock source */
    CLK_SetModuleClock(PWM01_MODULE, CLK_CLKSEL1_PWM01_S_HCLK, 0);

    /* Reset PWMA channel0~channel3 */
    SYS_ResetModule(PWM03_RST);

    //TODO: move this to IO.h
    /* Set P2 multi-function pins for PWMA Channel0~3 */
    SYS->P2_MFP |= SYS_MFP_P21_PWM1;



    /* set PWMA channel 1 output configuration */
    // TODO: 16kHz
    PWM_ConfigOutputChannel(PWMA, PWM_CH1, 16000, 1);

    /* Enable PWM Output path for PWMA channel 1 */
    PWM_EnableOutput(PWMA, 1<< PWM_CH1);

    // Enable PWM channel 1 period interrupt
//    PWMA->PIER = PWM_PIER_PWMIE1_Msk;
//   NVIC_EnableIRQ(PWMA_IRQn);

    // Start
    PWM_Start(PWMA, 1<< PWM_CH1);

    PWM_ENABLE_OUTPUT_INVERTER(PWMA, PWM_CH1);
	pwm_n = PWM_GET_CNR(PWMA, PWM_CH1);
	//1562 ?? for some reason the u8Prescale is set to 2 in PWM_ConfigOutputChannel
	//maybe it is possible to get 1562*2

}

void setPWM(uint8_t pin, uint32_t value)
{
	//TODO: use pin
	PWM_SET_CMR(PWMA, PWM_CH1, value);
}

} //namespace outputPWM


