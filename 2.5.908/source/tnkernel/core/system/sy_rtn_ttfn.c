/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        sy_rtn_ttfn.c
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

#include "sy_extr.h"

#include "..\\common\\cm_extr.h"            /* common routines         */
#include "..\\task\\ts_extr.h"              /* task functions          */
//     /* core depended functions */

TN_BOOL task_wait_complete(TN_TCB_S *task, TN_BOOL tqueue_remove_enable);
void    task_curr_to_wait_action(CDLL_QUEUE_S *wait_que, TN_UWORD wait_reason, TN_TIMEOUT timeout);

/**
 *
 * @param par
 *
 * @return TN_TASK void
 */
void TN_TASK tn_timer_task_func (void *par)
{
    volatile TN_UWORD     TN_UNUSED tn_save_status_reg = 0;     /* for SR save */
    volatile TN_TCB_S     *task;
    volatile CDLL_QUEUE_S *curr_que;

    appl_init_callback();
    cpu_interrupt_enbl_callback();

    for (;;)
    {
        tn_disable_interrupt(); /* V. 2.5 fix */

        if (!is_queue_empty((CDLL_QUEUE_S *)&tn_wait_timeout_list))
        {
            curr_que = tn_wait_timeout_list.next;
            for (;;)
            {
                task = get_task_by_timer_queue((CDLL_QUEUE_S *)curr_que);

                if (task->tick_count > 0 && task->tick_count != TN_WAIT_INFINITE)
                {
                    task->tick_count--;

                    if (task->tick_count == 0)
                    {
                        task_wait_complete((TN_TCB_S *)task, TN_TRUE);
                        task->task_wait_rc = TERR_TIMEOUT;
                    }
                }
                if (curr_que->next == &tn_wait_timeout_list)
                    break;
                else
                    curr_que = curr_que->next;
            }
        }

        task_curr_to_wait_action(TN_NULL, TSK_WAIT_REASON_SLEEP, TN_WAIT_INFINITE);
        tn_enable_interrupt();
        tn_switch_context();
    }
}

#undef  TN_SOURCE_FILE
