/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        ts_rtn_wcmp.c
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


void         task_to_runnable(TN_TCB_S *task);
TN_BOOL      set_current_priority(TN_TCB_S *task, TN_UWORD priority);
TN_UWORD     find_max_blocked_priority(TN_MUTEX_S *mutex, TN_UWORD ref_priority);

/**
 *
 * @param task
 * @param tqueue_remove_enable
 *
 * @return TN_BOOL
 */
TN_BOOL  task_wait_complete (TN_TCB_S *task, TN_BOOL tqueue_remove_enable)
{
    TN_UWORD     curr_priority;
    TN_BOOL      fl_mutex;
    TN_MUTEX_S   *mutex;
    TN_TCB_S     *tmp_task;
    TN_BOOL       rc;
    CDLL_QUEUE_S *t_que;

    if (task == TN_NULL)
        return TN_FALSE;

    rc    = TN_FALSE;
    t_que = TN_NULL;

    if (task->task_wait_reason == TSK_WAIT_REASON_MUTEX_I     ||
        task->task_wait_reason == TSK_WAIT_REASON_MUTEX_C     ||
        task->task_wait_reason == TSK_WAIT_REASON_MUTEX_C_BLK
       )
    {
        fl_mutex = TN_TRUE;
        t_que = task->pwait_queue;
    }
    else
    {
        fl_mutex = TN_FALSE;
    }

    if(tqueue_remove_enable)
        queue_remove_entry(&(task->task_queue));

    task->pwait_queue      = TN_NULL;
    task->task_wait_rc     = TERR_NO_ERR;

    if (task->tick_count != TN_WAIT_INFINITE)
        queue_remove_entry(&(task->timer_queue));

    task->tick_count = TN_WAIT_INFINITE;

    if (!(task->task_state & TSK_STATE_SUSPEND))
    {
        task_to_runnable(task);
        rc = TN_TRUE;
    }
    else
    {
        task->task_state = TSK_STATE_SUSPEND;
    }

    if (fl_mutex)
    {
        mutex = get_mutex_by_task_wait_queue(t_que);

        if (task->task_wait_reason == TSK_WAIT_REASON_MUTEX_C_BLK)
        {
            tmp_task = task->blk_task;

            if(queue_contains_entry(&tn_blocked_tasks_list, &(task->block_queue)))
                queue_remove_entry(&(task->block_queue));
        }
        else
        {
            tmp_task = mutex->holder;
        }

        if (tmp_task != TN_NULL)
        {
            if ((tmp_task->priority != tmp_task->base_priority) &&
                (tmp_task->priority == task->priority)
               )
            {
                curr_priority = find_max_blocked_priority(mutex, tmp_task->base_priority);
                if (set_current_priority(tmp_task,curr_priority))
                {
                    rc = TN_TRUE;
                }
            }
        }
    }

    task->task_wait_reason = TSK_WAIT_REASON_NO;
    return rc;
}


#undef  TN_SOURCE_FILE
