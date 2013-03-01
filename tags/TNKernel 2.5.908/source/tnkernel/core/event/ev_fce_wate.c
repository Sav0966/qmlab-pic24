/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        ev_fce_wate.c
 *  Component:   EVENT
 *  Description: Wait Event with parameter error checking
 *
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


TN_RETVAL tnec_event_wait (TN_EVENT_S *evf, TN_UWORD wait_pattern, TN_UWORD wait_mode,
                           TN_UWORD *p_flags_pattern, TN_TIMEOUT timeout
                          )
{
    TN_UWORD  tn_save_status_reg TN_UNUSED;     /* for SR save */
    TN_RETVAL rc;
    TN_BOOL   fCond;

    /* Check parameters */
    if (evf             == TN_NULL ||
        wait_pattern    == 0       ||
        p_flags_pattern == TN_NULL ||
        timeout         == 0
       )
    {
        return TERR_WRONG_PARAM;
    }

    /* Check event control block */
    if (evf->id_event != TN_ID_EVENT)
        return TERR_NOEXS;

    /* Check calling context - not interrupt and not user critical section */
    if (tn_is_non_task_context())
        return TERR_WCONTEXT;


    tn_disable_interrupt();

    /* If event attr is TN_EVENT_ATTR_SINGLE and another task already in event wait queue - return TERR_ILUSE
       without checking release condition */
    if ((evf->attr & TN_EVENT_ATTR_SINGLE) && !is_queue_empty(&(evf->wait_queue)))
    {
        rc = TERR_ILUSE;
    }
    else
    {
        /* Get pattern coincidence flag */
        if (wait_mode & TN_EVENT_WCOND_OR)
            fCond = (TN_BOOL)((evf->pattern & wait_pattern) != 0);
        else
            fCond = (TN_BOOL)((evf->pattern & wait_pattern) == wait_pattern);

        if (fCond)
        {
            /* Event pattern is equal asked */
            *p_flags_pattern = evf->pattern;

            if (evf->attr & TN_EVENT_ATTR_CLR)
                evf->pattern = 0;       /* Clear event flag, if TN_EVENT_ATTR_CLR parameter set */

            rc = TERR_NO_ERR;
        }
        else
        {
            /* Change task state to WAITING and switch context */
            tn_curr_run_task->ewait_mode    = wait_mode;
            tn_curr_run_task->ewait_pattern = wait_pattern;

            task_curr_to_wait_action(&(evf->wait_queue),TSK_WAIT_REASON_EVENT,timeout);
            tn_enable_interrupt();
            tn_switch_context();

            if (tn_curr_run_task->task_wait_rc == TERR_NO_ERR)
            {
                /* Exit form waiting by event receive */
                *p_flags_pattern = tn_curr_run_task->ewait_pattern;
            }

            return tn_curr_run_task->task_wait_rc;
        }
    }

    tn_enable_interrupt();
    return rc;
}

/* ===================================================================================================================*/
#undef  TN_SOURCE_FILE
