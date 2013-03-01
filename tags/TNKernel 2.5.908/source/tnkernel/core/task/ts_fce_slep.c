/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        tn_fce_slep.c
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

void task_curr_to_wait_action(CDLL_QUEUE_S *wait_que, TN_UWORD wait_reason, TN_TIMEOUT timeout);

/**
 *
 * @param timeout
 *
 * @return TN_RETVAL
 */
TN_RETVAL tnec_task_sleep (TN_TIMEOUT timeout)
{
    TN_UWORD tn_save_status_reg TN_UNUSED;     /* for SR save */
    TN_RETVAL rc;

    if (timeout == 0)
        return  TERR_WRONG_PARAM;

    if (tn_is_non_task_context())
    {
        return TERR_WCONTEXT;
    }

    tn_disable_interrupt();

    if (tn_curr_run_task->wakeup_count > 0)
    {
        tn_curr_run_task->wakeup_count--;
        rc = TERR_NO_ERR;
    }
    else
    {
        task_curr_to_wait_action(TN_NULL, TSK_WAIT_REASON_SLEEP, timeout);
        tn_enable_interrupt();
        tn_switch_context();
        return  TERR_NO_ERR;
    }

    tn_enable_interrupt();
    return rc;
}

#undef  TN_SOURCE_FILE
