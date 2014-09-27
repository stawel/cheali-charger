//*****************************************************************************
//
//! \file DrvADC.c
//! \brief Driver for the ADC.
//! \version v1.02.002
//! \date 09/05/2011
//! \author Nuvoton
//! \copy
//!
//! Copyright (c)  2011, Nuvoton 
//! All rights reserved.
//
//*****************************************************************************

//*****************************************************************************
//
//! Include related headers
//
//*****************************************************************************
#include "M051Series.h"
#include "core_cm0.h"
#include "DrvADC.h"
#include "DrvSYS.h"

//*****************************************************************************
//
//! Global file scope (static) variables
//
//*****************************************************************************
static DRVADC_ADC_CALLBACK *g_ptADCCallBack;
static DRVADC_ADCMP0_CALLBACK *g_ptADCMP0CallBack;
static DRVADC_ADCMP1_CALLBACK *g_ptADCMP1CallBack;
static uint32_t g_pu32UserData[3];

//*****************************************************************************
//
//! \brief Enable the ADC function. Configure the corresponding pins of the
//!  specified channels as analog input pins. 
//!
//! \param InputMode Specify the type of the analog input signal. It might be
//!  single-end (ADC_SINGLE_END) or differential input (ADC_DIFFERENTIAL).
//!
//! \param OpMode Specify the operation mode. It might be single, burst,
//!  single cycle scan or continuous scan mode. (ADC_SINGLE_OP / ADC_BURST_OP /
//!  ADC_SINGLE_CYCLE_OP / ADC_CONTINUOUS_OP)
//!
//! \param u8ChannelSelBitwise Specify the input channels. For example,
//!  "u8ChannelSelBitwise=0x1(00000001b)" means the ADC channel 0 is enabled;
//!  "u8ChannelSelBitwise=0x19(00011001b)" means the ADC channel 0, 3 and 4 are
//!  enabled.
//!
//! \param ClockSrc Specify the clock source of ADC clock. It could be
//!  EXTERNAL_12MHZ / INTERNAL_PLL / INTERNAL_RC22MHZ.
//!
//! \param u8AdcDivisor Determine the ADC clock frequency. It could be 0~255.
//!  ADC clock frequency = ADC clock source frequency / ( u8AdcDivisor + 1 ) 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_Open(E_ADC_INPUT_MODE InputMode, E_ADC_OPERATION_MODE OpMode, uint8_t u8ChannelSelBitwise, E_ADC_CLK_SRC ClockSrc, uint8_t u8AdcDivisor)
{
    //
    // reset ADC 
    //
    SYS->IPRSTC2.ADC_RST = 1;
    SYS->IPRSTC2.ADC_RST = 0;
    
    //
    // ADC clock source 
    //
    SYSCLK->CLKSEL1.ADC_S = ClockSrc;
    
    //
    // Set ADC divisor 
    //
    SYSCLK->CLKDIV.ADC_N = u8AdcDivisor;

    //
    // ADC engine clock enable 
    //
    SYSCLK->APBCLK.ADC_EN = 1;

    //
    // ADC enable 
    //
    ADC->ADCR.ADEN = 1;
    
    if( InputMode == ADC_SINGLE_END )

        //
        // single end input
        //
        ADC->ADCR.DIFFEN = 0;     
    else

        //
        // differential input
        //
        ADC->ADCR.DIFFEN = 1;     
    
    if( OpMode == ADC_SINGLE_OP )

        //
        // single mode
        //
        ADC->ADCR.ADMD = 0;     
    else if( OpMode == ADC_BURST_OP )

        //
        // burst mode
        //
        ADC->ADCR.ADMD = 1;     
    else if( OpMode == ADC_SINGLE_CYCLE_OP )

        //
        // single-cycle scan mode
        //
        ADC->ADCR.ADMD = 2;     
    else

        //
        // continuous scan mode
        //
        ADC->ADCR.ADMD = 3;     
    
    ADC->ADCHER.CHEN = u8ChannelSelBitwise;
}

//*****************************************************************************
//
//! \brief Disable the ADC function, ADC engine clock, ADC compare 0&1 and
//!  related interrupts. 
//!
//! \param None 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_Close(void)
{     
    //
    // ADC disable
    //
    ADC->ADCR.ADEN = 0;      
          
    //
    // ADC engine clock disable
    //
    SYSCLK->APBCLK.ADC_EN = 0;   
      
    //
    // Disable ADC interrupt
    //
    ADC->ADCR.ADIE = 0;                
    NVIC_DisableIRQ(ADC_IRQn);
    
    //
    // Disable ADC compare 0
    //
    ADC->ADCMPR[0].CMPEN = 0;   
       
    //
    // Disable ADC compare 0 interrupt
    //
    ADC->ADCMPR[0].CMPIE = 0;          
    
    //
    // Disable ADC compare 1
    //
    ADC->ADCMPR[1].CMPEN = 0; 
         
    //
    // Disable ADC compare 1 interrupt
    //
    ADC->ADCMPR[1].CMPIE = 0;          
}

//*****************************************************************************
//
//! \brief Select ADC input channels. 
//!
//! \param u8ChannelSelBitwise Specify the analog input channels. 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_SetADCChannel(uint8_t u8ChannelSelBitwise)
{     
    ADC->ADCHER.CHEN = u8ChannelSelBitwise;
}

//*****************************************************************************
//
//! \brief Select the input signal source of channel 7. ADC channel 7 is a multi
//! -function channel. Its input source could come from a external signal or a
//!  internal bandgap voltage. 
//!
//! \param Ch7Src Specify the analog input source. It could be
//!  EXTERNAL_INPUT_SIGNAL / INTERNAL_BANDGAP. 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_ConfigADCChannel7(E_ADC_CH7_SRC Ch7Src)
{
    ADC->ADCHER.PRESEL = Ch7Src;
}

//*****************************************************************************
//
//! \brief Set the ADC input mode. 
//!
//! \param InputMode Specify the input mode. It could be ADC_SINGLE_END /
//!  ADC_DIFFERENTIAL. 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_SetADCInputMode(E_ADC_INPUT_MODE InputMode)
{
    if( InputMode == ADC_SINGLE_END )

        //
        // single end input
        //
        ADC->ADCR.DIFFEN = 0;     
    else

        //
        // differential input
        //
        ADC->ADCR.DIFFEN = 1;     
}

//*****************************************************************************
//
//! \brief Set the ADC operation mode. 
//!
//! \param OpMode Specify the operation mode. It could be ADC_SINGLE_OP /
//!  ADC_BURST_OP / ADC_SINGLE_CYCLE_OP / ADC_CONTINUOUS_OP. 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_SetADCOperationMode(E_ADC_OPERATION_MODE OpMode)
{
    if( OpMode == ADC_SINGLE_OP )

        //
        // single mode
        //
        ADC->ADCR.ADMD = 0;         
    else if( OpMode == ADC_BURST_OP )

        //
        // burst mode
        //
        ADC->ADCR.ADMD = 1;         
    else if( OpMode == ADC_SINGLE_CYCLE_OP )

        //
        // single-cycle scan mode
        //
        ADC->ADCR.ADMD = 2;         
    else

        //
        // continuous scan mode
        //
        ADC->ADCR.ADMD = 3;         
}

//*****************************************************************************
//
//! \brief Set the ADC clock source. 
//!
//! \param ClockSrc Specify the ADC clock source. It could be EXTERNAL_12MHZ
//!  / INTERNAL_PLL / INTERNAL_RC22MHZ. 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_SetADCClkSrc(E_ADC_CLK_SRC ClockSrc)
{
    //
    // ADC clock source
    //
    SYSCLK->CLKSEL1.ADC_S = ClockSrc; 
}

//*****************************************************************************
//
//! \brief Set the divisor value of ADC clock. 
//!
//! \param u8AdcDivisor Specify the divisor value. It could be 0~255. ADC
//!  clock frequency = ADC clock source frequency / ( u8AdcDivisor + 1 ) 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_SetADCDivisor(uint8_t u8AdcDivisor)
{
    SYSCLK->CLKDIV.ADC_N = u8AdcDivisor;
}

//*****************************************************************************
//
//! \brief Enable the ADC interrupt and setup callback function. 
//!
//! \param Callback The callback function of ADC interrupt.
//!
//! \param u32UserData This user's data will be passed to the callback
//!  function. 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_EnableADCInt(DRVADC_ADC_CALLBACK Callback, uint32_t u32UserData)
{
    g_ptADCCallBack = Callback;
    g_pu32UserData[0] = u32UserData;

    //
    // clear the A/D interrupt flag for safe
    //
    _DRVADC_CLEAR_ADC_INT_FLAG();     
    ADC->ADCR.ADIE = 1;
    NVIC_SetPriority(ADC_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
    NVIC_EnableIRQ(ADC_IRQn);
}

//*****************************************************************************
//
//! \brief Disable the ADC interrupt. 
//!
//! \param None 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_DisableADCInt(void)
{
    ADC->ADCR.ADIE = 0;
}

//*****************************************************************************
//
//! \brief Enable the ADC compare 0 interrupt and setup callback function. 
//!
//! \param Callback The callback function of ADC result monitor 0 (compare 0)
//!  interrupt.
//!
//! \param u32UserData This user's data will be passed to the callback
//!  function. 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_EnableADCCmp0Int(DRVADC_ADCMP0_CALLBACK Callback, uint32_t u32UserData)
{
    g_ptADCMP0CallBack = Callback;
    g_pu32UserData[1] = u32UserData;

    //
    // clear the ADC comparator 0 interrupt flag for safe
    //
    _DRVADC_CLEAR_CMP0_INT_FLAG();        
    ADC->ADCMPR[0].CMPIE = 1;
    NVIC_SetPriority(ADC_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
    NVIC_EnableIRQ(ADC_IRQn);
}

//*****************************************************************************
//
//! \brief Disable the ADC compare 0 interrupt. 
//!
//! \param None 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_DisableADCCmp0Int(void)
{
    ADC->ADCMPR[0].CMPIE = 0;
}

//*****************************************************************************
//
//! \brief Enable the ADC compare 1 interrupt and setup callback function. 
//!
//! \param Callback The callback function of ADC result monitor 1 (compare 1)
//!  interrupt.
//!
//! \param u32UserData This user's data will be passed to the callback
//!  function. 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_EnableADCCmp1Int(DRVADC_ADCMP1_CALLBACK Callback, uint32_t u32UserData)
{
    g_ptADCMP1CallBack = Callback;
    g_pu32UserData[2] = u32UserData;

    //
    // clear the ADC comparator 1 interrupt flag for safe
    //
    _DRVADC_CLEAR_CMP1_INT_FLAG();        
    ADC->ADCMPR[1].CMPIE = 1;
    NVIC_SetPriority(ADC_IRQn, (1<<__NVIC_PRIO_BITS) - 2);
    NVIC_EnableIRQ(ADC_IRQn);
}

//*****************************************************************************
//
//! \brief Disable the ADC compare 1 interrupt. 
//!
//! \param None 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_DisableADCCmp1Int(void)
{
    ADC->ADCMPR[1].CMPIE = 0;
}

//*****************************************************************************
//
//! \brief ADC Interrupt Service Routine. According to the interrupt flags to
//!  call the corresponding callback function. 
//!
//! \param None 
//!
//! \return None.  
//
//*****************************************************************************
/*void ADC_IRQHandler(void)
{
    if(ADC->ADSR.ADF==1)
    {
        if(g_ptADCCallBack)
            g_ptADCCallBack(g_pu32UserData[0]);
        
        //
        // clear the A/D conversion flag "ADC->ADSR.ADF = 1;" is not recommended.
        //  It may clear CMPF0 and CMPF1. 
        //
        outpw(ADC_ADSR, (inpw(ADC_ADSR)&(~0x7))|0x1);
    }
    
    if(ADC->ADSR.CMPF0==1)
    {
        if(g_ptADCMP0CallBack)
            g_ptADCMP0CallBack(g_pu32UserData[1]);
        
        //
        // clear the A/D compare flag 0 "ADC->ADSR.CMPF0 = 1;" is not recommended.
        //  It may clear ADF and CMPF1. 
        //
        outpw(ADC_ADSR, (inpw(ADC_ADSR)&(~0x7))|0x2);
    }
    
    if(ADC->ADSR.CMPF1==1)
    {
        if(g_ptADCMP1CallBack)
            g_ptADCMP1CallBack(g_pu32UserData[2]);
        
        //
        // clear the A/D compare flag 1 "ADC->ADSR.CMPF1 = 1;" is not recommended.
        //  It may clear ADF and CMPF0. 
        //
        outpw(ADC_ADSR, (inpw(ADC_ADSR)&(~0x7))|0x4);
    }
}
*/
//*****************************************************************************
//
//! \brief Return the A/D conversion rate (sample/second.) The conversion rate
//!  depends on the clock source of ADC clock. It needs about 27 ADC clocks to
//!  complete an A/D conversion. 
//!
//! \param None 
//!
//! \return Conversion rate.  
//
//*****************************************************************************
uint32_t DrvADC_GetConversionRate(void)
{
    //
    // external 12MHz crystal clock
    //
    if( SYSCLK->CLKSEL1.ADC_S == 0 )           
    {
        return (__XTAL/(SYSCLK->CLKDIV.ADC_N + 1)/27);
    }

    //
    // PLL clock
    //
    else if( SYSCLK->CLKSEL1.ADC_S == 1 )      
    {
        return ( DrvSYS_GetPLLClockFreq()/(SYSCLK->CLKDIV.ADC_N + 1)/27);
    }

    //
    // internal 22MHz RC oscillator clock
    //
    else                                       
    {
        return (__IRC22M/(SYSCLK->CLKDIV.ADC_N + 1)/27);
    }
}

//*****************************************************************************
//
//! \brief Allow the external trigger pin STADC (P3.2) to be the trigger source
//!  of ADC. The trigger condition could be low-level/high-level/falling
//! -edge/positive-edge. TRGE bit and ADST bit should be clear to 0 before
//!  changing TRGS bit. 
//!
//! \param TriggerCondition Specify the trigger condition. It could be
//!  LOW_LEVEL / HIGH_LEVEL / FALLING_EDGE / RISING_EDGE. 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_EnableExtTrigger(E_ADC_EXT_TRI_COND TriggerCondition)
{
    //
    // Configure P3.2 as an input pin
    //
    PORT3->PMD.PMD2 = 0;        
          
    //
    // Stop A/D conversion
    //
    ADC->ADCR.ADST = 0;       
            
    //
    // Disable the trigger source of STADC pin
    //
    ADC->ADCR.TRGEN = 0;      
            
    //
    // Select the STADC pin as the hardware trigger source
    //
    ADC->ADCR.TRGS = 0;    
               
    //
    // Set the trigger condition
    //
    ADC->ADCR.TRGCOND = TriggerCondition; 

    //
    // Enable the trigger source of STADC pin
    //
    ADC->ADCR.TRGEN = 1;                  
}

//*****************************************************************************
//
//! \brief Prohibit the external ADC trigger. 
//!
//! \param None 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_DisableExtTrigger(void)
{
    ADC->ADCR.TRGEN = 0;
}

//*****************************************************************************
//
//! \brief Set the ADST bit of ADCR to start the A/D conversion action. 
//!
//! \param None 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_StartConvert(void)
{
    //
    // clear the A/D conversion interrupt flag (ADF) "ADC->ADSR.ADF = 1;" is not
    //  recommended. It may clear CMPF0 and CMPF1 
    //
    outpw(ADC_ADSR, (inpw(ADC_ADSR)&(~0x7))|0x1);
    ADC->ADCR.ADST = 1;
}

//*****************************************************************************
//
//! \brief Clear the ADST bit of ADCR to stop the A/D conversion action. 
//!
//! \param None 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_StopConvert(void)
{
    ADC->ADCR.ADST = 0;
}

//*****************************************************************************
//
//! \brief Check whether the conversion action is finished or not. 
//!
//! \param None 
//!
//! \return TRUE the conversion action is finished. FALSE: the conversion
//!  action is in progress.  
//
//*****************************************************************************
uint32_t DrvADC_IsConversionDone(void)
{
    if(ADC->ADSR.ADF==0)
        return FALSE;
    else
        return TRUE;
}

//*****************************************************************************
//
//! \brief Get the conversion result of the specified ADC channel.  
//!
//! \param u8ChannelNum Specify the ADC channel. It could be 0~7. 
//!
//! \return Conversion result.  
//
//*****************************************************************************
int32_t DrvADC_GetConversionData(uint8_t u8ChannelNum)
{
    return (ADC->ADDR[u8ChannelNum].RSLT);
}

//*****************************************************************************
//
//! \brief Check whether the conversion data is valid or not. 
//!
//! \param u8ChannelNum Specify the ADC channel. It could be 0~7. 
//!
//! \return TRUE valid. 
//!         FALSE invalid.  
//
//*****************************************************************************
uint32_t DrvADC_IsDataValid(uint8_t u8ChannelNum)
{
    volatile uint8_t u8ChannelSelBitwise[8]={ 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80 };
    
    if(u8ChannelNum>7)
        return FALSE;
    if( ADC->ADSR.VALID & u8ChannelSelBitwise[u8ChannelNum] )
        return TRUE;
    else
        return FALSE;
}

//*****************************************************************************
//
//! \brief Check whether the conversion data is overrun or not. 
//!
//! \param u8ChannelNum Specify the ADC channel. It could be 0~7. 
//!
//! \return TRUE overrun. 
//!         FALSE non-overrun.  
//
//*****************************************************************************
uint32_t DrvADC_IsDataOverrun(uint8_t u8ChannelNum)
{
    volatile uint8_t u8ChannelSelBitwise[8]={ 1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80 };
    
    if(u8ChannelNum>7)
        return FALSE;
    if( ADC->ADSR.OVERRUN & u8ChannelSelBitwise[u8ChannelNum] )
        return TRUE;
    else
        return FALSE;
}

//*****************************************************************************
//
//! \brief Enable the ADC result monitor 0 and configure the necessary settings.
//!  
//!
//! \param u8CmpChannelNum Specify the channel number that want to compare.
//!
//! \param CmpCondition Specify the compare condition. LESS_THAN for the
//!  condition of "less than the compare data"; GREATER_OR_EQUAL for the
//!  condition of "greater or equal to the compare data."
//!
//! \param u16CmpData Specify the compare data.
//!
//! \param u8CmpMatchCount Specify the compare match count. 
//!
//! \return E_DRVADC_ARGUMENT one of the input arguments is out of the range.
//!         E_SUCCESS the compare function is enabled.  
//
//*****************************************************************************
int32_t DrvADC_EnableADCCmp0(uint8_t u8CmpChannelNum, E_ADC_CMP_CONDITION CmpCondition, uint16_t u16CmpData, uint8_t u8CmpMatchCount)
{
    if(u8CmpChannelNum>7)
        return E_DRVADC_ARGUMENT;
    
    if(u16CmpData>0x0FFF)
        return E_DRVADC_ARGUMENT;
    
    if(u8CmpMatchCount>0x0F)
        return E_DRVADC_ARGUMENT;
    
    ADC->ADCMPR[0].CMPCH     = u8CmpChannelNum;
    ADC->ADCMPR[0].CMPCOND   = CmpCondition;
    ADC->ADCMPR[0].CMPD      = u16CmpData;
    ADC->ADCMPR[0].CMPMATCNT = u8CmpMatchCount;
    ADC->ADCMPR[0].CMPEN     = 1;
    
    return E_SUCCESS;
}

//*****************************************************************************
//
//! \brief Disable the ADC result monitor 0. 
//!
//! \param None 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_DisableADCCmp0(void)
{
    ADC->ADCMPR[0].CMPEN = 0;
}

//*****************************************************************************
//
//! \brief Enable the ADC result monitor 1 and configure the necessary settings.
//!  
//! \param u8CmpChannelNum Specify the channel number that want to compare.
//!
//! \param CmpCondition Specify the compare condition. LESS_THAN for the
//!  condition of "less than the compare data"; GREATER_OR_EQUAL for the
//!  condition of "greater or equal to the compare data."
//!
//! \param u16CmpData Specify the compare data.
//!
//! \param u8CmpMatchCount Specify the compare match count. 
//!
//! \return E_DRVADC_ARGUMENT one of the input arguments is out of the range.
//!         E_SUCCESS the compare function is enabled.  
//
//*****************************************************************************
int32_t DrvADC_EnableADCCmp1(uint8_t u8CmpChannelNum, E_ADC_CMP_CONDITION CmpCondition, uint16_t u16CmpData, uint8_t u8CmpMatchCount)
{
    if(u8CmpChannelNum>7)
        return E_DRVADC_ARGUMENT;
    
    if(u16CmpData>0x0FFF)
        return E_DRVADC_ARGUMENT;
    
    if(u8CmpMatchCount>0x0F)
        return E_DRVADC_ARGUMENT;
    
    ADC->ADCMPR[1].CMPCH     = u8CmpChannelNum;
    ADC->ADCMPR[1].CMPCOND   = CmpCondition;
    ADC->ADCMPR[1].CMPD      = u16CmpData;
    ADC->ADCMPR[1].CMPMATCNT = u8CmpMatchCount;
    ADC->ADCMPR[1].CMPEN     = 1;
    
    return E_SUCCESS;
}

//*****************************************************************************
//
//! \brief Disable the ADC result monitor 1. 
//!
//! \param None 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_DisableADCCmp1(void)
{
    ADC->ADCMPR[1].CMPEN = 0;
}

//*****************************************************************************
//
//! \brief Enable the self calibration function. 
//!
//! \param None 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_EnableSelfCalibration(void)
{
    ADC->ADCALR.CALEN = 1;
}

//*****************************************************************************
//
//! \brief Check whether the self calibration action is finished or not. 
//!
//! \param None 
//!
//! \return TRUE the self calibration action is finished.
//!         FALSE the self calibration action is in progress.  
//
//*****************************************************************************
uint32_t DrvADC_IsCalibrationDone(void)
{
    if(ADC->ADCALR.CALDONE == 1)
        return TRUE;
    else
        return FALSE;
}

//*****************************************************************************
//
//! \brief Disable the self calibration function. 
//!
//! \param None 
//!
//! \return None.  
//
//*****************************************************************************
void DrvADC_DisableSelfCalibration(void)
{
    ADC->ADCALR.CALEN = 0;
}

//*****************************************************************************
//
//! \brief Get the version number of M05xx ADC driver. 
//!
//! \param None 
//!
//! \return Version number.  
//
//*****************************************************************************
uint32_t DrvADC_GetVersion (void)
{
    return DRVADC_VERSION_NUM;
}

