/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        ts_rtn_sdms.c
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


void task_set_dormant_state (TN_TCB_S *task)
{
    queue_reset(&(task->task_queue  ));
    queue_reset(&(task->timer_queue ));
    queue_reset(&(task->create_queue));
    queue_reset(&(task->mutex_queue ));
    queue_reset(&(task->block_queue ));

    task->pwait_queue      = TN_NULL;
    task->blk_task         = TN_NULL;

    task->priority         = task->base_priority;
    task->task_state       = TSK_STATE_DORMANT;
    task->task_wait_reason = TSK_WAIT_REASON_NO;
    task->task_wait_rc     = TERR_NO_ERR;
    task->ewait_pattern    = 0;
    task->ewait_mode       = 0;
    task->data_elem        = TN_NULL;

    task->tick_count       = TN_WAIT_INFINITE;
    task->wakeup_count     = 0;
    task->suspend_count    = 0;

    task->tslice_count     = 0;
}

#undef  TN_SOURCE_FILE
