/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        sy_fnc_tiip.c
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
//     /* core depended functions */
/**
 *
 */
void  tnnc_tick_int_processing (void)
{
    volatile TN_UWORD           tn_save_status_reg TN_UNUSED;
    volatile CDLL_QUEUE_S *curr_que;
    volatile CDLL_QUEUE_S *pri_queue;
    volatile TN_UWORD           priority;

    if (tn_is_non_sys_int_context())
    {
        return;
    }

    tn_idisable_interrupt();

    priority  = tn_curr_run_task->priority;

    if (tn_tslice_ticks[priority] != NO_TIME_SLICE)
    {
        tn_curr_run_task->tslice_count++;

        if (tn_curr_run_task->tslice_count > tn_tslice_ticks[priority])
        {
            tn_curr_run_task->tslice_count = 0;

            pri_queue = &(tn_ready_list[priority]);

            if (!(is_queue_empty((CDLL_QUEUE_S *)pri_queue)) && pri_queue->next->next != pri_queue)
            {
                curr_que = queue_remove_tail(&(tn_ready_list[priority]));
                queue_add_head(&(tn_ready_list[priority]), (CDLL_QUEUE_S *)curr_que);
            }
        }
    }

    tn_sys_time_count++;        /* increment system timer */    /* 2.4.5 */

    /* v. 2.3 */

    queue_remove_entry(&(tn_timer_task.task_queue));
    tn_timer_task.task_wait_reason = TSK_WAIT_REASON_NO;
    tn_timer_task.task_state       = TSK_STATE_RUNNABLE;
    tn_timer_task.pwait_queue      = TN_NULL;
    tn_timer_task.task_wait_rc     = TERR_NO_ERR;

    queue_add_tail(&(tn_ready_list[0]), &(tn_timer_task.task_queue));
    tn_ready_to_run_bmp |= 1;

    tn_next_task_to_run = &tn_timer_task;
    tn_context_switch_request = TN_TRUE;

    tn_ienable_interrupt();
}

#undef  TN_SOURCE_FILE

