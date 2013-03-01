/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        tn_extr.h
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


#ifndef __TS_EXTR_H
#define __TS_EXTR_H

/* Task service prototypes with parameter error checking */

TN_RETVAL tnec_task_create (TN_TCB_S  *task,
                            void     (*task_func)(void *param),
                            TN_UWORD  priority,
                            TN_UWORD  *task_stack_start,
                            TN_UWORD  task_stack_size,
                            void      *param,
                            TN_UWORD  option);
TN_RETVAL tnec_task_delete(TN_TCB_S *task);

void      tnec_task_exit(TN_UWORD attr);

TN_RETVAL tnec_task_sleep(TN_TIMEOUT timeout);
TN_RETVAL tnec_task_resume(TN_TCB_S *task);

TN_RETVAL tnec_task_activate(TN_TCB_S *task);
TN_RETVAL tnec_task_iactivate(TN_TCB_S *task);

TN_RETVAL tnec_task_wakeup(TN_TCB_S *task);
TN_RETVAL tnec_task_iwakeup(TN_TCB_S *task);

TN_RETVAL tnec_task_release_wait(TN_TCB_S *task);
TN_RETVAL tnec_task_irelease_wait(TN_TCB_S *task);

TN_RETVAL tnec_task_suspend(TN_TCB_S *task);
TN_RETVAL tnec_task_terminate(TN_TCB_S *task);

TN_RETVAL tnec_task_change_priority(TN_TCB_S *task, TN_UWORD new_priority);

/* Task service prototypes without parameter error checking */

TN_RETVAL tnnc_task_create (TN_TCB_S  *task,
                            void     (*task_func)(void *param),
                            TN_UWORD  priority,
                            TN_UWORD  *task_stack_start,
                            TN_UWORD  task_stack_size,
                            void      *param,
                            TN_UWORD  option);
TN_RETVAL tnnc_task_delete(TN_TCB_S   *task);

void      tnnc_task_exit(TN_UWORD attr);

TN_RETVAL tnnc_task_sleep(TN_TIMEOUT timeout);
TN_RETVAL tnnc_task_resume(TN_TCB_S *task);

TN_RETVAL tnnc_task_activate(TN_TCB_S *task);
TN_RETVAL tnnc_task_iactivate(TN_TCB_S *task);

TN_RETVAL tnnc_task_wakeup(TN_TCB_S *task);
TN_RETVAL tnnc_task_iwakeup(TN_TCB_S *task);

TN_RETVAL tnnc_task_release_wait(TN_TCB_S *task);
TN_RETVAL tnnc_task_irelease_wait(TN_TCB_S *task);

TN_RETVAL tnnc_task_suspend(TN_TCB_S *task);
TN_RETVAL tnnc_task_terminate(TN_TCB_S *task);

TN_RETVAL tnnc_task_change_priority(TN_TCB_S *task, TN_UWORD new_priority);

#endif  /* __TS_EXTR_H */
