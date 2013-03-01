/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        ev_rtn_sewq.c
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


TN_BOOL task_wait_complete (TN_TCB_S *task, TN_BOOL tqueue_remove_enable);

/**
 *
 * @param evf
 *
 * @return TN_BOOL
 */
TN_BOOL scan_event_waitqueue (TN_EVENT_S *evf)
{
    CDLL_QUEUE_S *que;
    CDLL_QUEUE_S *tmp_que;
    TN_TCB_S     *task;
    TN_BOOL       fCond;
    TN_BOOL       wcflag;
    TN_BOOL       fBreak;

    /* Scan event wait queue (checking for empty - before function call) */

    wcflag = TN_FALSE;
    fBreak = TN_FALSE;
    que    = evf->wait_queue.next;

    for (;;)
    {
        task = get_task_by_tsk_queue(que);

        if (task->ewait_mode & TN_EVENT_WCOND_OR)
        {
            fCond = (TN_BOOL)((evf->pattern & task->ewait_pattern) != 0);
        }
        else
        {
            fCond = (TN_BOOL)((evf->pattern & task->ewait_pattern) == task->ewait_pattern);
        }

        if (fCond)
        {
            if (que->next == &(evf->wait_queue))
            {
                queue_remove_entry(que);
                fBreak = TN_TRUE;
            }
            else
            {
                tmp_que = que->next;
                queue_remove_entry(que);
                que = tmp_que;
            }

            task->ewait_pattern = evf->pattern;

            if (task_wait_complete(task, TN_FALSE))
                wcflag = TN_TRUE;

            if (fBreak)
                break;
        }
        else
        {
            if (que->next == &(evf->wait_queue))
                break;
            else
                que = que->next;
        }
    }
    return wcflag;
}

#undef  TN_SOURCE_FILE
