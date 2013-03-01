/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        ev_fnc_delt.c
 *  Component:   EVENT
 *  Description: Event delete service without parameter error checking
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


TN_BOOL task_wait_complete (TN_TCB_S *task, TN_BOOL tqueue_remove_enable);


TN_RETVAL tnnc_event_delete (TN_EVENT_S *evf)
{
    TN_UWORD      tn_save_status_reg TN_UNUSED;     /* for SR save */
    CDLL_QUEUE_S *que;
    TN_TCB_S     *task;

/*  Not check parameter error

    if (evf == TN_NULL)
    {
        return TERR_WRONG_PARAM;
    }


*/
    if (evf->id_event != TN_ID_EVENT)
        return TERR_NOEXS;

    /* Check calling context - not interrupt and not user critical section */
    if (tn_is_non_task_context())
        return TERR_WCONTEXT;


    /* Find all ready tasks in event waiting queue and switch context */
    while (!is_queue_empty(&(evf->wait_queue)))
    {
/*      Dummy checking in V.2.3 - let's disable interrupts always

        if (tn_chk_irq_disabled() == 0)
            tn_disable_interrupt();
*/
        tn_disable_interrupt();

        que  = queue_remove_head(&(evf->wait_queue));
        task = get_task_by_tsk_queue(que);

        if (task_wait_complete(task, TN_FALSE))
        {
            task->task_wait_rc = TERR_DLT;
            tn_enable_interrupt();
            tn_switch_context();
        }
    }

/*  Dummy checking in V.2.3 - let's disable interrupts always

    if (tn_chk_irq_disabled() == 0)
        tn_disable_interrupt();
*/
    /* Delete event control block after wake-up of ALL tasks that waited event */
    tn_disable_interrupt();
    evf->id_event = TN_ID_UNKNOWN;
    tn_enable_interrupt();

    return TERR_NO_ERR;
}

/* ===================================================================================================================*/
#undef  TN_SOURCE_FILE

