/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        tn_fnc_irlw.c
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

TN_BOOL task_wait_complete(TN_TCB_S *task, TN_BOOL tqueue_remove_enable);

/**
 *
 * @param task
 *
 * @return TN_RETVAL
 */
TN_RETVAL tnnc_task_irelease_wait (TN_TCB_S *task)
{
    TN_UWORD tn_save_status_reg TN_UNUSED;
    TN_RETVAL rc;
/*
    Not check parameter error

    if (task == TN_NULL)
        return TERR_WRONG_PARAM;
*/
    if (task->id_task != TN_ID_TASK)
        return TERR_NOEXS;

    if (tn_is_non_sys_int_context())
    {
        return TERR_WCONTEXT;
    }

    tn_idisable_interrupt();

    if ((task->task_state & TSK_STATE_WAIT) == 0)
    {
        rc = TERR_WCONTEXT;
    }
    else
    {
        if (task_wait_complete(task, TN_TRUE))
        {
            tn_context_switch_request = TN_TRUE;
        }
        rc = TERR_NO_ERR;
    }

    tn_ienable_interrupt();
    return rc;
}

#undef  TN_SOURCE_FILE

