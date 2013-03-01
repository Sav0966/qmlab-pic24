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
     Description: TNKernel resources description
    
   ============================================================================================*/

#ifndef _TNRES_H
#define _TNRES_H

/* RAM allocation for tasks */

#define TMR_TASK_STACK_SIZE         256
#define IDL_TASK_STACK_SIZE         256

#define USER_TASK_1_STACK_SIZE      128
#define USER_TASK_2_STACK_SIZE      128
#define USER_TASK_3_STACK_SIZE      256
#define USER_TASK_4_STACK_SIZE      128
#define UART_TASK_STACK_SIZE        256

/* User Task Priorities */

#if defined(__dsPIC33F__) || defined(__PIC24F__)
    #define UART_TASK_PRIORITY      10
    #define USER_TASK_1_PRIORITY    11
    #define USER_TASK_2_PRIORITY    12
    #define USER_TASK_3_PRIORITY    13
    #define USER_TASK_4_PRIORITY    14
#elif defined(__PIC32MX__)
    #define UART_TASK_PRIORITY      26
    #define USER_TASK_1_PRIORITY    27
    #define USER_TASK_2_PRIORITY    28
    #define USER_TASK_3_PRIORITY    29
    #define USER_TASK_4_PRIORITY    30
#elif (defined(__ARMCC_VERSION) && (__TARGET_ARCH_ARM == 0) && (__TARGET_ARCH_THUMB == 4))
    #define UART_TASK_PRIORITY      26
    #define USER_TASK_1_PRIORITY    27
    #define USER_TASK_2_PRIORITY    28
    #define USER_TASK_3_PRIORITY    29
    #define USER_TASK_4_PRIORITY    30
#endif

#define BTN_QUEUE_DEPTH             8
#define BTN_QUEUE_SIZE              sizeof(TN_UWORD)

#define UART_QUEUE_DEPTH            16


/* Functions prototypes */

void app_conf(void);
void int_conf(void);
void idle_handler(void);

void TN_TASK User_Task1(void *par);
void TN_TASK User_Task2(void *par);
void TN_TASK User_Task3(void *par);
void TN_TASK User_Task4(void *par);
void TN_TASK UART_Task(void *par);

extern TN_DQUE  que_uart_queue TN_DATA;


#endif  /* _TNRES_H */
/* end of file ---------------------------------------------------------------------------------- */

