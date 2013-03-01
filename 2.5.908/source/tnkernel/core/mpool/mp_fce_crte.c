/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        mp_fce_crte.c
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
 * @param fmp
 * @param start_addr
 * @param block_size
 * @param num_blocks
 *
 * @return TN_RETVAL
 */
TN_RETVAL tnec_fmem_create (TN_FMP_S *fmp, void *start_addr, TN_UWORD block_size, TN_UWORD num_blocks)
{
    void          **p_tmp;
    TN_UCHAR  *p_block;
    TN_UWORD    i, j;

    if (fmp == TN_NULL)
        return TERR_WRONG_PARAM;

    if (start_addr == TN_NULL || num_blocks < 2 || block_size < sizeof(TN_UWORD))
    {
        fmp->fblkcnt    = 0;
        fmp->num_blocks = 0;
        fmp->id_fmp     = TN_ID_UNKNOWN;
        fmp->free_list  = TN_NULL;
        return TERR_WRONG_PARAM;
    }


    if (fmp->id_fmp != 0)
        return TERR_EXS;

    queue_reset(&(fmp->wait_queue));

    /* Prepare addr/block aligment */

    i = (((TN_UWORD)start_addr + (TN_ALIG -1))/TN_ALIG) * TN_ALIG;

    fmp->start_addr = (void*)i;
    fmp->block_size = ((block_size + (TN_ALIG -1))/TN_ALIG) * TN_ALIG;

    i = (TN_UWORD)start_addr      + block_size * num_blocks;
    j = (TN_UWORD)fmp->start_addr + fmp->block_size * num_blocks;

    fmp->num_blocks = num_blocks;

    while (j > i)
    {
        /* Get actual num_blocks */
        j -= fmp->block_size;
        fmp->num_blocks--;
    }

    if (fmp->num_blocks < 2)
    {
        fmp->fblkcnt    = 0;
        fmp->num_blocks = 0;
        fmp->free_list  = TN_NULL;
        return TERR_WRONG_PARAM;
    }

    /* Set blocks ptrs for allocation */

    p_tmp    = (void **)fmp->start_addr;
    p_block  = (TN_UCHAR *)fmp->start_addr + fmp->block_size;

    for (i = 0; i < (fmp->num_blocks - 1); i++)
    {
        *p_tmp   = (void *)p_block;  /* contents of cell = addr of next block */
        p_tmp    = (void **)p_block;
        p_block += fmp->block_size;
    }

    *p_tmp = TN_NULL;          /* Last memory block first cell contents -  NULL */

    fmp->free_list = fmp->start_addr;
    fmp->fblkcnt   = fmp->num_blocks;
    fmp->id_fmp    = TN_ID_FSMEMORYPOOL;

    return TERR_NO_ERR;
}

#undef  TN_SOURCE_FILE
