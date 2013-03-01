/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        mx_fce_crte.c
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


TN_RETVAL tnec_mutex_create (TN_MUTEX_S *mutex, TN_UWORD attribute, TN_UWORD ceil_priority)
{
    if (mutex == TN_NULL)
        return TERR_WRONG_PARAM;

    if (attribute != TN_MUTEX_ATTR_CEILING && attribute != TN_MUTEX_ATTR_INHERIT)
        return TERR_WRONG_PARAM;

    if (attribute == TN_MUTEX_ATTR_CEILING &&
        (ceil_priority < 1 || ceil_priority > TN_NUM_PRIORITY-2)
       )
    {
        return TERR_WRONG_PARAM;
    }

    if (mutex->id_mutex != 0)
        return TERR_EXS;

    /*
    if (tn_is_user_crit_sect())
    {
        return TERR_WCONTEXT;
    }
    */

    queue_reset(&(mutex->wait_queue));
    queue_reset(&(mutex->mutex_queue));
    queue_reset(&(mutex->lock_mutex_queue));

    mutex->attr          = attribute;
    mutex->holder        = TN_NULL;
    mutex->ceil_priority = ceil_priority;
    mutex->cnt           = 0;
    mutex->id_mutex      = TN_ID_MUTEX;

    return TERR_NO_ERR;
}

#undef  TN_SOURCE_FILE

