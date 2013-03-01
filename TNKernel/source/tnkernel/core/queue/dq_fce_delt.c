/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        dq_fce_delt.c
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
 * @param dque
 *
 * @return TN_RETVAL
 */
TN_RETVAL tnec_queue_delete (TN_DQUE_S *dque)
{
    TN_UWORD      tn_save_status_reg TN_UNUSED;     /* for SR save */
    CDLL_QUEUE_S *que;
    TN_TCB_S     *task;

    if (dque == TN_NULL)                                    /* Check for object */
        return TERR_WRONG_PARAM;

    if (dque->id_dque != TN_ID_DATAQUEUE)                   /* Check if object is dataqueue */
        return TERR_NOEXS;

    if (tn_is_non_task_context())      /* Check for service calling context */
        return TERR_WCONTEXT;

    while (!is_queue_empty(&(dque->wait_send_list)))        /* While sending list is not empty */
    {
        /* In this loop kernel try to move all
           tasks, that sent data early into RUNNABLE state
        */

        tn_disable_interrupt();

        que  = queue_remove_head(&(dque->wait_send_list));
        task = get_task_by_tsk_queue(que);

        if (task_wait_complete(task, TN_FALSE))
        {
            task->task_wait_rc = TERR_DLT;
            tn_enable_interrupt();
            tn_switch_context();
        }
    }

    while (!is_queue_empty(&(dque->wait_receive_list)))
    {
        /* In this loop kernel try to move all
           tasks, that received data early into RUNNABLE state
        */

        tn_disable_interrupt();

        que  = queue_remove_head(&(dque->wait_receive_list));
        task = get_task_by_tsk_queue(que);

        if (task_wait_complete(task, TN_FALSE))
        {
            task->task_wait_rc = TERR_DLT;
            tn_enable_interrupt();
            tn_switch_context();
        }
    }

    tn_disable_interrupt();

    dque->id_dque = TN_ID_UNKNOWN;

    tn_enable_interrupt();
    return TERR_NO_ERR;
}

#undef  TN_SOURCE_FILE
