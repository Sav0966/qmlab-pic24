/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        ts_rtn_crpr.c
 *  Compiler:    Microchip C30 v.2.xx - 3.11 (PIC24/dsPIC)
 *
 *  Ver. 2.5
 *
 *  Copyright � 2004,2008 Yuri Tiomkin
 *  Copyright � 2008 Alex Borisov (PIC24/dsPIC port)
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


void find_next_task_to_run(void);

/**
 *
 * @param task
 * @param new_priority
 *
 * @return TN_BOOL
 */
TN_BOOL change_running_task_priority (TN_TCB_S *task, TN_UWORD new_priority)
{
    TN_BOOL        rc = TN_FALSE;
    TN_UWORD       old_priority;
    CDLL_QUEUE_S  *que;

    old_priority = task->priority;
    que          = &(tn_ready_list[old_priority]);

    queue_remove_entry(&(task->task_queue));

    if (is_queue_empty(que))
    {
        tn_ready_to_run_bmp &= ~(1 << old_priority);
    }

    task->priority = new_priority;

    queue_add_tail(&(tn_ready_list[new_priority]), &(task->task_queue));

    tn_ready_to_run_bmp |= (1 << new_priority);

    if (tn_next_task_to_run == task)
    {
        if (new_priority >= old_priority)  /* new less or the same as old */
        {
            find_next_task_to_run();
            if (tn_next_task_to_run != task)
            {
                rc = tn_enable_switch_context;
            }
        }
    }
    else
    {
        if (new_priority < tn_next_task_to_run->priority) /* ���� '<' , ��� ��� �����c��� ��������� - 0 */
        {
            tn_next_task_to_run = task;
            rc = tn_enable_switch_context;
        }
    }
    return rc;
}

#undef  TN_SOURCE_FILE
