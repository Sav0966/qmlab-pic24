/* ==============================================================================================
     Project:         TNKernel RTOS Example
     Project desc.:   

   ----------------------------------------------------------------------------------------------
     File:            bsp.c
     Author:          Copyright © 2004-2006 Yuri Tiomkin
                      Copyright © 2006-2010 Alex Borisov
     Date:            2004, 2005, 2010

   ==============================================================================================
     Compiled Using:  Microchip C30 v.2.5
                      Microchip C30 v.1.11A
     Processor:       PIC24/dsPIC
                      PIC32

   ==============================================================================================
     TNKernel real-time kernel 2.5
     -----------------------------

     Copyright © 2004,2005 Yuri Tiomkin
     All rights reserved.

     Permission to use, copy, modify, and distribute this software in source and binary forms and
     its documentation for any purpose and without fee is hereby granted, provided that the above
     copyright notice appear in all copies and that both that copyright notice and this permission
     notice appear in supporting documentation.

     THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
     AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL YURI TIOMKIN OR
     CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
     CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
     SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
     THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
     OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
     POSSIBILITY OF SUCH DAMAGE.

   ==============================================================================================
     Description: Board Support Package

   ============================================================================================*/

#include "bsp.h"            /* BSP      */

#include <tnkernel.h>       /* TNK      */
#include "tnres.h"          /* ресурсы  */


#define CORE_TIMER_PERIOD      40000UL          /* 1 ms @ 80 MHz */

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------


void Sys_Conf (void)
{
    mCheConfigure(CHE_CONF_WS2       |  /* Flash WS = 2 (Fflmax = 30 MHz, FCY = 80 MHz) */
                  CHE_CONF_PF_ALL    |  /* Предвыбора для кэшируемого и некэшируемого регионов */
                  CHE_CONF_DC_NONE   |  /* Для данных кэш не используем */
                  CHE_CONF_COH_INVUNL   /* Механизм проверки валидности данных в кэше  */
                  );
    CheKseg0CacheOn();                  /* Включаем кэширование в kseg0             */
    mBMXDisableDRMWaitState();          /* Скорость доступа к SRAM = частоте шины  */

    AD1PCFG = 0xFFFF;       /* Disable all analog channels */
    CNEN = 0xFFFFFFFF;

    DDPCON = 0;
}






void Sys_Int_Conf (void)
{
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();
}






void Sys_Tmr_Conf (void)
{
    _CP0_BIC_DEBUG(1 << 25);                    /* Таймер будет остановлен при входе в отладку */
    OpenCoreTimer(CORE_TIMER_PERIOD);           // load with the period
}






void Sys_Tmr_Int_En (void)
{
    INTSetVectorPriority(INT_CORE_TIMER_VECTOR, INT_PRIORITY_LEVEL_1);
    INTSetVectorSubPriority(INT_CORE_TIMER_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTClearFlag(INT_CT);
    INTEnable(INT_CT, INT_ENABLED);
}








/**
 * 
 * 
 */
void Sys_Tmr_Int_Handler (void)
{
    INTClearFlag(INT_CT);                   // clear the interrupt flag
    UpdateCoreTimer(CORE_TIMER_PERIOD);     // update the period
}







void Btn_Conf (void)
{
    mCNOpen(CN_ON, CN15_ENABLE, 0);             // Enable Change notification for CN15
}








void Btn_Int_Handler (void)
{
    INTClearFlag(INT_CN);                   // clear the interrupt flag
}







unsigned int Btn_State_Get (void)
{
    if (PORTDbits.RD6)
        return 0;
    else
        return 1;
}







void Btn_Int_En (void)
{
    INTSetVectorPriority(INT_CHANGE_NOTICE_VECTOR, INT_PRIORITY_LEVEL_1);
    INTSetVectorSubPriority(INT_CHANGE_NOTICE_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTClearFlag(INT_CN);
    INTEnable(INT_CN, INT_ENABLED);
}








void Led_Conf (void)
{
    TRISACLR = 0x000000FF;
    LATACLR  = 0x000000FF;
}







void Led_On (LEDS num)
{
    LATASET = (1 << num);
}







void Led_Off (LEDS num)
{
    LATACLR = (1 << num);
}








void Pot_Conf (void)
{
    TRISBbits.TRISB5 = 1;
    AD1PCFG &= ~(1 << 5);


    AD1CON1bits.FORM = 0;
    AD1CON1bits.SSRC = 7;       /* auto-convert */
    AD1CON1bits.ASAM = 0;       /* start sampling manually */

    AD1CON2bits.VCFG = 0;       /* VDD-VSS */
    AD1CON2bits.CSCNA = 0;      /* do not scan inputs */
    AD1CON2bits.SMPI = 0;
    AD1CON2bits.BUFM = 0;
    AD1CON2bits.ALTS = 0;

    AD1CON3bits.ADRC = 0;
    AD1CON3bits.ADCS = 16;
    AD1CON3bits.SAMC = 8;

    AD1CHSbits.CH0SA = 5;       /* AN5 */
    AD1CHSbits.CH0NA = 0;
    AD1CHSbits.CH0SB = 5;       /* AN5 */
    AD1CHSbits.CH0NB = 0;

    AD1CON1bits.ON = 1;       /* enable */
}



/**
 * 
 *
 * 
 * @return unsigned int 
 */
unsigned int Pot_State_Get (void)
{
    unsigned int ret = 0;

    IFS1bits.AD1IF   = 0;
    AD1CON1bits.SAMP = 1;
    while (!IFS1bits.AD1IF)
    ret = ADC1BUF0;

    return ret;
}



/**
 * 
 *
 */
void Uart_Conf (void)
{
    UARTConfigure      (UART2, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode    (UART2, UART_INTERRUPT_ON_TX_DONE | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetDataRate    (UART2, 40000000UL, 19200UL);
    UARTSetLineControl (UART2, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
}


void Uart_En (void)
{
    UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
}


/**
 * 
 *
 */
void Uart_Int_Conf (void)
{
    INTSetVectorPriority(INT_UART_2_VECTOR, INT_PRIORITY_LEVEL_1);
    INTSetVectorSubPriority(INT_UART_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTClearFlag(INT_U2RX);   
    INTEnable(INT_U2RX, INT_ENABLED);
}


/**
 * 
 *
 * @param data 
 * @param size 
 */
void Uart_Send (unsigned char *data, unsigned short size)
{
    while (size--)
    {
        U2TXREG = *data++;
        while (U2STA & (1<<9));
    } 
}




/**
 * 
 *
 */
void Uart_RX_Int_Handler (void)
{
    while (U2STA & 1)
    {
        if ((U2STA & (1<<3)) || (U2STA & (1<<2)) || (U2STA & (1<<1))) 
        {
            /* FERR ??? */
            U2STACLR = 0b1110;
        }
        else    
        {
            tn_queue_isend_polling(&que_uart_queue, (void*)(U2RXREG));
        }       
    }
    INTClearFlag(INT_U2RX);                 
}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
