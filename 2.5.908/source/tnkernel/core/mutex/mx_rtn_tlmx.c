/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        mx_rtn_tlmx.c
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


TN_BOOL enable_lock_mutex(TN_TCB_S *curr_task, TN_TCB_S ** blk_task);
TN_BOOL task_wait_complete(TN_TCB_S *task, TN_BOOL tqueue_remove_enable);

/**
 *
 * @param task
 *
 * @return TN_BOOL
 */
TN_BOOL try_lock_mutex (TN_TCB_S * task)
{
    TN_MUTEX_S *mutex;
    TN_BOOL     need_switch_context;

    need_switch_context = TN_FALSE;
    mutex = get_mutex_by_task_wait_queue(task->pwait_queue);

    if (mutex == TN_NULL)
        return need_switch_context;

    if (mutex->holder == TN_NULL)
    {
        if (enable_lock_mutex(task, TN_NULL))
        {
            queue_remove_entry(&(task->task_queue));
            queue_remove_entry(&(task->block_queue));

            if (task_wait_complete(task, TN_FALSE))
                need_switch_context = TN_TRUE;

            mutex->holder  = task;
            task->blk_task = TN_NULL;

            queue_add_tail(&(task->mutex_queue),&(mutex->mutex_queue));
            queue_add_tail(&tn_locked_mutexes_list,&(mutex->lock_mutex_queue));
        }
    }

    return need_switch_context;
}

#undef  TN_SOURCE_FILE
