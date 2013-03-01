/* ==============================================================================================
     Project:         TNKernel RTOS Example
     Project desc.:   

   ----------------------------------------------------------------------------------------------
     File:            main.c
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
     Description: Test application
    
     Пример предназначен для того, чтобы запустить его на отладочной плате Explorer 16
     с соответствующим процессорным модулем: версия для PIC24/dsPIC работает с модулями 
     MA240011 (PIC24FJ128GA010) и MA330012 (dsPIC33FJ256GP710), которые входят в комплект поставки
     Explorer 16, а версия TNKernel для PIC32 работает с модулем MA320001 (PIC32MX360F512).
     
     В примере используется четыре пользовательских задачи.
     Первая задача User_Task1 занимается только тем, что включает и выключает светодиод D3 с 
     периодичностью, которая зависит от положения потенциометра R6.
     Вторая задача User_Task2 ожидает сообщения от обработчика прерывания кнопки S6. Если кнопка
     нажимается, задача включает светодид, если отпускается - выключает. С помощью этой задачи
     проверяются асинхронные прерывания.
     Задачи User_Task3 и User_Task4 обмениваются семафорами, поочередно зажигая светодиоды D5 и D6.

   ============================================================================================*/

#include <string.h>

#include "bsp.h"            /* BSP      */

#include <tnkernel.h>       /* TNK      */
#include "tnres.h"          /* ресурсы  */

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
/* Global variables */

TN_TCB      tcb_User_Task1 TN_DATA;
TN_TCB      tcb_User_Task2 TN_DATA;
TN_TCB      tcb_User_Task3 TN_DATA;
TN_TCB      tcb_User_Task4 TN_DATA;
TN_TCB      tcb_UART_Task  TN_DATA;

TN_UWORD    stk_Tmr_Task[TMR_TASK_STACK_SIZE]       TN_DATA;
TN_UWORD    stk_Idl_Task[IDL_TASK_STACK_SIZE]       TN_DATA;
TN_UWORD    stk_User_Task1[USER_TASK_1_STACK_SIZE]  TN_DATA;
TN_UWORD    stk_User_Task2[USER_TASK_2_STACK_SIZE]  TN_DATA;
TN_UWORD    stk_User_Task3[USER_TASK_3_STACK_SIZE]  TN_DATA;
TN_UWORD    stk_User_Task4[USER_TASK_4_STACK_SIZE]  TN_DATA;
TN_UWORD    stk_UART_Task[UART_TASK_STACK_SIZE]     TN_DATA;

TN_FMP      fmp_btn_queue TN_DATA;
TN_UWORD    fmp_btn_queue_mem[BTN_QUEUE_DEPTH * (MAKE_ALIG(BTN_QUEUE_SIZE) / sizeof(TN_UWORD))] TN_DATA;
TN_DQUE     que_btn_queue TN_DATA;
void        *que_btn_queue_mem[BTN_QUEUE_DEPTH] TN_DATA;

TN_SEM      sem_Led_Exchange    TN_DATA;

TN_UWORD    user_idle_cnt;

unsigned char  uart_tx_buf[16];
unsigned char  uart_rx_buf[16];
unsigned int   rx_indx;

TN_DQUE        que_uart_queue TN_DATA;
void           *que_uart_queue_mem[UART_QUEUE_DEPTH] TN_DATA;


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------


/** 
 *  
 * @param void
 * @return int
 */
int main (void)
{
#if defined(__CORTEX_M3_RVTC__)
    tn_arm_disable_interrupts();
    Sys_Conf();
    Sys_Tmr_Conf();
#endif
    tn_start_system(stk_Tmr_Task, TMR_TASK_STACK_SIZE,
                    stk_Idl_Task, IDL_TASK_STACK_SIZE,
                    app_conf,
                    int_conf,
                    idle_handler
                   );
    return 1;
}




/**
 * 
 *
 */
void app_conf (void)
{
#if !defined(__CORTEX_M3_RVTC__)
    Sys_Conf();    
    Sys_Tmr_Conf();
#endif


    Btn_Conf();
    Led_Conf();
    Pot_Conf();

    tn_task_create(&tcb_User_Task1,
                   User_Task1,
                   USER_TASK_1_PRIORITY,
                   stk_User_Task1,
                   USER_TASK_1_STACK_SIZE,
                   TN_NULL,
                   TN_TASK_START_ON_CREATION
                  );

    tn_task_create(&tcb_User_Task2,
                   User_Task2,
                   USER_TASK_2_PRIORITY,
                   stk_User_Task2,
                   USER_TASK_2_STACK_SIZE,
                   TN_NULL,
                   TN_TASK_START_ON_CREATION
                  );

    tn_task_create(&tcb_User_Task3,
                   User_Task3,
                   USER_TASK_3_PRIORITY,
                   stk_User_Task3,
                   USER_TASK_3_STACK_SIZE,
                   TN_NULL,
                   TN_TASK_START_ON_CREATION
                  );

    tn_task_create(&tcb_User_Task4,
                   User_Task4,
                   USER_TASK_4_PRIORITY,
                   stk_User_Task4,
                   USER_TASK_4_STACK_SIZE,
                   TN_NULL,
                   TN_TASK_START_ON_CREATION
                  );

    tn_task_create(&tcb_UART_Task,
                   UART_Task,
                   UART_TASK_PRIORITY,
                   stk_UART_Task,
                   UART_TASK_STACK_SIZE,
                   TN_NULL,
                   TN_TASK_START_ON_CREATION
                  );

    tn_queue_create(&que_uart_queue, que_uart_queue_mem, UART_QUEUE_DEPTH);

    Uart_Conf();

    tn_sem_create(&sem_Led_Exchange, TN_TRUE, TN_TRUE);

    tn_fmem_create(&fmp_btn_queue, fmp_btn_queue_mem, BTN_QUEUE_SIZE, BTN_QUEUE_DEPTH);
    tn_queue_create(&que_btn_queue, que_btn_queue_mem, BTN_QUEUE_DEPTH);

}





/**
 *
 */
void int_conf(void)
{
    Sys_Tmr_Int_En();
    Btn_Int_En();
    Uart_Int_Conf();

    Sys_Int_Conf();
}




/**
 * Функция, которая периодически вызывается из задачи IDLE
 *  
 */
void idle_handler (void)
{
    tn_sys_enter_critical();
    user_idle_cnt++;
    tn_sys_exit_critical();
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------



/**
 *
 * @param par
 */
void TN_TASK User_Task1 (void *par)
{
    for (;;)
    {
        TN_TIMEOUT  delay = Pot_State_Get();
        delay = delay > 20 ? delay : 20;

        Led_On(LED_1);
        tn_task_sleep(delay);
        Led_Off(LED_1);
        tn_task_sleep(delay);
    }
}






void TN_TASK User_Task2 (void *par)
{
    unsigned int *msg;
    for (;;)
    {
        tn_queue_receive(&que_btn_queue, (void*)&msg, TN_WAIT_INFINITE);
        if (*msg == 0)
            Led_Off(LED_2);
        else
            Led_On(LED_2);
        tn_fmem_release(&fmp_btn_queue, msg);
    }
}




void TN_TASK User_Task3 (void *par)
{
    for (;;)
    {
        tn_sem_acquire(&sem_Led_Exchange, TN_WAIT_INFINITE);
        Led_On(LED_3);
        tn_task_sleep(100);
        Led_Off(LED_3);
        tn_sem_signal(&sem_Led_Exchange);
    }    
}

void TN_TASK User_Task4 (void *par)
{
    for (;;)
    {
        tn_sem_acquire(&sem_Led_Exchange, TN_WAIT_INFINITE);
        Led_On(LED_4);
        tn_task_sleep(100);
        Led_Off(LED_4);
        tn_sem_signal(&sem_Led_Exchange);        
    }
}


/**
 * 
 *
 * @param par 
 * 
 * @return void TN_TASK 
 */
void TN_TASK UART_Task (void *par)
{
    unsigned int bt;

    Uart_En();
    for (;;)
    {
        if (tn_queue_receive(&que_uart_queue, (void*)&bt, 2) == TERR_TIMEOUT)
        {
            rx_indx = 0;
        }
        else
        {
            uart_rx_buf[rx_indx++] = bt;
            if (rx_indx > 9)
            {
                memcpy(uart_tx_buf, uart_rx_buf, 10);
                rx_indx = 0;
                Uart_Send(uart_tx_buf, 10);
            }
        }
    }
}



//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

/**
 * 
 */

#if defined(__dsPIC33F__) || defined(__PIC24F__)
    tn_sys_interrupt(_T2Interrupt)
#elif defined(__PIC32MX__)
    tn_sys_interrupt(_CORE_TIMER_VECTOR)
#elif (defined(__ARMCC_VERSION) && (__TARGET_ARCH_ARM == 0) && (__TARGET_ARCH_THUMB == 4))
    tn_sys_interrupt(SysTick_Handler)
#endif
{
    Sys_Tmr_Int_Handler();
    tn_tick_int_processing();
}


/**
 * 
 */

#if defined(__dsPIC33F__) || defined(__PIC24F__)
    tn_sys_interrupt(_CNInterrupt)
#elif defined(__PIC32MX__)
    tn_sys_interrupt(_CHANGE_NOTICE_VECTOR)
#elif (defined(__ARMCC_VERSION) && (__TARGET_ARCH_ARM == 0) && (__TARGET_ARCH_THUMB == 4))
    tn_sys_interrupt(EINT3_IRQHandler)
#endif
{
    unsigned int *mem;
    
    if (Btn_State_Get())
    {
        if (tn_fmem_get_ipolling(&fmp_btn_queue, (void*)&mem) ==  TERR_NO_ERR)
        {
            *mem = 1;
            tn_queue_isend_polling(&que_btn_queue, mem);
        }
    }
    else
    {
        if (tn_fmem_get_ipolling(&fmp_btn_queue, (void*)&mem) ==  TERR_NO_ERR)
        {
            *mem = 0;
            tn_queue_isend_polling(&que_btn_queue, mem);
        }
    }
    Btn_Int_Handler();
}




/**
 * 
 */

#if defined(__dsPIC33F__) || defined(__PIC24F__)
tn_sys_interrupt(_U2RXInterrupt)
#elif defined(__PIC32MX__)
tn_sys_interrupt(_UART_2_VECTOR)
#elif (defined(__ARMCC_VERSION) && (__TARGET_ARCH_ARM == 0) && (__TARGET_ARCH_THUMB == 4))
tn_sys_interrupt(UART0_IRQHandler)
#endif
{
    Uart_RX_Int_Handler();
}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------


