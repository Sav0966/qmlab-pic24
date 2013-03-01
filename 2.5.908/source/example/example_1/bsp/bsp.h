/* ==============================================================================================
     Project:         TNKernel RTOS example
     Project desc.:   

   ----------------------------------------------------------------------------------------------
     File:            bsp.h
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
     Description: Board Support Package header
    
   ============================================================================================*/

#ifndef _BSP_H
#define _BSP_H

#if (defined(__ARMCC_VERSION) && (__TARGET_ARCH_ARM == 0) && (__TARGET_ARCH_THUMB == 4))
    #define __CORTEX_M3_RVTC__
#endif


#if defined(__dsPIC33F__)
    #include  <p33Fxxxx.h>
#elif defined(__PIC24F__)
    #include <p24Fxxxx.h>
#elif defined(__PIC32MX__)
    #include <p32xxxx.h>
    #include <plib.h>
#elif defined(__CORTEX_M3_RVTC__)
    #include <lpc17xx.h>
    //#include "lpc176x.h"
#else
    #error "Unknown MCU"
#endif


typedef enum
{
    LED_1,
    LED_2,
    LED_3,
    LED_4,
    LED_5,
    LED_6,
    LED_7,
    LED_8
} LEDS;


void Sys_Conf(void);
void Sys_Int_Conf(void);
void Sys_Tmr_Conf(void);
void Sys_Tmr_Int_Handler(void);
void Sys_Tmr_Int_En(void);

void         Btn_Conf(void);
void         Btn_Int_En(void);
void         Btn_Int_Handler(void);
unsigned int Btn_State_Get(void);

void Btn_Tmr_En(void);
void Btn_Tmr_Dis(void);
unsigned int Btn_Tmr_Get(void);

void Led_Conf(void);
void Led_On(LEDS num);
void Led_Off(LEDS num);

void Pot_Conf(void);
unsigned int Pot_State_Get(void);

void Uart_Conf(void);
void Uart_En(void);
void Uart_Int_Conf(void);
void Uart_Send(unsigned char *data, unsigned short size);
void Uart_RX_Int_Handler(void);



#endif  /* _BSP_H */
/* end of file ---------------------------------------------------------------------------------- */

