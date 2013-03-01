/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        tn_fnc_crte.c
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

void task_to_runnable(TN_TCB_S *task);
void task_set_dormant_state(TN_TCB_S *task);

/**
 *
 * @param task
 * @param task_func
 * @param priority
 * @param task_stack_start
 * @param task_stack_size
 * @param param
 * @param option
 *
 * @return TN_RETVAL
 */
TN_RETVAL tnnc_task_create (TN_TCB_S      *task,
                            void         (*task_func)(void *param),
                            TN_UWORD       priority,
                            TN_UWORD      *task_stack_start,
                            TN_UWORD       task_stack_size,
                            void          *param,
                            TN_UWORD       option
                           )
{
    TN_UWORD   TN_UNUSED tn_save_status_reg = 0;     /* for SR save */
    TN_WORD    i;
    TN_UWORD  *ptr_stack;
/*
    Not check parameter error

    if ((priority == 0 && ((option & TN_TASK_TIMER) == 0))              ||
        (priority == TN_NUM_PRIORITY-1 && (option & TN_TASK_IDLE) == 0)
       )
    {
        return TERR_WRONG_PARAM;
    }

    if (( priority        > TN_NUM_PRIORITY - 1) ||
         task_stack_size  < TN_MIN_STACK_SIZE    ||
         task_func        == TN_NULL             ||
         task             == TN_NULL             ||
         task_stack_start == TN_NULL
       )
    {
        return TERR_WRONG_PARAM;
    }
*/


    if (task->id_task != 0)
        return TERR_EXS;

    if (tn_is_non_task_context())
    {
        return TERR_WCONTEXT;
    }

    if (tn_system_state == TN_ST_STATE_RUNNING)
    {
        tn_disable_interrupt();
    }

    task->task_func_addr  = (void *)task_func;
    task->task_func_param = param;
    task->stk_start       = (TN_UWORD*)task_stack_start;
    task->stk_size        = task_stack_size;
    task->base_priority   = priority;
    task->activate_count  = 0;
    task->id_task         = TN_ID_TASK;

    for (ptr_stack = task->stk_start, i = 0; i < task->stk_size; i++)
    {
        *ptr_stack++ = TN_FILL_STACK_VAL;
    }

    task_set_dormant_state(task);

    ptr_stack = tn_stack_init(task->task_func_addr,
                              task->stk_start,
                              task->stk_size,
                              task->task_func_param
                             );

    task->task_stk = ptr_stack;

    queue_add_tail(&tn_create_queue, &(task->create_queue));
    tn_created_tasks_qty++;

    if ((option & TN_TASK_START_ON_CREATION) != 0)
        task_to_runnable(task);

    if (tn_system_state == TN_ST_STATE_RUNNING)
    {
        tn_enable_interrupt();
    }

    return TERR_NO_ERR;
}

#undef  TN_SOURCE_FILE

