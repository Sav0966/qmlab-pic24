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


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

uint32_t SystemFrequency;

void Sys_Conf (void)
{
    //---- System clock ---

    #define  PLL0CFG_VAL            11      /* 11+1  */
    #define  CCLKCFG_VAL            3       /* 3+1  */
    #define  FLASHCFG_VAL           0x0000403A

    //-- Fosc = 14.7456 MHz, Fcco = 2 * 12 * 14.7456 = 353.8944 MHz
    //-- cclk = 353.8944 / 4 = 88.4736 MHz
    //-- cclk/2 = 44.2368 MHz

    //-- Also was tested with

    //--   Fosc = 14.7456 MHz, Fcco = 2 * 10 * 14.7456 = 294.912 MHz,
    //--   cclk = 294.912 / 3 = 98.304 MHz
    
    LPC_SC->SCS = (1<<5);                //-- Start main oscillator,  freq < 20MHz
    while((LPC_SC->SCS & (1<<6)) == 0);  //-- Wait for Oscillator to be ready
    
    LPC_SC->CCLKCFG = CCLKCFG_VAL;       //-- Setup Clock Divider
    LPC_SC->CLKSRCSEL = 1;               //-- Select Clock Source for PLL0 - Main oscillator
    
    LPC_SC->PLL0CFG   = PLL0CFG_VAL;
    
    LPC_SC->PLL0CON   = 0x01;             //--- PLL0 Enable
    LPC_SC->PLL0FEED  = 0xAA;
    LPC_SC->PLL0FEED  = 0x55;
    while( (LPC_SC->PLL0STAT & (1<<26)) == 0); //-- Wait for PLOCK0
    
    LPC_SC->FLASHCFG  = FLASHCFG_VAL;     //-- Flash speed
    
    LPC_SC->PLL0CON   = 0x03;             //--- PLL0 Enable & Connect
    
    LPC_SC->PLL0FEED  = 0xAA;
    LPC_SC->PLL0FEED  = 0x55;

    SystemFrequency = 72000000;
}






void Sys_Int_Conf (void)
{
    /* =============== */
}






void Sys_Tmr_Conf (void)
{
    SysTick_Config(88473);                      //-- cpu clk/1000
}






void Sys_Tmr_Int_En (void)
{
    /* =============== */
}

/**
 * 
 * 
 */
void Sys_Tmr_Int_Handler (void)
{
    /* =============== */
}

///////////////////////////////////////////

extern TN_DQUE     que_btn_queue TN_DATA;


/**
 * 
 *
 */
void Btn_Conf (void)
{
    /* порт на вход, разрешение прерывания */
    //кнопка на P0.1
    //после сброса порт на вход, с подтяжкой к + питания
    LPC_PINCON->PINSEL0  &= ~(3 << 2);  //функция порта - просто вход
    LPC_PINCON->PINMODE0 &= ~(3 << 2);  //порт на вход, pull-up
}

void Btn_Int_En (void)
{
    /* разрешение прерываний от кнопки */
    //разрешаем прерывание EXTINT3
    NVIC_EnableIRQ(EINT3_IRQn);
    //разрешаем прерывание для P0.1
    LPC_GPIOINT->IO0IntEnR |= 0x02;     //для переднего фронта
    LPC_GPIOINT->IO0IntEnF |= 0x02;     //для заднего фронта
}


void Btn_Int_Handler (void)
{
    /* обработка прерывания от кнопки  */
    LPC_GPIOINT->IO0IntClr = (1 << 1); //очищаем флаг прерывания
}

unsigned int Btn_State_Get (void)
{
    /* возвращает состояние кнопки */
    if (LPC_GPIO0->FIOPIN & (1 << 1)) {  
        return 0;  //кнопка отпущена
    } else {
        return 1;  //кнопка нажата
    }
}


void Led_Conf (void)
{
    LPC_GPIO1->FIODIR = (1 << 26) | (1 << 27) | (1 << 28) | (1 << 29); /* P2.xx defined as Outputs */
    LPC_GPIO1->FIOCLR = (1 << 26) | (1 << 27) | (1 << 28) | (1 << 29);      /* turn off all the LEDs */
}

void Led_On (LEDS num)
{
    LPC_GPIO1->FIOSET = (1 << (num + 26));
}

void Led_Off (LEDS num)
{
    LPC_GPIO1->FIOCLR = (1 << (num + 26));
}

void Pot_Conf (void)
{   //переменный резистор подключен ко входу P0.24 (AD0.1)
    uint32_t samplerate = 1000;  //количество выборок в сек
    uint32_t clkdiv;
    uint32_t pclkdiv, pclk;
    //
    LPC_SC->PCONP |= (1 << 12);   /* Enable CLOCK into ADC controller */
    // По умолчанию биты PCLKSELx сброшены и PCLK 
    //  для всей периферии установлен в 1/4 от SystemFrequency. 
    pclkdiv = (LPC_SC->PCLKSEL0 >> 24) & 0x03;
    switch (pclkdiv) {
    case 0x00:
    default:
        pclk = SystemFrequency/4;  
        break;
    case 0x01:
        pclk = SystemFrequency;
        break; 
    case 0x02:
        pclk = SystemFrequency/2;
        break; 
    case 0x03:
        pclk = SystemFrequency/8;
        break;
    }
    //
    //установка пина как входа АЦП
    //для P0.24 (AD0.1)
    LPC_PINCON->PINMODE1 &= ~(3UL << 16);
    LPC_PINCON->PINMODE1 |=  (2UL << 16);
    LPC_PINCON->PINSEL1 &=  ~(3UL << 16);
    LPC_PINCON->PINSEL1 |=   (1UL << 16);
    //
    clkdiv = (pclk / samplerate + 32) / 65;  //one conversion requries 65 ticks, addintion of 32 is used to round divide result
    if (clkdiv)
        --clkdiv;   //отнимаем 1, положено
    LPC_ADC->ADCR = (0x02 & 0xFF | ((clkdiv & 0xFF) << 8) | (1 << 21)); //конфигурация АЦП
}

/**
 * 
 *
 * 
 * @return unsigned int 
 */
unsigned int Pot_State_Get(void)
{   
    uint32_t ret;
    LPC_ADC->ADCR |= (1 << 24);       //start conversion now;
    while (!(LPC_ADC->ADSTAT & (1 << 16))) 
        ;
    LPC_ADC->ADCR &= 0xF8FFFFFF;        // stop ADC now 
    ret = ((LPC_ADC->ADGDR & 0xFFF0) >> 4);
    return ret;
}

////////////////////////////////////////////  UART  //////////////////////////////////////////

#define IER_RBR         0x01
#define IER_THRE        0x02
#define IER_RLS         0x04

#define IIR_PEND        0x01
#define IIR_RLS         0x03
#define IIR_RDA         0x02
#define IIR_CTI         0x06
#define IIR_THRE        0x01

#define LSR_RDR         0x01
#define LSR_OE          0x02
#define LSR_PE          0x04
#define LSR_FE          0x08
#define LSR_BI          0x10
#define LSR_THRE        0x20
#define LSR_TEMT        0x40
#define LSR_RXFE        0x80

#define UART_RXBUFSIZE 256
#define UART_TXBUFSIZE 256

volatile struct {
    //
    uint32_t lsr;  // line status register - регистp состояния линии UART
    uint8_t  txempty;   // признак свободного передатчика
} UART_desc;  //для одного порта

/**
 * 
 *
 */
void Uart_Conf (void)
{   //настройка для UART0 (выводы P0.2, P0.3)
    uint32_t baudrate = 19200;
    uint32_t Fdiv;
        uint32_t Fdiff;
    uint32_t pclkdiv, pclk;
    uint32_t da, m, cda = 0, cm = 1;
    uint32_t cdiff = Fdiff, ndiff;

    // Вычисление значений для предделителя и коррекции

    // By default, the PCLKSELx value is zero, thus, the PCLK for
    //   all the peripherals is 1/4 of the SystemFrequency. 
    pclkdiv = (LPC_SC->PCLKSEL0 >> 6) & 0x03; 
    switch (pclkdiv) {
    case 0x00:
    default:
        pclk = SystemFrequency/4;
        break;
    case 0x01:
        pclk = SystemFrequency;
        break; 
    case 0x02:
        pclk = SystemFrequency/2;
        break; 
    case 0x03:
        pclk = SystemFrequency/8;
        break;
    }
    //
    Fdiv = ( pclk / 16 ) / baudrate ;   //baud rate 
    Fdiff = pclk - Fdiv * baudrate * 16; //division error (reminder)
    //
    if ( Fdiff ) { //подбор коэффициентов для коррекции
        cdiff = Fdiff; //запоминаем текущую ошибку 
        for ( da = 1; da <= 15; da++ ) {         //в цикле проходим все сочетания 
            for ( m = 15; m > 0; m-- ) {
                ndiff = baudrate * 16 * Fdiv * da / m;  // очередное значение коррекции
                // вычитаем из текущей ошибки (по модулю)
                if ( ndiff > Fdiff ) {  
                    ndiff -= Fdiff;
                } else {
                    ndiff = Fdiff - ndiff;
                }
                if ( ndiff > cdiff )
                    break;  //дальше не увеличиваем - нет смысла
                if ( ndiff < cdiff ) {
                    cdiff = ndiff;
                    cda = da;
                    cm = m;
                }
            }
        }
    }
    // Установка переменных порта
    UART_desc.lsr = 0;
    UART_desc.txempty = 1;
    //
    // Установка регистров порта
    //  
    LPC_PINCON->PINSEL0 &= ~0x000000F0;
    LPC_PINCON->PINSEL0 |= 0x00000050;  /* RxD0 is P0.3 and TxD0 is P0.2 */
        /* By default, the PCLKSELx value is zero, thus, the PCLK for
        all the peripherals is 1/4 of the SystemFrequency. */
        /* Bit 6~7 is for UART0 */
    // Делитель частоты для UART и формат кадра
    LPC_UART0->LCR = 0x83;          /* 8 bits, no Parity, 1 Stop bit */
    LPC_UART0->DLM = Fdiv / 256;              //загружаем предделитель                                       
    LPC_UART0->DLL = Fdiv % 256;                
    LPC_UART0->FDR = cda & 0x0F | ((cm & 0x0F) << 4);  // и регистр коррекции скорости
    LPC_UART0->LCR = 0x03;          /* DLAB = 0 */
    //
    LPC_UART0->FCR = 0x07;          /* Enable and reset TX and RX FIFO. */
    //
}


void Uart_En (void)
{
    //?????
}


/**
 * 
 *
 */
void Uart_Int_Conf (void)
{
    NVIC_EnableIRQ(UART0_IRQn);
    LPC_UART0->IER = IER_RBR | /* IER_THRE | */ IER_RLS;  /* Enable UART0 interrupt */
}


/**
 * 
 *
 * @param data 
 * @param size 
 */
void Uart_Send (unsigned char *data, unsigned short size)
{
    while ( size ) {
        /* THRE status, contain valid data */
        //        while ( !UART_desc.txempty );    
        while (!(LPC_UART0->LSR & LSR_THRE))    ;
        //!! по идее здесь надо запретить прерывания при использовании RTOS???
        //!! чтобы запись в буфер и увеличение указателей были атомарными
        LPC_UART0->THR = *data;
        UART_desc.txempty = 0;   // not empty in the THR until it shifts out
        data++;
        size--;
    }
}

/**
 * 
 *
 */
void Uart_RX_Int_Handler (void)
{      //!!!! внимание! для LPC один обработчик на все события UART!
    uint8_t IIRValue, LSRValue;
    uint8_t Dummy = Dummy;
        uint8_t readflag = 0;

    IIRValue = LPC_UART0->IIR;
    
    IIRValue >>= 1;                     // skip pending bit in IIR 
    IIRValue &= 0x07;                   // check bit 1~3, interrupt identification
    if ( IIRValue == IIR_RLS )  {       // Receive Line Status 
        LSRValue = LPC_UART0->LSR;
        // Receive Line Status 
        if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) ) {
            // There are errors or break interrupt 
            // Read LSR will clear the interrupt 
            UART_desc.lsr = LSRValue;
            Dummy = LPC_UART0->RBR;             // Dummy read on RX to clear 
                                                //      interrupt, then bail out 
            return;
        }
        if ( LSRValue & LSR_RDR )       { // Receive Data Ready 
            // Int-t is triggered when there is unread characters in RX FIFO
            // If no error on RLS, normal ready, save into the data buffer.
            // Note: reading RBR will clear the interrupt 

            readflag = 1;
        }
    } else if ( IIRValue == IIR_RDA )   { // Receive Data Available 
        // Int-t is triggered when characters level in RX FIFO reaches treshold 
        // сейчас установлен порог в 1 символ, в дальнейшем можно увеличить
        readflag = 1;
    } else if ( IIRValue == IIR_CTI ) { // Character timeout indicator 
        UART_desc.lsr |= 0x100;              // Bit 9 as the CTI error 
    } else if ( IIRValue == IIR_THRE ) {      // THRE, transmit holding register empty 
        // THRE interrupt 
        // Прерывание возникает при опустошении FIFO буфера
        LSRValue = LPC_UART0->LSR;     // Check status in the LSR to see if valid data in U0THR or not 
        if ( LSRValue & LSR_THRE ) {
          UART_desc.txempty = 1;
        }       else {
          UART_desc.txempty = 0;
        }
    }
    if ( readflag ) {
        // Char receive
        Dummy = LPC_UART0->RBR;
        tn_queue_isend_polling(&que_uart_queue, (void*)(Dummy));
    }
}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
