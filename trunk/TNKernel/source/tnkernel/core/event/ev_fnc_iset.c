/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        ev_fnc_iset.c
 *  Component:   EVENT
 *  Description: Event set in interrupt service without parameter error checking
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


TN_BOOL scan_event_waitqueue(TN_EVENT_S *evf);


TN_RETVAL tnnc_event_iset (TN_EVENT_S *evf, TN_UWORD pattern)
{
    TN_UWORD      tn_save_status_reg TN_UNUSED;

/*  Not check parameter error

    if (evf == TN_NULL || pattern == 0)
        return TERR_WRONG_PARAM;
*/
    if (evf->id_event != TN_ID_EVENT)
        return TERR_NOEXS;

    /* Check calling context - only in first level interrupt and not before system start */
    if (tn_is_non_sys_int_context())
        return TERR_WCONTEXT;


    tn_idisable_interrupt();
    evf->pattern |= pattern;

    if (!(is_queue_empty(&(evf->wait_queue))))
    {
        /* Find any task, that wait event with this pattern and wait attribute */
        if (scan_event_waitqueue(evf))
        {
            if (evf->attr & TN_EVENT_ATTR_CLR)
            {
                /* Clear event flag, if TN_EVENT_ATTR_CLR parameter set */
                evf->pattern = 0;
            }

            tn_context_switch_request = TN_TRUE;    /* Set request for context switching */
            tn_ienable_interrupt();
            return TERR_NO_ERR;
        }
    }

    tn_ienable_interrupt();
    return TERR_NO_ERR;
}

/* ===================================================================================================================*/
#undef  TN_SOURCE_FILE

