/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        sy_dat.c
 *  Compiler:    Microchip C30 v.2.xx - 3.11 (PIC24/dsPIC)
 *
 *  Ver. 2.5
 *
 *  Copyright � 2004,2008 Yuri Tiomkin
 *  Copyright � 2008 Alex Borisov (PIC24/dsPIC port)
 *
 *  All rights reserved.
 *
 *  Permission to use, copy, modify, and distribute this software in source and binary forms and its documentation
 *  for any purpose and without fee is hereby granted, provided that the above copyright notice appear in all copies
 *  and that both that copyright notice and this permission notice appear in supporting documentation.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL YURI TIOMKIN OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 *  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#define TN_SOURCE_FILE

#include "..\\..\\tnkernel.h"


volatile TN_STATE      tn_system_state TN_DATA;
volatile TN_CONTEXT    tn_sys_context  TN_DATA;

volatile TN_UWORD      tn_created_tasks_qty TN_DATA;

         TN_UWORD      tn_tslice_ticks[TN_NUM_PRIORITY] TN_DATA;

         CDLL_QUEUE_S  tn_create_queue TN_DATA;
         CDLL_QUEUE_S  tn_ready_list[TN_NUM_PRIORITY] TN_DATA;

volatile TN_BOOL       tn_context_switch_request TN_DATA;
volatile TN_BOOL       tn_enable_switch_context  TN_DATA;

volatile TN_IDLE_CNT_T tn_idle_count TN_DATA;

volatile TN_SYS_TIM_T  tn_sys_time_count TN_DATA;                       /* 2.4.5 */

volatile TN_PRTY_BMP_T tn_ready_to_run_bmp TN_DATA;

         TN_TCB_S      *tn_curr_run_task    TN_DATA;
         TN_TCB_S      *tn_next_task_to_run TN_DATA;

         CDLL_QUEUE_S  tn_wait_timeout_list TN_DATA;

         TN_TCB_S      tn_timer_task TN_DATA;
         TN_TCB_S      tn_idle_task  TN_DATA;

         void          (*appl_init_callback)(void)          TN_DATA;    /* Pointer to user callback app init function */
         void          (*cpu_interrupt_enbl_callback)(void) TN_DATA;    /* Pointer to user interrupt enable function  */
         void          (*idle_user_func_callback)(void)     TN_DATA;    /* Pointer to user idle loop function         */

volatile TN_UWORD      tn_int_counter TN_DATA;

#undef  TN_SOURCE_FILE
