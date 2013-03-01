/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        ev_extr.h
 *  Component:   EVENT
 *  Description: This file contains function prototypes of all Event services accessible to other components
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


#ifndef __EV_EXTR_H
#define __EV_EXTR_H

/* ---------------------------------------------------------------------------------------------------------------------
   Event services prototypes with parameter error checking
   ------------------------------------------------------------------------------------------------------------------ */
TN_RETVAL tnec_event_create(TN_EVENT_S *evf, TN_UWORD attr, TN_UWORD pattern);
TN_RETVAL tnec_event_delete(TN_EVENT_S *evf);

TN_RETVAL tnec_event_set(TN_EVENT_S *evf, TN_UWORD pattern);
TN_RETVAL tnec_event_iset(TN_EVENT_S *evf, TN_UWORD pattern);

TN_RETVAL tnec_event_clear(TN_EVENT_S *evf, TN_UWORD pattern);
TN_RETVAL tnec_event_iclear(TN_EVENT_S *evf, TN_UWORD pattern);

TN_RETVAL tnec_event_wait(TN_EVENT_S *evf, TN_UWORD wait_pattern, TN_UWORD wait_mode,
                          TN_UWORD *p_flags_pattern, TN_TIMEOUT timeout);
TN_RETVAL tnec_event_iwait(TN_EVENT_S *evf, TN_UWORD wait_pattern, TN_UWORD wait_mode, TN_UWORD *p_flags_pattern);
TN_RETVAL tnec_event_wait_polling(TN_EVENT_S *evf, TN_UWORD wait_pattern,
                                  TN_UWORD wait_mode, TN_UWORD *p_flags_pattern);

/* ---------------------------------------------------------------------------------------------------------------------
   Event services prototypes without parameter error checking
   ------------------------------------------------------------------------------------------------------------------ */
TN_RETVAL tnnc_event_create(TN_EVENT_S *evf, TN_UWORD attr, TN_UWORD pattern);
TN_RETVAL tnnc_event_delete(TN_EVENT_S *evf);

TN_RETVAL tnnc_event_set(TN_EVENT_S *evf, TN_UWORD pattern);
TN_RETVAL tnnc_event_iset(TN_EVENT_S *evf, TN_UWORD pattern);

TN_RETVAL tnnc_event_clear(TN_EVENT_S *evf, TN_UWORD pattern);
TN_RETVAL tnnc_event_iclear(TN_EVENT_S *evf, TN_UWORD pattern);

TN_RETVAL tnnc_event_wait(TN_EVENT_S *evf, TN_UWORD wait_pattern, TN_UWORD wait_mode,
                          TN_UWORD *p_flags_pattern, TN_TIMEOUT timeout);
TN_RETVAL tnnc_event_iwait(TN_EVENT_S *evf, TN_UWORD wait_pattern, TN_UWORD wait_mode, TN_UWORD *p_flags_pattern);
TN_RETVAL tnnc_event_wait_polling(TN_EVENT_S *evf, TN_UWORD wait_pattern,
                                  TN_UWORD wait_mode, TN_UWORD *p_flags_pattern);

/* ===================================================================================================================*/
#endif  /* __EV_EXTR_H */
