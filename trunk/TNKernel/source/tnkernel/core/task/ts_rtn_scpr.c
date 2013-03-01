/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        ts_rtn_scpr.c
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

     /* core depended functions */


TN_BOOL  change_running_task_priority(TN_TCB_S *task, TN_UWORD new_priority);
TN_UWORD find_max_blocked_priority(TN_MUTEX_S *mutex, TN_UWORD ref_priority);

/**
 *
 */
TN_BOOL set_current_priority (TN_TCB_S * task, TN_UWORD priority)
{
    TN_MUTEX_S *mutex;
    TN_TCB_S   *curr_task;
    TN_BOOL     rc = TN_FALSE;
    TN_UWORD   old_priority;
    TN_UWORD   new_priority;

    /* Transitive priority changing

       if we have a task A that is blocked by the task B and we changed priority of task A,priority of task B also
       have to be changed. I.e, if we have the task A that is waiting for the mutex M1 and we changed priority
       of this task, a task B that holds a mutex M1 now, also needs priority's changing.
       Then, if a task B now is waiting for the mutex M2, the same procedure have to be applied to the task C
       that hold a mutex M2 now
    */

    curr_task    = task;
    new_priority = priority;

    for (;;)
    {
        old_priority = curr_task->priority;

        if (old_priority == new_priority)
            break;

        if (curr_task->task_state == TSK_STATE_RUNNABLE)
        {
            rc = change_running_task_priority(curr_task, new_priority);
            break;
        }
        else if (curr_task->task_state & TSK_STATE_WAIT)
        {

            if (curr_task->task_wait_reason == TSK_WAIT_REASON_MUTEX_I ||
                curr_task->task_wait_reason == TSK_WAIT_REASON_MUTEX_C ||
                curr_task->task_wait_reason == TSK_WAIT_REASON_MUTEX_C_BLK)
            {
                mutex = get_mutex_by_task_wait_queue(curr_task->pwait_queue);

                if (curr_task->task_wait_reason == TSK_WAIT_REASON_MUTEX_C_BLK)
                {
                    curr_task = curr_task->blk_task;
                }
                else
                {
                    curr_task = mutex->holder;
                }

                if (old_priority < new_priority)
                {
                    new_priority = find_max_blocked_priority(mutex, curr_task->base_priority);
                }
            }
            else
            {
                task->priority = new_priority;
                break;
            }
        }
        else
        {
            task->priority = new_priority;
            break;
        }
    }

    return rc;
}

#undef  TN_SOURCE_FILE
