/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        dq_fnc_crte.c
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

/**
 *
 * @param dque
 * @param data_fifo
 * @param num_entries
 *
 * @return TN_RETVAL
 */
TN_RETVAL tnnc_queue_create (TN_DQUE_S *dque, void **data_fifo, TN_UWORD num_entries)
{
/*
    Not check parameter error

    if (dque == TN_NULL)
        return TERR_WRONG_PARAM;
*/

    if (dque->id_dque != 0)
        return TERR_EXS;

    queue_reset(&(dque->wait_send_list));
    queue_reset(&(dque->wait_receive_list));

    dque->data_fifo   = data_fifo;
    dque->num_entries = num_entries;

    if (dque->data_fifo == TN_NULL)
        dque->num_entries = 0;

    dque->tail_cnt   = 0;
    dque->header_cnt = 0;

    dque->id_dque = TN_ID_DATAQUEUE;

    return  TERR_NO_ERR;
}

#undef  TN_SOURCE_FILE
