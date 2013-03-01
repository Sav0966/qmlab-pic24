/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        sy_fnc_stsy.c
 *  Compiler:    Microchip C30 v.2.xx - 3.11 (PIC24/dsPIC)
 *
 *  Ver. 2.5
 *
 *  Copyright © 2004,2008 Yuri Tiomkin
 *  Copyright © 2008 Alex Borisov (PIC24/dsPIC port)
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

#include "sy_extr.h"

#include "..\\common\\cm_extr.h"            /* common routines         */
#include "..\\task\\ts_extr.h"              /* task functions          */

//     /* core depended functions */


void tn_idle_task_func(void *par);
void tn_timer_task_func(void *par);
void task_to_runnable(TN_TCB_S * task);

/**
 *
 * @param timer_task_stack
 * @param timer_task_stack_size
 * @param idle_task_stack
 * @param idle_task_stack_size
 * @param app_in_cb
 * @param cpu_int_en
 * @param idle_user_cb
 */
void TN_NORETURN tnnc_start_system(TN_UWORD  *timer_task_stack,
                                   TN_UWORD   timer_task_stack_size,
                                   TN_UWORD  *idle_task_stack,
                                   TN_UWORD   idle_task_stack_size,
                                   void     (*app_in_cb)(void),
                                   void     (*cpu_int_en)(void),
                                   void     (*idle_user_cb)(void)
                                  )
{
    TN_WORD i;

    /* ToDo - initialize the sys log (if enabled) */

    for (i = 0; i < TN_NUM_PRIORITY; i++)
    {
        queue_reset(&(tn_ready_list[i]));
        tn_tslice_ticks[i] = NO_TIME_SLICE;
    }

    queue_reset(&tn_create_queue);
    tn_created_tasks_qty = 0;

    tn_system_state = TN_ST_STATE_NOT_RUN;

    tn_enable_switch_context  = TN_TRUE;
    tn_ready_to_run_bmp       = 0;

    tn_context_switch_request = TN_FALSE;
    tn_sys_context            = TN_CONTEXT_TASK;

    tn_next_task_to_run       = TN_NULL;
    tn_curr_run_task          = TN_NULL;

    //tn_sys_context            = TN_CONTEXT_TASK;

    queue_reset(&tn_locked_mutexes_list);
    queue_reset(&tn_blocked_tasks_list);
    queue_reset(&tn_wait_timeout_list);

    tnnc_task_create((TN_TCB_S*)&tn_timer_task,
                     tn_timer_task_func,
                     0,
                     timer_task_stack,
                     timer_task_stack_size,
                     TN_NULL,
                     TN_TASK_TIMER
                    );

    tnnc_task_create((TN_TCB_S*)&tn_idle_task,
                     tn_idle_task_func,
                     (TN_NUM_PRIORITY - 1),
                     idle_task_stack,
                     idle_task_stack_size,
                     TN_NULL,
                     TN_TASK_IDLE
                    );

    task_to_runnable(&tn_idle_task);
    task_to_runnable(&tn_timer_task);

    tn_curr_run_task = &tn_idle_task;

    appl_init_callback          = app_in_cb;
    cpu_interrupt_enbl_callback = cpu_int_en;
    idle_user_func_callback     = idle_user_cb;

    tn_start_exe();

    for(;;);

/*    for (;;);*/
}

#undef  TN_SOURCE_FILE

