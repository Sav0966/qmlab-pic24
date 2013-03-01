/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        cm_extr.h
 *  Component:   COMMON
 *  Description: This file contains function prototypes of all functions accessible to other components from
 *                    cm_xxxx.c
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

#ifndef __CM_EXTR_H
#define __CM_EXTR_H

/* ---------------------------------------------------------------------------------------------------------------------
   Common routines prototypes
   ------------------------------------------------------------------------------------------------------------------ */

void           queue_reset(CDLL_QUEUE_S *que);
TN_BOOL        is_queue_empty(CDLL_QUEUE_S *que);
void           queue_add_head(CDLL_QUEUE_S *que, CDLL_QUEUE_S * entry);
void           queue_add_tail(CDLL_QUEUE_S *que, CDLL_QUEUE_S * entry);
CDLL_QUEUE_S * queue_remove_head(CDLL_QUEUE_S *que);
CDLL_QUEUE_S * queue_remove_tail(CDLL_QUEUE_S *que);
TN_BOOL        queue_contains_entry(CDLL_QUEUE_S *que, CDLL_QUEUE_S * entry);
void           queue_remove_entry(CDLL_QUEUE_S *entry);

/* ===================================================================================================================*/
#endif  /* __CM_EXTR_H */
