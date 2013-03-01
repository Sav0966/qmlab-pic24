/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        mx_rtn_dulm.c
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


TN_BOOL set_current_priority(TN_TCB_S * task, TN_UWORD priority);
TN_UWORD find_max_blocked_priority(TN_MUTEX_S *mutex, TN_UWORD ref_priority);
TN_BOOL enable_lock_mutex(TN_TCB_S *curr_task, TN_TCB_S **blk_task);
void    remove_task_from_blocked_list(TN_TCB_S *task);
TN_BOOL task_wait_complete (TN_TCB_S *task, TN_BOOL tqueue_remove_enable);
TN_BOOL try_lock_mutex(TN_TCB_S *task);

/**
 *
 * @param mutex
 *
 * @return TN_BOOL
 */
TN_BOOL do_unlock_mutex (TN_MUTEX_S *mutex)
{
    CDLL_QUEUE_S *curr_que;
    TN_MUTEX_S   *tmp_mutex;
    TN_TCB_S     *task;
    TN_TCB_S     *hi_pri_task = TN_NULL;
    TN_TCB_S     *blk_task;
    TN_TCB_S     *tmp_task;
    TN_UWORD     priority;
    TN_UWORD     pr;
    TN_BOOL      need_switch_context = TN_FALSE;

    queue_remove_entry(&(mutex->mutex_queue));
    queue_remove_entry(&(mutex->lock_mutex_queue));

    if (is_queue_empty(&(tn_curr_run_task->mutex_queue)))
    {
        need_switch_context = set_current_priority(tn_curr_run_task, tn_curr_run_task->base_priority);
    }
    else
    {
        if (mutex->attr == TN_MUTEX_ATTR_INHERIT  ||
            mutex->attr == TN_MUTEX_ATTR_CEILING
           )
        {
            pr       = tn_curr_run_task->base_priority;
            curr_que = tn_curr_run_task->mutex_queue.next;

            for (;;)
            {
                tmp_mutex = get_mutex_by_mutex_queue(curr_que);
                pr        = find_max_blocked_priority(tmp_mutex,pr);

                if (curr_que->next == &(tn_curr_run_task->mutex_queue))
                    break;
                else
                {
                    curr_que = curr_que->next;
                }
            }

            if (pr != tn_curr_run_task->priority)
            {
                need_switch_context = set_current_priority(tn_curr_run_task,pr);
            }
        }
    }

    if (is_queue_empty(&(mutex->wait_queue)))
    {
        mutex->holder = TN_NULL;
        hi_pri_task   = tn_curr_run_task;
    }
    else
    {
        if (mutex->attr == TN_MUTEX_ATTR_CEILING)
        {
            priority = TN_NUM_PRIORITY - 1;
            curr_que = mutex->wait_queue.next;

            for (;;)
            {
                task = get_task_by_tsk_queue(curr_que);

                if (task->priority < priority)
                {
                    hi_pri_task = task;
                    priority    = task->priority;
                }
                if (curr_que->next == &(mutex->wait_queue))
                    break;
                else
                    curr_que = curr_que->next;
            }

            if (enable_lock_mutex(hi_pri_task,&blk_task))
            {
                queue_remove_entry(&(hi_pri_task->task_queue));
                remove_task_from_blocked_list(hi_pri_task);

                if (task_wait_complete(hi_pri_task, TN_FALSE))
                    need_switch_context = TN_TRUE;

                mutex->holder         = hi_pri_task;
                hi_pri_task->blk_task = TN_NULL;

                queue_add_tail(&(hi_pri_task->mutex_queue),&(mutex->mutex_queue));
                queue_add_tail(&tn_locked_mutexes_list,&(mutex->lock_mutex_queue));
            }
            else
            {
                if (!queue_contains_entry(&tn_blocked_tasks_list, &(hi_pri_task->block_queue)))
                {
                    queue_add_tail(&tn_blocked_tasks_list,&(hi_pri_task->block_queue));
                    hi_pri_task->task_wait_reason = TSK_WAIT_REASON_MUTEX_C_BLK;
                    hi_pri_task->blk_task         = blk_task;
                }
            }
        }
        else if (mutex->attr == TN_MUTEX_ATTR_INHERIT)
        {
            curr_que = queue_remove_head(&(mutex->wait_queue));
            task     = get_task_by_tsk_queue(curr_que);

            if (task_wait_complete(task, TN_FALSE))
                need_switch_context = TN_TRUE;

            mutex->holder = task;

            queue_add_tail(&(task->mutex_queue),&(mutex->mutex_queue));
/*--- v.2.3 */
            queue_add_tail(&tn_locked_mutexes_list,&(mutex->lock_mutex_queue));
/*---       */
        }
    }

    if (mutex->holder == TN_NULL && mutex->attr == TN_MUTEX_ATTR_CEILING)
    {
        if (!is_queue_empty(&tn_blocked_tasks_list))
        {
            curr_que = tn_blocked_tasks_list.next;
            for (;;)
            {
                tmp_task = get_task_by_block_queue(curr_que);
    
                if (tmp_task != hi_pri_task)
                {
                    if (try_lock_mutex(tmp_task))
                    {
                        need_switch_context = TN_TRUE;
                    }
                }
                if (curr_que->next == &tn_blocked_tasks_list)
                    break;
                else
                    curr_que = curr_que->next;
            }
        }
    }

    return need_switch_context;
}

#undef  TN_SOURCE_FILE
