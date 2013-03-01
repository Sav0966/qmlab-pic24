/*
 *  TNKernel RTOS
 *
 *  File:        tnkernel.h
 *  Compiler:
 *
 *  Ver. 2.5
 *
 *  Copyright © 2004,2008 Yuri Tiomkin
 *  Copyright © 2008 Alex Borisov (PIC24/dsPIC port)
 *
 *  All rights reserved.
 *
 *  Permission to use, copy, modify, and distribute this software in source and binary forms and
 *  its documentation for any purpose and without fee is hereby granted, provided that the above
 *  copyright notice appear in all copies and that both that copyright notice and this permission
 *  notice appear in supporting documentation.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 *  AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL YURI TIOMKIN OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 *  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef __TNKERNEL_H
#define __TNKERNEL_H

#include <limits.h>         /* Standart ANSI C header for proper max constants definition */

#include "tnkernel_rev.h"   /* Revision file */
#ifndef TN_SOURCE_FILE
#include "tnkernel_conf.h"  /* Application depended */
#endif
#include "portable\\port_extr.h"    /* core depended functions */

/***************************************************************************************************
   TNKernel internal data types definition and standart constants
 **************************************************************************************************/

typedef enum _TN_BOOL
{
    TN_FALSE,
    TN_TRUE
} TN_BOOL;

#define TN_NULL                 (void*)(0)
#define  MAKE_ALIG(a)           ((((a) + (TN_ALIG - 1)) / TN_ALIG) * TN_ALIG)

/***************************************************************************************************
   System API constants
 **************************************************************************************************/

#define TN_FILL_STACK_VAL       UINT_MAX

/***************************************************************************************************
   API definitions
 **************************************************************************************************/

                            /* SERVICE COMPLETION STATUS CONSTANTS */

typedef enum _TN_RETVAL
{
    TERR_NO_ERR      =   (0),   /* Normal completion */
    TERR_OVERFLOW    =  (-1),   /* Resource has max value (sem counter, task's counters, e.t.c.)*/
    TERR_WCONTEXT    =  (-2),   /* Function calling context error */
    TERR_WSTATE      =  (-3),   /* Wrong state of task */
    TERR_TIMEOUT     =  (-4),   /* Timeout has been expired */
    TERR_WRONG_PARAM =  (-5),   /* Function parameter error */
    TERR_UNDERFLOW   =  (-6),   /* -- not API -- */
    TERR_OUT_OF_MEM  =  (-7),   /* -- not API -- */
    TERR_ILUSE       =  (-8),   /* Illegal usage of mutex, or for event wait polling function */
    TERR_NOEXS       =  (-9),   /* Illegal control block as parameter of service */
    TERR_DLT         = (-10),   /* Task return from waiting with this code, if wating component
                                   has been deleted */
    TERR_EXS         = (-11),   /* The object already exist */
} TN_RETVAL;

                                        /* OBJECTS ID */

typedef enum _TN_OBJ_ID
{
    TN_ID_UNKNOWN      = 0,
    TN_ID_TASK         = ((TN_WORD)0x47ABCF69),
    TN_ID_SEMAPHORE    = ((TN_WORD)0x6FA173EB),
    TN_ID_EVENT        = ((TN_WORD)0x5E224F25),
    TN_ID_MUTEX        = ((TN_WORD)0x17129E45),
    TN_ID_DATAQUEUE    = ((TN_WORD)0x8C8A6C89),
    TN_ID_FSMEMORYPOOL = ((TN_WORD)0x26B7CE8B)
} TN_OBJ_ID;

                                  /* TNKERNEL SYSTME STATE */

typedef enum _TN_STATE
{
    TN_ST_STATE_NOT_RUN,
    TN_ST_STATE_RUNNING
} TN_STATE;

                                      /* TNKERNEL CONTEXT */

typedef enum _TN_CONTEXT
{
    TN_CONTEXT_TASK     = 0,
    TN_CONTEXT_SYS_INT  = (1 << 0),
    TN_CONTEXT_CRITICAL = (1 << 1)
} TN_CONTEXT;
                                  /* TASK SERVICE DEFINITIONS */

    /* Parameter for task create function: */

#define TN_TASK_DORMANT_ON_CREATION     (0)             /* dormant after creation   */
#define TN_TASK_START_ON_CREATION       (1)             /* run task after creation  */
#if defined(TN_SOURCE_FILE)
#define  TN_TASK_TIMER                  (1 << 7)        /* timer system task        */
#define  TN_TASK_IDLE                   (1 << 6)        /* idle system task         */
#endif

    /* Parameter for task exit function: */

#define TN_EXIT_TASK                    (0)             /* only exit                */
#define TN_EXIT_AND_DELETE_TASK         (1)             /* exit and delete task     */

#define TN_WAIT_INFINITE                (TN_TIMEOUT_MAX) /* used as parameter for infinitive timeout */

    /* Round-Robin schedulling defines: */

#define NO_TIME_SLICE                   (0)             /* disable slicing  */
#define MAX_TIME_SLICE                  (TN_TIMEOUT_MAX - 1)  /* max time slicing */


                                /* EVENTS SERVICE DEFINITIONS */

    /* Event attirbute */

#define TN_EVENT_ATTR_SINGLE            (1 << 0)        /* single task only is allowed to be in the
                                                           waiting state for the eventflag*/

#define TN_EVENT_ATTR_MULTI             (1 << 1)        /* multiple tasks are allowed to be in the
                                                           waiting state for the eventflag*/

#define TN_EVENT_ATTR_CLR               (1 << 2)        /* eventflag's entire bit pattern will be
                                                           cleared when a task is released from the
                                                           waiting state for the eventflag */

    /* Event wait condition */

#define TN_EVENT_WCOND_OR               (1 << 3)        /* any bit getting set is enough for
                                                           release condition */

#define TN_EVENT_WCOND_AND              (1 << 4)        /* release condition requires all set bits
                                                           matching */

                                /* MUTEX SERVICE DEFINITIONS */

    /* Mutex used protocol: */

#define TN_MUTEX_ATTR_CEILING           1               /* priority ceiling protocol        */
#define TN_MUTEX_ATTR_INHERIT           2               /* priority inheritance protocol    */



/***************************************************************************************************
    RTOS objects structures
 **************************************************************************************************/

    /* Task control block */

typedef enum _TN_TASK_STATE         /* Task state */
{
    TSK_STATE_RUNNABLE              = (1 << 0),
    TSK_STATE_WAIT                  = (1 << 2),
    TSK_STATE_SUSPEND               = (1 << 3),
    TSK_STATE_WAITSUSP              = (TSK_STATE_SUSPEND | TSK_STATE_WAIT),
    TSK_STATE_DORMANT               = (1 << 4),
} TN_TASK_STATE;


typedef enum _TN_TASK_WAIT_REASON   /* Task wait reasons */
{
    TSK_WAIT_REASON_NO              = (0),
    TSK_WAIT_REASON_SLEEP           = (1 <<  0),      /* Sleep state              */
    TSK_WAIT_REASON_SEM             = (1 <<  1),      /* Semaphore waiting        */
    TSK_WAIT_REASON_EVENT           = (1 <<  2),      /* Event waiting            */
    TSK_WAIT_REASON_DQUE_WSEND      = (1 <<  3),      /* Wait queue sending       */
    TSK_WAIT_REASON_DQUE_WRECEIVE   = (1 <<  4),      /* Wait queue receiving     */
    TSK_WAIT_REASON_MUTEX_C         = (1 <<  5),      /*                          */
    TSK_WAIT_REASON_MUTEX_C_BLK     = (1 <<  6),      /*                          */
    TSK_WAIT_REASON_MUTEX_I         = (1 <<  7),      /*                          */
    TSK_WAIT_REASON_MUTEX_H         = (1 <<  8),      /*                          */
    TSK_WAIT_REASON_RENDEZVOUS      = (1 <<  9),      /*                          */
    TSK_WAIT_REASON_WFIXMEM         = (1 << 13),      /*                          */
} TN_TASK_WAIT_REASON;



    /* Common circular queue */

typedef struct _CDLL_QUEUE_S
{
    struct _CDLL_QUEUE_S * prev;
    struct _CDLL_QUEUE_S * next;
} CDLL_QUEUE_S;

typedef struct _TN_TASK_REF
{
    TN_TASK_STATE       state;
    TN_TASK_WAIT_REASON wait_reason;
    TN_UWORD            base_priority;
    TN_UWORD            current_priority;
    TN_TIMEOUT          timeout;
} TN_TASK_REF;

typedef struct _TN_TCB_S
{
    TN_UWORD          * task_stk;
    CDLL_QUEUE_S        task_queue;
    CDLL_QUEUE_S        timer_queue;
    CDLL_QUEUE_S        block_queue;
    CDLL_QUEUE_S        create_queue;
    CDLL_QUEUE_S        mutex_queue;
    CDLL_QUEUE_S      * pwait_queue;
    struct _TN_TCB_S  * blk_task;

    TN_UWORD          * stk_start;
    TN_UWORD            stk_size;

    void              * task_func_addr;
    void              * task_func_param;

    TN_UWORD            base_priority;
    TN_UWORD            priority;
    TN_OBJ_ID           id_task;

    TN_TASK_STATE       task_state;
    TN_TASK_WAIT_REASON task_wait_reason;
    TN_RETVAL           task_wait_rc;
    TN_TIMEOUT          tick_count;            /* Alex B. - else can't be more then 0x7FFF (16-bit) or 0x7FFFFFFF */
    TN_TIMEOUT          tslice_count;

    TN_UWORD            ewait_pattern;
    TN_UWORD            ewait_mode;

    void              * data_elem;

    TN_UWORD            activate_count;
    TN_UWORD            wakeup_count;
    TN_UWORD            suspend_count;
} TN_TCB_S;

    /* Semaphore control block */

typedef struct TN_SEM_S_STRUCT
{
    CDLL_QUEUE_S        wait_queue;
    TN_UWORD            count;
    TN_UWORD            max_count;
    TN_OBJ_ID           id_sem;
} TN_SEM_S;

    /* Event control block */

typedef struct _TN_EVENT_S
{
    CDLL_QUEUE_S        wait_queue;
    TN_UWORD            attr;
    TN_UWORD            pattern;
    TN_OBJ_ID           id_event;
} TN_EVENT_S;

    /* Data queue control block */

typedef struct _TN_DQUE_S
{
    CDLL_QUEUE_S        wait_send_list;
    CDLL_QUEUE_S        wait_receive_list;

    void             ** data_fifo;
    TN_UWORD            num_entries;
    TN_UWORD            tail_cnt;
    TN_UWORD            header_cnt;
    TN_OBJ_ID           id_dque;
} TN_DQUE_S;

    /* Memory pool control block */

typedef struct _TN_FMP_S
{
    CDLL_QUEUE_S        wait_queue;

    TN_UWORD            block_size;
    TN_UWORD            num_blocks;
    void              * start_addr;
    void              * free_list;
    TN_UWORD            fblkcnt;
    TN_OBJ_ID           id_fmp;
} TN_FMP_S;

    /* Mutex control block */

typedef struct _TN_MUTEX_S
{
    CDLL_QUEUE_S        wait_queue;
    CDLL_QUEUE_S        mutex_queue;
    CDLL_QUEUE_S        lock_mutex_queue;
    TN_UWORD            attr;

    TN_TCB_S          * holder;
    TN_UWORD            ceil_priority;
    TN_WORD             cnt;
    TN_OBJ_ID           id_mutex;
} TN_MUTEX_S;

/***************************************************************************************************
    Close object structures if TN_DEBUG undefined
 **************************************************************************************************/

typedef TN_TCB_S    TN_TCB;
typedef TN_SEM_S    TN_SEM;
typedef TN_EVENT_S  TN_EVENT;
typedef TN_DQUE_S   TN_DQUE;
typedef TN_FMP_S    TN_FMP;
typedef TN_MUTEX_S  TN_MUTEX;


/* Macro for get queue by type */

#ifdef TN_SOURCE_FILE  /* ONLY in TNKernel source */

    #define  get_task_by_tsk_queue(a)           (TN_UWORD*)(a) ? (TN_TCB_S*)((TN_UWORD*)(a) - 1) : (TN_TCB_S*)(TN_NULL)
    #define  get_task_by_timer_queue(a)         (TN_UWORD*)(a) ? (TN_TCB_S*)((TN_UWORD*)(a) - 3) : (TN_TCB_S*)(TN_NULL)
    #define  get_task_by_block_queue(a)         (TN_UWORD*)(a) ? (TN_TCB_S*)((TN_UWORD*)(a) - 5) : (TN_TCB_S*)(TN_NULL)
    
    #define  get_mutex_by_task_wait_queue(a)    (TN_UWORD*)(a) ? (TN_MUTEX_S*)((TN_UWORD*)(a) - 0) : (TN_MUTEX_S*)(TN_NULL)
    #define  get_mutex_by_mutex_queue(a)        (TN_UWORD*)(a) ? (TN_MUTEX_S*)((TN_UWORD*)(a) - 2) : (TN_MUTEX_S*)(TN_NULL)
    #define  get_mutex_by_lock_mutex_queue(a)   (TN_UWORD*)(a) ? (TN_MUTEX_S*)((TN_UWORD*)(a) - 4) : (TN_MUTEX_S*)(TN_NULL)

#endif


/***************************************************************************************************
   Define the API services as macro for different parameter checking option.
 **************************************************************************************************/

#ifndef TN_SOURCE_FILE  /* not include in TNKernel source */

    #ifdef  TN_NO_ERROR_CHECKING

    /* Define API functions with error checking */

        #define tn_sys_enter_critical       tn_enter_critical
        #define tn_sys_exit_critical        tn_exit_critical
        #define tn_sys_context_get          tncm_sys_context_get
        #define tn_start_system             tnnc_start_system
        #define tn_tick_int_processing      tnnc_tick_int_processing
        #define tn_sys_tslice_ticks         tnnc_sys_tslice_ticks
        #define tn_sys_time_get             tncm_sys_time_get
        #define tn_sys_time_set             tncm_sys_time_set
        #define tn_task_reference           tnnc_task_reference
        #define tn_task_ireference          tnnc_task_ireference
        #define tn_task_create              tnnc_task_create
        #define tn_task_terminate           tnnc_task_terminate
        #define tn_task_exit                tnnc_task_exit
        #define tn_task_delete              tnnc_task_delete
        #define tn_task_activate            tnnc_task_activate
        #define tn_task_iactivate           tnnc_task_iactivate
        #define tn_task_isuspend            tnnc_task_isuspend
        #define tn_task_change_priority     tnnc_task_change_priority
        #define tn_task_suspend             tnnc_task_suspend
        #define tn_task_resume              tnnc_task_resume
        #define tn_task_iresume             tnnc_task_iresume
        #define tn_task_sleep               tnnc_task_sleep
        #define tn_task_wakeup              tnnc_task_wakeup
        #define tn_task_iwakeup             tnnc_task_iwakeup
        #define tn_task_release_wait        tnnc_task_release_wait
        #define tn_task_irelease_wait       tnnc_task_irelease_wait
        #define tn_sem_create               tnnc_sem_create
        #define tn_sem_delete               tnnc_sem_delete
        #define tn_sem_signal               tnnc_sem_signal
        #define tn_sem_isignal              tnnc_sem_isignal
        #define tn_sem_acquire              tnnc_sem_acquire
        #define tn_sem_polling              tnnc_sem_polling
        #define tn_sem_ipolling             tnnc_sem_ipolling
        #define tn_queue_create             tnnc_queue_create
        #define tn_queue_delete             tnnc_queue_delete
        #define tn_queue_send               tnnc_queue_send
        #define tn_queue_send_polling       tnnc_queue_send_polling
        #define tn_queue_isend_polling      tnnc_queue_isend_polling
        #define tn_queue_receive            tnnc_queue_receive
        #define tn_queue_receive_polling    tnnc_queue_receive_polling
        #define tn_queue_ireceive           tnnc_queue_ireceive
        #define tn_event_create             tnnc_event_create
        #define tn_event_delete             tnnc_event_delete
        #define tn_event_wait               tnnc_event_wait
        #define tn_event_wait_polling       tnnc_event_wait_polling
        #define tn_event_iwait              tnnc_event_iwait
        #define tn_event_set                tnnc_event_set
        #define tn_event_iset               tnnc_event_iset
        #define tn_event_clear              tnnc_event_clear
        #define tn_event_iclear             tnnc_event_iclear
        #define tn_fmem_create              tnnc_fmem_create
        #define tn_fmem_delete              tnnc_fmem_delete
        #define tn_fmem_get                 tnnc_fmem_get
        #define tn_fmem_get_polling         tnnc_fmem_get_polling
        #define tn_fmem_get_ipolling        tnnc_fmem_get_ipolling
        #define tn_fmem_release             tnnc_fmem_release
        #define tn_fmem_irelease            tnnc_fmem_irelease
        #define tn_mutex_create             tnnc_mutex_create
        #define tn_mutex_delete             tnnc_mutex_delete
        #define tn_mutex_lock               tnnc_mutex_lock
        #define tn_mutex_lock_polling       tnnc_mutex_lock_polling
        #define tn_mutex_unlock             tnnc_mutex_unlock

    #else

    /* Define API functions without error checking */

        #define tn_sys_enter_critical       tn_enter_critical
        #define tn_sys_exit_critical        tn_exit_critical
        #define tn_sys_context_get          tncm_sys_context_get
        #define tn_start_system             tnec_start_system
        #define tn_tick_int_processing      tnec_tick_int_processing
        #define tn_sys_tslice_ticks         tnec_sys_tslice_ticks
        #define tn_sys_time_get             tncm_sys_time_get
        #define tn_sys_time_set             tncm_sys_time_set
        #define tn_task_reference           tnec_task_reference
        #define tn_task_ireference          tnec_task_ireference
        #define tn_task_create              tnec_task_create
        #define tn_task_terminate           tnec_task_terminate
        #define tn_task_exit                tnec_task_exit
        #define tn_task_delete              tnec_task_delete
        #define tn_task_activate            tnec_task_activate
        #define tn_task_iactivate           tnec_task_iactivate
        #define tn_task_isuspend            tnec_task_isuspend
        #define tn_task_change_priority     tnec_task_change_priority
        #define tn_task_suspend             tnec_task_suspend
        #define tn_task_resume              tnec_task_resume
        #define tn_task_iresume             tnec_task_iresume
        #define tn_task_sleep               tnec_task_sleep
        #define tn_task_wakeup              tnec_task_wakeup
        #define tn_task_iwakeup             tnec_task_iwakeup
        #define tn_task_release_wait        tnec_task_release_wait
        #define tn_task_irelease_wait       tnec_task_irelease_wait
        #define tn_sem_create               tnec_sem_create
        #define tn_sem_delete               tnec_sem_delete
        #define tn_sem_signal               tnec_sem_signal
        #define tn_sem_isignal              tnec_sem_isignal
        #define tn_sem_acquire              tnec_sem_acquire
        #define tn_sem_polling              tnec_sem_polling
        #define tn_sem_ipolling             tnec_sem_ipolling
        #define tn_queue_create             tnec_queue_create
        #define tn_queue_delete             tnec_queue_delete
        #define tn_queue_send               tnec_queue_send
        #define tn_queue_send_polling       tnec_queue_send_polling
        #define tn_queue_isend_polling      tnec_queue_isend_polling
        #define tn_queue_receive            tnec_queue_receive
        #define tn_queue_receive_polling    tnec_queue_receive_polling
        #define tn_queue_ireceive           tnec_queue_ireceive
        #define tn_event_create             tnec_event_create
        #define tn_event_delete             tnec_event_delete
        #define tn_event_wait               tnec_event_wait
        #define tn_event_wait_polling       tnec_event_wait_polling
        #define tn_event_iwait              tnec_event_iwait
        #define tn_event_set                tnec_event_set
        #define tn_event_iset               tnec_event_iset
        #define tn_event_clear              tnec_event_clear
        #define tn_event_iclear             tnec_event_iclear
        #define tn_fmem_create              tnec_fmem_create
        #define tn_fmem_delete              tnec_fmem_delete
        #define tn_fmem_get                 tnec_fmem_get
        #define tn_fmem_get_polling         tnec_fmem_get_polling
        #define tn_fmem_get_ipolling        tnec_fmem_get_ipolling
        #define tn_fmem_release             tnec_fmem_release
        #define tn_fmem_irelease            tnec_fmem_irelease
        #define tn_mutex_create             tnec_mutex_create
        #define tn_mutex_delete             tnec_mutex_delete
        #define tn_mutex_lock               tnec_mutex_lock
        #define tn_mutex_lock_polling       tnec_mutex_lock_polling
        #define tn_mutex_unlock             tnec_mutex_unlock

    #endif


                                /* TNKERNEL USER API*/

    /* System functions */

    void      tn_start_system(TN_UWORD *timer_task_stack,
                              TN_UWORD timer_task_stack_size,
                              TN_UWORD *idle_task_stack,
                              TN_UWORD idle_task_stack_size,
                              void (*app_in_cb)(void),
                              void (*cpu_int_en)(void),
                              void (*idle_user_cb)(void)
                             );

    void      tn_tick_int_processing(void);
    TN_RETVAL tn_sys_tslice_ticks(TN_UWORD priority, TN_TIMEOUT value);
    void      tn_sys_enter_critical(void);
    void      tn_sys_exit_critical(void);

    TN_CONTEXT   tn_sys_context_get(void);

    void         tn_sys_time_set(TN_SYS_TIM_T value);
    TN_SYS_TIM_T tn_sys_time_get(void);

    /* Task control functions */

    TN_RETVAL tn_task_create(TN_TCB *task,
                             void (*task_func)(void *param),
                             TN_UWORD priority,
                             TN_UWORD *task_stack_start,
                             TN_UWORD task_stack_size,
                             void *param,
                             TN_UWORD option
                            );

    TN_RETVAL tn_task_terminate(TN_TCB *task);
    void      tn_task_exit(TN_UWORD attr);
    TN_RETVAL tn_task_delete(TN_TCB *task);
    TN_RETVAL tn_task_activate(TN_TCB *task);
    TN_RETVAL tn_task_iactivate(TN_TCB *task);
    TN_RETVAL tn_task_isuspend(TN_TCB *task);
    TN_RETVAL tn_task_change_priority(TN_TCB *task, TN_UWORD new_priority);
    TN_RETVAL tn_task_suspend(TN_TCB *task);
    TN_RETVAL tn_task_resume(TN_TCB *task);
    TN_RETVAL tn_task_iresume(TN_TCB *task);
    TN_RETVAL tn_task_sleep(TN_TIMEOUT timeout);
    TN_RETVAL tn_task_wakeup(TN_TCB *task);
    TN_RETVAL tn_task_iwakeup(TN_TCB *task);
    TN_RETVAL tn_task_release_wait(TN_TCB *task);
    TN_RETVAL tn_task_irelease_wait(TN_TCB *task);
    TN_RETVAL tn_task_reference(TN_TCB * task, TN_TASK_REF * ref);
    TN_RETVAL tn_task_ireference(TN_TCB * task, TN_TASK_REF * ref);

    /* Semaphores control functions */

    TN_RETVAL tn_sem_create(TN_SEM *sem, TN_UWORD start_value, TN_UWORD max_val);
    TN_RETVAL tn_sem_delete(TN_SEM *sem);
    TN_RETVAL tn_sem_signal(TN_SEM *sem);
    TN_RETVAL tn_sem_isignal(TN_SEM *sem);
    TN_RETVAL tn_sem_acquire(TN_SEM *sem, TN_TIMEOUT timeout);
    TN_RETVAL tn_sem_polling(TN_SEM *sem);
    TN_RETVAL tn_sem_ipolling(TN_SEM *sem);

    /* Queues control functions */

    TN_RETVAL tn_queue_create(TN_DQUE *dque, void **data_fifo, TN_UWORD num_entries);
    TN_RETVAL tn_queue_delete(TN_DQUE *dque);
    TN_RETVAL tn_queue_send(TN_DQUE *dque, void  *data_ptr, TN_TIMEOUT timeout);
    TN_RETVAL tn_queue_send_polling(TN_DQUE *dque, void *data_ptr);
    TN_RETVAL tn_queue_isend_polling(TN_DQUE *dque, void *data_ptr);
    TN_RETVAL tn_queue_receive(TN_DQUE *dque, void **data_ptr, TN_TIMEOUT timeout);
    TN_RETVAL tn_queue_receive_polling(TN_DQUE *dque, void **data_ptr);
    TN_RETVAL tn_queue_ireceive(TN_DQUE *dque, void **data_ptr);

    /* Events control functions */

    TN_RETVAL tn_event_create(TN_EVENT *evf, TN_UWORD attr, TN_UWORD pattern);
    TN_RETVAL tn_event_delete(TN_EVENT *evf);
    TN_RETVAL tn_event_wait(TN_EVENT *evf, TN_UWORD wait_pattern, TN_UWORD wait_mode,
                            TN_UWORD *p_flags_pattern, TN_TIMEOUT timeout);
    TN_RETVAL tn_event_wait_polling(TN_EVENT *evf, TN_UWORD wait_pattern,
                                    TN_UWORD wait_mode, TN_UWORD *p_flags_pattern);
    TN_RETVAL tn_event_iwait(TN_EVENT *evf, TN_UWORD  wait_pattern,
                        TN_UWORD wait_mode, TN_UWORD *p_flags_pattern);
    TN_RETVAL tn_event_set(TN_EVENT *evf, TN_UWORD pattern);
    TN_RETVAL tn_event_iset(TN_EVENT *evf, TN_UWORD pattern);
    TN_RETVAL tn_event_clear(TN_EVENT *evf, TN_UWORD pattern);
    TN_RETVAL tn_event_iclear(TN_EVENT *evf, TN_UWORD pattern);

    /* Memory pools control functions */

    TN_RETVAL tn_fmem_create(TN_FMP *fmp, void *start_addr,
                             TN_UWORD block_size, TN_UWORD num_blocks);
    TN_RETVAL tn_fmem_delete(TN_FMP *fmp);
    TN_RETVAL tn_fmem_get(TN_FMP *fmp, void **p_data, TN_TIMEOUT timeout);
    TN_RETVAL tn_fmem_get_polling(TN_FMP *fmp, void **p_data);
    TN_RETVAL tn_fmem_get_ipolling(TN_FMP *fmp, void **p_data);
    TN_RETVAL tn_fmem_release(TN_FMP *fmp, void  *p_data);
    TN_RETVAL tn_fmem_irelease(TN_FMP *fmp, void  *p_data);

    /* Mutexes control functions */

    TN_RETVAL tn_mutex_create(TN_MUTEX *mutex, TN_UWORD attribute, TN_UWORD ceil_priority);
    TN_RETVAL tn_mutex_delete(TN_MUTEX *mutex);
    TN_RETVAL tn_mutex_lock(TN_MUTEX *mutex, TN_TIMEOUT timeout);
    TN_RETVAL tn_mutex_lock_polling(TN_MUTEX *mutex);
    TN_RETVAL tn_mutex_unlock(TN_MUTEX *mutex);


#endif  /* TN_SOURCE_FILE */

/**************************************************************************************************/
#endif  /* __TNKERNEL_H */

