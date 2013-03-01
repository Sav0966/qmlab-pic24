/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        mx_fce_lock.c
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


TN_BOOL enable_lock_mutex(TN_TCB_S *curr_task, TN_TCB_S **blk_task);
TN_BOOL set_current_priority(TN_TCB_S *task, TN_UWORD priority);
void    task_curr_to_wait_action(CDLL_QUEUE_S *wait_que, TN_UWORD wait_reason, TN_TIMEOUT timeout);

/**
 *
 * @param mutex
 * @param timeout
 *
 * @return TN_RETVAL
 */
TN_RETVAL tnec_mutex_lock (TN_MUTEX_S *mutex, TN_TIMEOUT timeout)
{
    TN_UWORD  tn_save_status_reg TN_UNUSED;     /* for SR save */
    TN_TCB_S *blk_task;
    TN_RETVAL       rc;

    if (mutex == TN_NULL || timeout == 0)
        return TERR_WRONG_PARAM;

    if (mutex->id_mutex != TN_ID_MUTEX)
        return TERR_NOEXS;

    if (tn_is_non_task_context())
    {
        return TERR_WCONTEXT;
    }

    tn_disable_interrupt();

    rc = TERR_NO_ERR;

    for (;;)
    {
        if (tn_curr_run_task == mutex->holder)
        {
            rc = TERR_ILUSE;
            break;
        }

        if (mutex->attr == TN_MUTEX_ATTR_CEILING &&
            tn_curr_run_task->base_priority < mutex->ceil_priority)
        {
            rc = TERR_ILUSE;
            break;
        }

        if (mutex->attr == TN_MUTEX_ATTR_CEILING)
        {
            if (mutex->holder == TN_NULL)
            {
                if (enable_lock_mutex(tn_curr_run_task, &blk_task))
                {
                    mutex->holder = tn_curr_run_task;
                    tn_curr_run_task->blk_task = TN_NULL;

                    queue_add_tail(&(tn_curr_run_task->mutex_queue), &(mutex->mutex_queue));
                    queue_add_tail(&tn_locked_mutexes_list, &(mutex->lock_mutex_queue));
                }
                else
                {
                    /*
                       Невозможно заблокировать мютекс

                       Протокол наследования:
                       Если текущий приоритет выполняемой задачи больше приоритета задачи, которая блокирует текущую
                       (удерживает мютекс), то блокирующая задача наследует приоритет текущей
                    */

                    if (blk_task != TN_NULL)
                    {
                        if (tn_curr_run_task->priority < blk_task->priority)
                        {
                            set_current_priority(blk_task, tn_curr_run_task->priority);
                        }
                    }
                    queue_add_tail(&tn_blocked_tasks_list, &(tn_curr_run_task->block_queue));

                    tn_curr_run_task->blk_task = blk_task;

                    task_curr_to_wait_action(&(mutex->wait_queue), TSK_WAIT_REASON_MUTEX_C_BLK, timeout);

                    tn_enable_interrupt();
                    tn_switch_context();
                    return tn_curr_run_task->task_wait_rc;
                }
            }

            else
            {
                if (tn_curr_run_task->priority < mutex->holder->priority)
                {
                    set_current_priority(mutex->holder,tn_curr_run_task->priority);
                }

                task_curr_to_wait_action(&(mutex->wait_queue), TSK_WAIT_REASON_MUTEX_C, timeout);

                tn_enable_interrupt();
                tn_switch_context();
                return tn_curr_run_task->task_wait_rc;
            }
        }

        else if (mutex->attr == TN_MUTEX_ATTR_INHERIT)
        {
            if (mutex->holder == TN_NULL)
            {
                mutex->holder = tn_curr_run_task;

                queue_add_tail(&(tn_curr_run_task->mutex_queue),&(mutex->mutex_queue));
/* --- v.2.3 */
                queue_add_tail(&tn_locked_mutexes_list,&(mutex->lock_mutex_queue));
/* ---       */
                rc = TERR_NO_ERR;
            }
            else
            {
                if (tn_curr_run_task->priority < mutex->holder->priority)
                {
                    set_current_priority(mutex->holder, tn_curr_run_task->priority);
                }

                task_curr_to_wait_action(&(mutex->wait_queue), TSK_WAIT_REASON_MUTEX_I,timeout);

                tn_enable_interrupt();
                tn_switch_context();
                return tn_curr_run_task->task_wait_rc;
            }
        }

        break;
    }

    tn_enable_interrupt();
    return rc;
}

#undef  TN_SOURCE_FILE
