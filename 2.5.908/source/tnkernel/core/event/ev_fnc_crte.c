/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        ev_fnc_crte.c
 *  Component:   EVENT
 *  Description: Event create service without parameter error checking
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


TN_RETVAL tnnc_event_create (TN_EVENT_S *evf, TN_UWORD attr, TN_UWORD pattern)
{
/*  Not check parameter error

    if (evf == TN_NULL                                                                 ||
        (((attr & TN_EVENT_ATTR_SINGLE) == 0)  && ((attr & TN_EVENT_ATTR_MULTI) == 0))
       )
    {
        return TERR_WRONG_PARAM;
    }
*/

    if (evf->id_event != 0)
        return TERR_EXS;

    queue_reset(&(evf->wait_queue));    /* Reset event waiting queue*/

    evf->pattern = pattern;
    evf->attr    = attr;
/*
    Const TN_INVALID_VAL used only here in 2.3 - move this check to the begin of funct in tnec_event_create()

    if ((attr & TN_EVENT_ATTR_CLR) && ((attr & TN_EVENT_ATTR_SINGLE)== 0))
    {
        evf->attr = TN_INVALID_VAL;
        return TERR_WRONG_PARAM;
    }
*/
    evf->id_event = TN_ID_EVENT;        /* Now we have new event control block */

    return TERR_NO_ERR;
}

/* ===================================================================================================================*/
#undef  TN_SOURCE_FILE
