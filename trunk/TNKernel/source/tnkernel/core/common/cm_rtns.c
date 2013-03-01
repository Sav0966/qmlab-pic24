/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        cm_rtns.c
 *  Component:   COMMON
 *  Description: This file contains linked list manipulation facilities used throughout the TNKernel. These
 *                    facilities operate on doubly-linked circular lists.
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

/**
 * Reset queue
 *
 * @param que
 */
void queue_reset (CDLL_QUEUE_S *que)
{
    que->prev = que->next = que;
}

/**
 *
 * @param que
 *
 * @return TN_BOOL
 */
TN_BOOL is_queue_empty (CDLL_QUEUE_S *que)
{
    if (que->next == que && que->prev == que)
        return TN_TRUE;
    return TN_FALSE;
}

/**
 *
 * @param que
 * @param entry
 */
void queue_add_head (CDLL_QUEUE_S *que, CDLL_QUEUE_S *entry)
{
    entry->next       = que->next;
    entry->prev       = que;
    entry->next->prev = entry;
    que->next         = entry;
}

/**
 *
 * @param que
 * @param entry
 */
void queue_add_tail (CDLL_QUEUE_S *que, CDLL_QUEUE_S *entry)
{
    entry->next       = que;
    entry->prev       = que->prev;
    entry->prev->next = entry;
    que->prev         = entry;
}

/**
 *
 * @param que
 *
 * @return CDLL_QUEUE_S*
 */
CDLL_QUEUE_S * queue_remove_head (CDLL_QUEUE_S *que)
{
    CDLL_QUEUE_S *entry;

    if (que == TN_NULL || que->next == que)
        return ((CDLL_QUEUE_S *)0);

    entry             = que->next;
    entry->next->prev = que;
    que->next         = entry->next;
    return entry;
}

/**
 *
 * @param que
 *
 * @return CDLL_QUEUE_S*
 */
CDLL_QUEUE_S * queue_remove_tail (CDLL_QUEUE_S *que)
{
    CDLL_QUEUE_S *entry;

    if (que->prev == que)
        return ((CDLL_QUEUE_S *)0);

    entry             = que->prev;
    entry->prev->next = que;
    que->prev         = entry->prev;
    return entry;
}

/**
 *
 * @param entry
 */
void queue_remove_entry (CDLL_QUEUE_S * entry)
{
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
}

/**
 *
 * @param que
 * @param entry
 *
 * @return TN_BOOL
 */
TN_BOOL queue_contains_entry (CDLL_QUEUE_S *que, CDLL_QUEUE_S *entry)
{
    TN_BOOL       result = TN_FALSE;
    CDLL_QUEUE_S *curr_que;

    curr_que = que->next;

    for (;;)
    {
        if (curr_que == entry)
        {
            result = TN_TRUE;
            break;
        }
        if (curr_que->next == que)
            break;
        else
            curr_que = curr_que->next;
    }
    return result;
}

/* ===================================================================================================================*/
#undef TN_SOURCE_FILE
