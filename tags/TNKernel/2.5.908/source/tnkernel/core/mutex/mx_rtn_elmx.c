/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        mx_rtn_elmx.c
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

/**
 *
 * @param curr_task
 * @param blk_task
 *
 * @return TN_BOOL
 */
TN_BOOL enable_lock_mutex(TN_TCB_S *curr_task, TN_TCB_S ** blk_task)
{
    TN_TCB_S     *res_t;
    TN_TCB_S     *tmp_task;
    TN_MUTEX_S   *tmp_mutex;
    CDLL_QUEUE_S *curr_que;
    TN_BOOL       result;
    TN_UWORD      priority;

    if (is_queue_empty(&tn_locked_mutexes_list))
    {
        if (blk_task != TN_NULL)
        {
            *blk_task = TN_NULL;
        }
        return TN_TRUE;
    }

    result   = TN_TRUE;
    curr_que = tn_locked_mutexes_list.next;
    priority = 0;
    res_t    = TN_NULL;

    for (;;)
    {
        tmp_mutex = get_mutex_by_lock_mutex_queue(curr_que);

        if (tmp_mutex->attr == TN_MUTEX_ATTR_CEILING)
        {
            tmp_task = tmp_mutex->holder;

            if (tmp_task != curr_task && tmp_mutex->ceil_priority < curr_task->priority)
            {
                result = TN_FALSE;
                if (tmp_mutex->ceil_priority > priority)
                {
                    priority = tmp_mutex->ceil_priority;
                    res_t    = tmp_mutex->holder;
                }
            }
        }

        if (curr_que->next == &tn_locked_mutexes_list)
            break;
        else
            curr_que = curr_que->next;
    }

    if (blk_task != TN_NULL)
    {
        *blk_task = res_t;
    }
    return result;
}

#undef  TN_SOURCE_FILE
