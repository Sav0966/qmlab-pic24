/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        tn_rtn_cwac.c
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

#include "..\\common\\cm_extr.h"            /* common routines         */
#include "..\\system\\sy_extr.h"            /* system externals        */

void task_to_non_runnable(TN_TCB_S *task);

/**
 *
 * @param wait_que
 * @param wait_reason
 * @param timeout
 */
void task_curr_to_wait_action (CDLL_QUEUE_S *wait_que, TN_UWORD wait_reason, TN_TIMEOUT timeout)
{
    task_to_non_runnable(tn_curr_run_task);

    tn_curr_run_task->task_state       = TSK_STATE_WAIT;
    tn_curr_run_task->task_wait_reason = (TN_TASK_WAIT_REASON)wait_reason;
    tn_curr_run_task->tick_count       = timeout;

    if (wait_que == TN_NULL) /* -- Thanks to Vavilov D.O. */
    {
        queue_reset(&(tn_curr_run_task->task_queue));
    }
    else
    {
        queue_add_tail(wait_que, &(tn_curr_run_task->task_queue));
        tn_curr_run_task->pwait_queue = wait_que;
    }

    if (timeout != TN_WAIT_INFINITE)
    {
        queue_add_tail(&tn_wait_timeout_list, &(tn_curr_run_task->timer_queue));
    }
}


#undef  TN_SOURCE_FILE
