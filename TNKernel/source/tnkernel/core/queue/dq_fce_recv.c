/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        dq_fce_recv.c
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
     /* core depended functions */

TN_BOOL   task_wait_complete(TN_TCB_S *task, TN_BOOL tqueue_remove_enable);
TN_RETVAL dque_fifo_read(TN_DQUE_S *dque, void **data_ptr);
TN_RETVAL dque_fifo_write(TN_DQUE_S *dque, void *data_ptr);
void      task_curr_to_wait_action(CDLL_QUEUE_S *wait_que, TN_UWORD wait_reason, TN_TIMEOUT timeout);

/**
 *
 * @param dque
 * @param data_ptr
 * @param timeout
 *
 * @return TN_RETVAL
 */
TN_RETVAL tnec_queue_receive (TN_DQUE_S * dque, void **data_ptr, TN_TIMEOUT timeout)
{
    TN_UWORD      tn_save_status_reg TN_UNUSED;     /* for SR save */
    TN_RETVAL           rc;
    CDLL_QUEUE_S *que;
    TN_TCB_S     *task;

    if (dque == TN_NULL || timeout == 0 || data_ptr == TN_NULL)
        return  TERR_WRONG_PARAM;

    if (dque->id_dque != TN_ID_DATAQUEUE)
        return TERR_NOEXS;

    if (tn_is_non_task_context())
    {
        return TERR_WCONTEXT;
    }

    tn_disable_interrupt();

    rc = dque_fifo_read(dque,data_ptr);

    if (rc == TERR_NO_ERR)
    {
        if (!is_queue_empty(&(dque->wait_send_list)))
        {
            que  = queue_remove_head(&(dque->wait_send_list));
            task = get_task_by_tsk_queue(que);

            dque_fifo_write(dque,task->data_elem);

            if (task_wait_complete(task, TN_FALSE))
            {
                tn_enable_interrupt();
                tn_switch_context();
                return TERR_NO_ERR;
            }
        }
    }
    else
    {
        if (!is_queue_empty(&(dque->wait_send_list)))
        {
            que  = queue_remove_head(&(dque->wait_send_list));
            task = get_task_by_tsk_queue(que);

            *data_ptr = task->data_elem;

            if (task_wait_complete(task, TN_FALSE))
            {
                tn_enable_interrupt();
                tn_switch_context();
                return TERR_NO_ERR;
            }

            rc = TERR_NO_ERR;
        }
        else
        {
            task_curr_to_wait_action(&(dque->wait_receive_list), TSK_WAIT_REASON_DQUE_WRECEIVE, timeout);
            tn_enable_interrupt();
            tn_switch_context();

            /* When return to this point,in data_elem have to be  valid value */

            *data_ptr = tn_curr_run_task->data_elem;
            return tn_curr_run_task->task_wait_rc;
        }
    }

    tn_enable_interrupt();
    return rc;
}

#undef  TN_SOURCE_FILE
