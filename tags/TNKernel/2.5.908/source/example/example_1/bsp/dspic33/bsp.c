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
#include "tnres.h"          /* περσπρϋ  */


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------


void Sys_Conf (void)
{
    AD1PCFGH = 0xFFFF;       /* Disable all analog channels */
    AD1PCFGL = 0xFFFF;       /* Disable all analog channels */
    CNEN1    = 0;            /* Disable all CN */
    CNEN2    = 0;
}






void Sys_Int_Conf (void)
{

}






void Sys_Tmr_Conf (void)
{
    T2CONbits.TCS   = 0;
    T2CONbits.TGATE = 0;
    T2CONbits.TSIDL = 0;
    T2CONbits.TCKPS = 0;
    PR2 = 16000-1;                              /* 1 ms @ 16 MHz */
    T2CONbits.TON   = 1;
}






void Sys_Tmr_Int_En (void)
{

    IPC1bits.T2IP   = 1;
    IFS0bits.T2IF   = 0;
    IEC0bits.T2IE   = 1;
}








/**
 * 
 * 
 */
void Sys_Tmr_Int_Handler (void)
{
    IFS0bits.T2IF = 0;
}







void Btn_Conf (void)
{
    /* Enable Button interrupt */
    TRISDbits.TRISD6 = 1;
    CNEN1bits.CN15IE = 1;                   
}








void Btn_Int_Handler (void)
{
    IFS1bits.CNIF = 0;
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
    IPC4bits.CNIP   = 1;
    IFS1bits.CNIF   = 0;
    IEC1bits.CNIE   = 1;
}








void Led_Conf (void)
{
    TRISA &= ~(0x00FF);
}







void Led_On (LEDS num)
{
    LATA |= (1 << num);
}







void Led_Off (LEDS num)
{
    LATA &= ~(1 << num);
}








void Pot_Conf (void)
{
    TRISBbits.TRISB5 = 1;
    AD1PCFGL &= ~(1 << 5);

    AD1CON1bits.FORM = 0;
    AD1CON1bits.SSRC = 7;       /* auto-convert */
    AD1CON1bits.ASAM = 0;       /* start sampling manually */
    AD1CON1bits.AD12B = 0;      /* 10-bit ADC */
    AD1CON1bits.SIMSAM = 0;

    AD1CON2bits.VCFG = 0;       /* VDD-VSS */
    AD1CON2bits.CHPS = 0;
    AD1CON2bits.CSCNA = 0;      /* do not scan inputs */
    AD1CON2bits.SMPI = 0;
    AD1CON2bits.BUFM = 0;
    AD1CON2bits.ALTS = 0;

    AD1CON3bits.ADRC = 0;
    AD1CON3bits.ADCS = 8;
    AD1CON3bits.SAMC = 4;

    AD1CHS0bits.CH0SA = 5;       /* AN5 */
    AD1CHS0bits.CH0NA = 0;
    AD1CHS0bits.CH0SB = 5;       /* AN5 */
    AD1CHS0bits.CH0NB = 0;

    AD1CON1bits.ADON = 1;       /* enable */
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

    IFS0bits.AD1IF   = 0;
    AD1CON1bits.SAMP = 1;
    while (!IFS0bits.AD1IF)
    ret = ADC1BUF0;

    return ret;
}



/**
 * 
 *
 */
void Uart_Conf (void)
{
    TRISFbits.TRISF5    = 0;                /* TX */
    TRISFbits.TRISF4    = 1;                /* RX */
    U2BRG               = 51;               /* 19200 */
    U2MODE              = 0;
    U2STA               = 0;
    U2MODEbits.UARTEN   = 1;
    U2STAbits.UTXEN     = 1;
}


void Uart_En (void)
{
    U2MODEbits.UARTEN   = 1;
    U2STAbits.UTXEN     = 1;
}


/**
 * 
 *
 */
void Uart_Int_Conf (void)
{
    IPC7bits.U2RXIP = 1;
    IFS1bits.U2RXIF = 0;
    IEC1bits.U2RXIE = 1;
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
        while (U2STAbits.UTXBF);
        U2TXREG = *data++;
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
            U2STA &= ~(0b1110);
        }
        else
        {
            tn_queue_isend_polling(&que_uart_queue, (void*)(U2RXREG));
        }
    }
    IFS1bits.U2RXIF = 0;
}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
