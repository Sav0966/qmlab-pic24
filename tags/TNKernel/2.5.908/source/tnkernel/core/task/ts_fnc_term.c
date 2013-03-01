/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        tn_fnc_term.c
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

TN_BOOL do_unlock_mutex(TN_MUTEX_S *mutex);
void task_to_runnable(TN_TCB_S *task);
void task_set_dormant_state(TN_TCB_S *task);
void task_to_non_runnable(TN_TCB_S *task);

/**
 *
 * @param task
 *
 * @return TN_RETVAL
 */
TN_RETVAL tnnc_task_terminate (TN_TCB_S *task)
{
    TN_UWORD       tn_save_status_reg TN_UNUSED;     /* for SR save */
    TN_RETVAL      rc;
    TN_UWORD      *ptr_stack;
    CDLL_QUEUE_S  *que;
    TN_MUTEX_S    *mutex;

/*
    Not check parameter error

    if (task == TN_NULL)
        return TERR_WRONG_PARAM;

*/

    if (task->id_task != TN_ID_TASK)
        return TERR_NOEXS;

    if (tn_is_non_task_context())
    {
        return TERR_WCONTEXT;
    }

    tn_disable_interrupt();

    rc = TERR_NO_ERR;

    if ((task->task_state == TSK_STATE_DORMANT) || (tn_curr_run_task == task))
    {
        rc = TERR_WCONTEXT;
    }
    else
    {
        if (task->task_state == TSK_STATE_RUNNABLE)
        {
            task_to_non_runnable(task);
        }
        else if (task->task_state & TSK_STATE_WAIT)
        {
            queue_remove_entry(&(task->task_queue));

            if (queue_contains_entry(&tn_blocked_tasks_list, &(task->block_queue)))
            {
                queue_remove_entry(&(task->block_queue));
            }
            if (task->tick_count != TN_WAIT_INFINITE)
            {
                queue_remove_entry(&(task->timer_queue));
            }
        }


        while (!is_queue_empty(&(task->mutex_queue)))
        {
            que   = queue_remove_head(&(task->mutex_queue));
            mutex = get_mutex_by_mutex_queue(que);
            do_unlock_mutex(mutex);
        }

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
            tn_enable_interrupt();

            if ((tn_next_task_to_run != tn_curr_run_task) && (tn_enable_switch_context != TN_FALSE))
            {
                tn_switch_context();
            }
            return TERR_NO_ERR;
        }
    }
    tn_enable_interrupt();
    return rc;
}


#undef  TN_SOURCE_FILE

