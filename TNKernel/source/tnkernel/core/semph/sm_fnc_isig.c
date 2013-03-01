/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        sm_fnc_isig.c
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
 * @param sem
 *
 * @return TN_RETVAL
 */
TN_RETVAL tnnc_sem_isignal (TN_SEM_S *sem)
{
    TN_UWORD          tn_save_status_reg TN_UNUSED;
    TN_RETVAL          rc;
    CDLL_QUEUE_S *que;
    TN_TCB_S     *task;

/*
    Not check parameter error

    if (sem == TN_NULL || sem->max_count == 0)
        return  TERR_WRONG_PARAM;
*/

    if (sem->id_sem != TN_ID_SEMAPHORE)
        return TERR_NOEXS;

    if (tn_is_non_sys_int_context())
    {
        return TERR_WCONTEXT;
    }

    tn_idisable_interrupt();

    if (!(is_queue_empty(&(sem->wait_queue))))
    {
        que  = queue_remove_head(&(sem->wait_queue));
        task = get_task_by_tsk_queue(que);

        if (task_wait_complete(task, TN_FALSE))
        {
            tn_context_switch_request = TN_TRUE;
            tn_ienable_interrupt();
            return TERR_NO_ERR;
        }
        rc = TERR_NO_ERR;
    }
    else
    {
        if (sem->count < sem->max_count)
        {
            sem->count++;
            rc = TERR_NO_ERR;
        }
        else
        {
            rc = TERR_OVERFLOW;
        }
    }

    tn_ienable_interrupt();
    return rc;
}

#undef  TN_SOURCE_FILE

