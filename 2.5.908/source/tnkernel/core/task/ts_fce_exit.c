/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        ts_fce_exit.c
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
//     /* core depended functions */


TN_BOOL  do_unlock_mutex(TN_MUTEX_S *mutex);
void     task_to_runnable(TN_TCB_S *task);
void     task_set_dormant_state(TN_TCB_S *task);
void     task_to_non_runnable(TN_TCB_S *task);

/**
 *
 * @param attr
 */
void tnec_task_exit (TN_UWORD attr)
{
    TN_UWORD       tn_save_status_reg TN_UNUSED;     /* for SR save */
    TN_TCB_S      *task;
    CDLL_QUEUE_S  *que;
    TN_MUTEX_S    *mutex;
    TN_UWORD      *ptr_stack;

    if (tn_is_non_task_context())
        return;

    tn_save_sr();               /* just for PIC24/dsPIC */
    tn_disable_interrupt();

    while (!is_queue_empty(&(tn_curr_run_task->mutex_queue)))
    {
        que   = queue_remove_head(&(tn_curr_run_task->mutex_queue));
        mutex = get_mutex_by_mutex_queue(que);
        do_unlock_mutex(mutex);
    }

    task = tn_curr_run_task;

    task_to_non_runnable(tn_curr_run_task);
    task_set_dormant_state(task);

    ptr_stack = tn_stack_init(task->task_func_addr,
                              task->stk_start,
                              task->stk_size,
                              task->task_func_param
                             );

    task->task_stk = ptr_stack;

    if (task->activate_count > 0)
    {
        task->activate_count--;
        task_to_runnable(task);
    }
    else
    {
        /* Alex B. - delete task only if activate counter = 0 ! */

        if (attr == TN_EXIT_AND_DELETE_TASK)
        {
            queue_remove_entry(&(task->create_queue));
            tn_created_tasks_qty--;
            task->id_task = TN_ID_UNKNOWN;
        }
    }

    tn_switch_context_exit(tn_save_status_reg); /* Exit from task with restore saved SR*/
}

#undef  TN_SOURCE_FILE
