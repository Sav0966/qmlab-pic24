/*
 *  TNKernel RTOS port for Microchip PIC24/dsPIC
 *
 *  File:        port_extr.h
 *  Component:   PORT
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


#ifndef __PORT_DEFS_H
#define __PORT_DEFS_H

/***************************************************************************************************
   TNKernel internal data types
 **************************************************************************************************/

typedef char                TN_CHAR;
typedef unsigned char       TN_UCHAR;

typedef unsigned int        TN_PRTY_BMP_T;
typedef unsigned long       TN_IDLE_CNT_T;

typedef unsigned int        TN_SYS_TIM_T;

typedef int                 TN_WORD;
typedef unsigned int        TN_UWORD;

typedef TN_UWORD            TN_TIMEOUT;

#define TN_TIMEOUT_MAX      UINT_MAX

#define TN_BITS_IN_INT      (16)
#define TN_ALIG             (2)

/***************************************************************************************************
   System API constants
 **************************************************************************************************/

/* Here the following definition present:

    TN_NUM_PRIORITY     - number of task priorities - 32 for ARM/MIPS and 16 for PIC24/dsPIC
    TN_MIN_STACK_SIZE   - minimum task stack size. This constant must be defined before library
                          compilation
    TN_INTERRUPT_LEVEL  - Interrupt level that can use kernel services - lowerest priority.
                          Used only in PIC24/dsPIC port
*/

#define TN_NUM_PRIORITY     (16)
#define TN_MIN_STACK_SIZE   (32)
#define TN_INTERRUPT_LEVEL  (1UL)

/***************************************************************************************************
   Compiler specific qualifiers
 **************************************************************************************************/

/* Here the following qualifiers defined:

    TN_INLINE   - this qualifier used for inline function
    TN_NORETURN - depended on compiler. This must be compiler specific attribute that told
                  compiler to not include function context save, for example
                  __attribute__((noreturn)) for GCC
    TN_TASK     - the same as TN_NORETURN
    TN_DATA     - this qualifier used for put all TNKernel data in one section
    TN_CODE     - this qualifier used for put all TNKernel code in one section
*/

#define TN_INLINE   inline
#define TN_NORETURN __attribute__((noreturn))
#define TN_UNUSED   __attribute__((unused))         /* means that the variable is meant to be possibly unused */
#define TN_TASK     TN_NORETURN
#define TN_DATA     __attribute__((section("tnk_data")))
#define TN_CODE     __attribute__((section("tnk_code")))


/***************************************************************************************************
   MCU and Compiler specific DATA
 **************************************************************************************************/

extern volatile TN_UWORD __TN_SR __attribute__((sfr(0x0042)));


/***************************************************************************************************
   MCU depended functions
 **************************************************************************************************/

void     tn_start_exe(void);

void     tn_enter_critical(void);
void     tn_exit_critical(void);

void     tn_switch_context(void);
void     tn_switch_context_exit(TN_UWORD sr);

TN_UWORD tn_cpu_save_sr(void);
void     tn_cpu_restore_sr(TN_UWORD sr);

//TN_UWORD tn_is_irq_disabled(void);

TN_UWORD ffs_asm(TN_PRTY_BMP_T val);
TN_UWORD *tn_stack_init(void *task_func, void *stack_start, TN_UWORD stack_size, void *param);

////// USED FOR SAVE AND RESTORE STATUS WITHOUT INTERRUPT DISABLE   ///////

#define  tn_save_sr()            {tn_save_status_reg = __TN_SR;}
#define  tn_rest_sr()            {__TN_SR = tn_save_status_reg;}

////// DISABLE ALL INTERRUPTS IN INTERRUPT                          ///////

#define  tn_idisable_interrupt()    /* tn_save_status_reg = tn_cpu_save_sr() */
#define  tn_ienable_interrupt()     /* tn_cpu_restore_sr(tn_save_status_reg) */

////// DISABLE ALL INTERRUPTS IN TASK                               ///////

#define  tn_disable_interrupt()  {__TN_SR |=  (1 << 5);}
#define  tn_enable_interrupt()   {__TN_SR &= ~(1 << 5);}


/* Context */

#define  tn_is_non_task_context()       (tn_sys_context)
#define  tn_is_non_sys_int_context()    (tn_sys_context != 1)



/***************************************************************************************************
   System interrupt declaration macros
 **************************************************************************************************/

#define tn_sys_interrupt(a)                                 \
    __asm__(".global _"#a);                                 \
    __asm__("_"#a":");                                      \
    __asm__("push        _SPLIM");                          \
    __asm__("push        _SR");                             \
    __asm__("push        W14");                             \
    __asm__("push.d      W12");                             \
    __asm__("push.d      W10");                             \
    __asm__("push.d      W8");                              \
    __asm__("push.d      W6");                              \
    __asm__("push.d      W4");                              \
    __asm__("push.d      W2");                              \
    __asm__("push.d      W0");                              \
    __asm__("push        _RCOUNT");                         \
    __asm__("push        _TBLPAG");                         \
    __asm__("push        _CORCON");                         \
    __asm__("push        _PSVPAG");                         \
                                                            \
    __asm__("mov         #_tn_sys_context,   W0");          \
    __asm__("bset        [W0],               #0");          \
                                                            \
    __asm__("rcall       _"#a"_handler");                   \
                                                            \
    __asm__("mov         #_tn_sys_context,   W0");          \
    __asm__("bclr        [W0],               #0");          \
                                                            \
    __asm__("mov         #0xFFFF,            W0");          \
    __asm__("mov         W0,                 _SPLIM");      \
                                                            \
    __asm__("mov         #_tn_context_switch_request, W0"); \
    __asm__("cp0         [W0]");                            \
    __asm__("bra         Z,                  1f");          \
                                                            \
    __asm__("mov         #0,                 W1");          \
    __asm__("mov         W1,                 [W0]");        \
                                                            \
    __asm__("mov         #_tn_curr_run_task,    W0");       \
    __asm__("mov         #_tn_next_task_to_run, W1");       \
    __asm__("mov         [W0],               W2");          \
    __asm__("mov         [W1],               W3");          \
                                                            \
    __asm__("mov         W15,                [W2]");        \
    __asm__("mov         [W3],               W15");         \
    __asm__("mov         W3,                 [W0]");        \
                                                            \
    __asm__("1:");                                          \
    __asm__("pop         _PSVPAG");                         \
    __asm__("pop         _CORCON");                         \
    __asm__("pop         _TBLPAG");                         \
    __asm__("pop         _RCOUNT");                         \
    __asm__("pop.d       W0");                              \
    __asm__("pop.d       W2");                              \
    __asm__("pop.d       W4");                              \
    __asm__("pop.d       W6");                              \
    __asm__("pop.d       W8");                              \
    __asm__("pop.d       W10");                             \
    __asm__("pop.d       W12");                             \
    __asm__("pop         W14");                             \
    __asm__("pop         _SR");                             \
    __asm__("pop         _SPLIM");                          \
    __asm__("retfie");                                      \
void a##_handler()


/* ================================================================================================================== */
#endif  /* __PORT_DEFS_H */
