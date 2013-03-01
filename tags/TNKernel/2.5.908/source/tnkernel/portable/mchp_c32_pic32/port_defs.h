/*
 *  TNKernel RTOS port for Microchip PIC32
 *
 *  File:        port_extr.h
 *  Component:   PORT
 *  Compiler:    Microchip C33 v.1.10(B)
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

#define  TN_BITS_IN_INT     (32)
#define  TN_ALIG            (4)


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

#define TN_NUM_PRIORITY     (32)
#define TN_MIN_STACK_SIZE   (48)
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
#define TN_UNUSED   __attribute__((unused)) /* means that the variable is meant to be possibly unused */
#define TN_TASK     TN_NORETURN
#define TN_DATA
#define TN_CODE

/***************************************************************************************************
   MCU and Compiler specific DATA
 **************************************************************************************************/



/***************************************************************************************************
   MCU depended functions
 **************************************************************************************************/

void     tn_start_exe(void);

void     tn_enter_critical(void);
void     tn_exit_critical(void);

void     tn_switch_context(void);
void     tn_switch_context_exit(TN_UWORD sr);


//TN_UWORD tn_cpu_save_sr(void);
//void     tn_cpu_restore_sr(TN_UWORD sr);

TN_UWORD ffs_asm(TN_PRTY_BMP_T val);
TN_UWORD *tn_stack_init(void *task_func, void *stack_start, TN_UWORD stack_size, void *param);

/* TN_UWORD tn_is_irq_disabled(void); */

////// USED FOR SAVE AND RESTORE STATUS WITHOUT INTERRUPT DISABLE   ///////

#define  tn_save_sr()               {tn_save_status_reg = __builtin_mfc0(12, 0);}
#define  tn_rest_sr()               {__builtin_mtc0(12, 0, tn_save_status_reg);}

////// DISABLE ALL INTERRUPTS IN INTERRUPT                          ///////

#define  tn_idisable_interrupt()    /* tn_save_status_reg = tn_cpu_save_sr() */
#define  tn_ienable_interrupt()     /* tn_cpu_restore_sr(tn_save_status_reg) */

////// DISABLE ALL INTERRUPTS IN TASK                               ///////

#define  tn_disable_interrupt()     asm volatile("di")
#define  tn_enable_interrupt()      asm volatile("ei")

/* Context */

#define  tn_is_non_task_context()       (tn_sys_context)
#define  tn_is_non_sys_int_context()    (tn_sys_context != 1)


/***************************************************************************************************
   System interrupt declaration macros
 **************************************************************************************************/

#define     STK_CONTEXT_SIZE        136

#define     STK_OFFSET_SR(a)          0(a)
#define     STK_OFFSET_EPC(a)         4(a)
#define     STK_OFFSET_MLO(a)         8(a)
#define     STK_OFFSET_MHI(a)        12(a)
#define     STK_OFFSET_AT(a)         16(a)      // $1
#define     STK_OFFSET_V0(a)         20(a)      // $2
#define     STK_OFFSET_V1(a)         24(a)      // $3
#define     STK_OFFSET_A0(a)         28(a)      // $4 - task argument
#define     STK_OFFSET_A1(a)         32(a)      // $5
#define     STK_OFFSET_A2(a)         36(a)      // $6
#define     STK_OFFSET_A3(a)         40(a)      // $7
#define     STK_OFFSET_T0(a)         44(a)      // $8
#define     STK_OFFSET_T1(a)         48(a)      // $9
#define     STK_OFFSET_T2(a)         52(a)      // $10
#define     STK_OFFSET_T3(a)         56(a)      // $11
#define     STK_OFFSET_T4(a)         60(a)      // $12
#define     STK_OFFSET_T5(a)         64(a)      // $13
#define     STK_OFFSET_T6(a)         68(a)      // $14
#define     STK_OFFSET_T7(a)         72(a)      // $15
#define     STK_OFFSET_S0(a)         76(a)      // $16
#define     STK_OFFSET_S1(a)         80(a)      // $17
#define     STK_OFFSET_S2(a)         84(a)      // $18
#define     STK_OFFSET_S3(a)         88(a)      // $19
#define     STK_OFFSET_S4(a)         92(a)      // $20
#define     STK_OFFSET_S5(a)         96(a)      // $21
#define     STK_OFFSET_S6(a)        100(a)      // $22
#define     STK_OFFSET_S7(a)        104(a)      // $23
#define     STK_OFFSET_T8(a)        108(a)      // $24
#define     STK_OFFSET_T9(a)        112(a)      // $25
#define     STK_OFFSET_K0(a)        116(a)      // $26
#define     STK_OFFSET_K1(a)        120(a)      // $27
#define     STK_OFFSET_GP(a)        124(a)      // $28
#define     STK_OFFSET_FP(a)        128(a)      // $30
#define     STK_OFFSET_RA(a)        132(a)      // $31

#define tn_sys_interrupt(v)                                                     \
    __attribute__((__noinline__)) void _func##v(void);                          \
    void __attribute__((naked))__attribute__((vector(v))) _isr##v(void)         \
    {                                                                           \
        asm volatile (".set noreorder");                                        \
        asm volatile (".set noat");                                             \
            \
        asm volatile ("rdpgpr       $sp,    $sp");                              \
        asm volatile ("mfc0         $k0,    $13"); /* Cause */                  \
        asm volatile ("mfc0         $k1,    $14"); /* EPC */                    \
        asm volatile ("srl          $k0,    $k0,    0xa");                      \
            \
            /* Save Context To Stack */                                         \
        asm volatile ("addiu        $sp,    $sp,    -136");                     \
        asm volatile ("sw           $k1,    4($sp)");  /* EPC */                \
            \
        asm volatile ("mfc0         $k1,    $12"); /* STATUS */                 \
        asm volatile ("sw           $k1,    0($sp)");  /* SR */                 \
        asm volatile ("ins          $k1,    $k0,    10,     6");                \
        asm volatile ("ins          $k1,    $zero,  1,      4");                \
        asm volatile ("mtc0         $k1,    $12");  /* Save Status */           \
            \
        asm volatile ("sw      $at,     16($sp) ");  /*  1  */                  \
        asm volatile ("sw      $v0,     20($sp) ");  /*  2  */                  \
        asm volatile ("sw      $v1,     24($sp) ");  /*  3  */                  \
        asm volatile ("sw      $a0,     28($sp) ");  /*  4  */                  \
        asm volatile ("sw      $a1,     32($sp) ");  /*  5  */                  \
        asm volatile ("sw      $a2,     36($sp) ");  /*  6  */                  \
        asm volatile ("sw      $a3,     40($sp) ");  /*  7  */                  \
        asm volatile ("sw      $t0,     44($sp) ");  /*  8  */                  \
        asm volatile ("sw      $t1,     48($sp) ");  /*  9  */                  \
        asm volatile ("sw      $t2,     52($sp) ");  /*  10 */                  \
        asm volatile ("sw      $t3,     56($sp) ");  /*  11 */                  \
        asm volatile ("sw      $t4,     60($sp) ");  /*  12 */                  \
        asm volatile ("sw      $t5,     64($sp) ");  /*  13 */                  \
        asm volatile ("sw      $t6,     68($sp) ");  /*  14 */                  \
        asm volatile ("sw      $t7,     72($sp) ");  /*  15 */                  \
        asm volatile ("sw      $s0,     76($sp) ");  /*  16 */                  \
        asm volatile ("sw      $s1,     80($sp) ");  /*  17 */                  \
        asm volatile ("sw      $s2,     84($sp) ");  /*  18 */                  \
        asm volatile ("sw      $s3,     88($sp) ");  /*  19 */                  \
        asm volatile ("sw      $s4,     92($sp) ");  /*  20 */                  \
        asm volatile ("sw      $s5,     96($sp) ");  /*  21 */                  \
        asm volatile ("sw      $s6,    100($sp) ");  /*  22 */                  \
        asm volatile ("sw      $s7,    104($sp) ");  /*  23 */                  \
        asm volatile ("sw      $t8,    108($sp) ");  /*  24 */                  \
        asm volatile ("sw      $t9,    112($sp) ");  /*  25 */                  \
        asm volatile ("sw      $k0,    116($sp) ");  /*  26 */                  \
        asm volatile ("sw      $k1,    120($sp) ");  /*  27 */                  \
        asm volatile ("sw      $gp,    124($sp) ");  /*  28 */                  \
        asm volatile ("sw      $fp,    128($sp) ");  /*  30 */                  \
        asm volatile ("sw      $ra,    132($sp) ");  /*  31 */                  \
        asm volatile ("mflo    $t0");                                           \
        asm volatile ("mfhi    $t1");                                           \
        asm volatile ("sw      $t0,      8($sp)");   /* MLO */                  \
        asm volatile ("sw      $t1,     12($sp)");   /* MHI */                  \
            \
            \
            /* Set interrupt context and call interrupt handler */              \
        asm volatile ("la           $t0,    tn_sys_context");                   \
        asm volatile ("addiu        $t1,    $zero,      1");                    \
        asm volatile ("sw           $t1,    0($t0)");                           \
        asm volatile ("jal          _func"#v);                                  \
        asm volatile ("nop");                                                   \
                                                                                \
            /* Restore task context */                                          \
        asm volatile ("la           $t0,    tn_sys_context");                   \
        asm volatile ("sw           $zero,  0($t0)");                           \
                                                                                \
            /* Check for switch request */                                      \
        asm volatile ("la           $t0,    tn_context_switch_request");        \
        asm volatile ("lw           $t0,    0($t0)");                           \
        asm volatile ("beq          $t0,    $zero,  1f");                       \
        asm volatile ("nop");                                                   \
        asm volatile ("la           $t0,    tn_context_switch_request");        \
        asm volatile ("sw           $zero,  0($t0)");                           \
            \
        asm volatile ("la      $t0,    tn_curr_run_task");                      \
        asm volatile ("la      $t1,    tn_next_task_to_run");                   \
        asm volatile ("lw      $t0,    0($t0)"); /* TCB current task */         \
        asm volatile ("lw      $t1,    0($t1)"); /* TCB next task */            \
            \
        asm volatile ("sw      $sp,    0($t0)");                                \
        asm volatile ("lw      $sp,    0($t1)");                                \
        asm volatile ("la      $t0,    tn_curr_run_task");                      \
        asm volatile ("sw      $t1,    0($t0)");                                \
            \
            \
            /* Restore Context */                                               \
        asm volatile ("1:");                                                    \
        asm volatile ("lw      $t0,      8($sp)");     /* LO */                 \
        asm volatile ("lw      $t1,     12($sp)");     /* HI */                 \
        asm volatile ("mtlo    $t0");                                           \
        asm volatile ("mthi    $t1");                                           \
        asm volatile ("lw      $at,     16($sp)");      /*  1  */               \
        asm volatile ("lw      $v0,     20($sp)");      /*  2  */               \
        asm volatile ("lw      $v1,     24($sp)");      /*  3  */               \
        asm volatile ("lw      $a0,     28($sp)");      /*  4  */               \
        asm volatile ("lw      $a1,     32($sp)");      /*  5  */               \
        asm volatile ("lw      $a2,     36($sp)");      /*  6  */               \
        asm volatile ("lw      $a3,     40($sp)");      /*  7  */               \
        asm volatile ("lw      $t0,     44($sp)");      /*  8  */               \
        asm volatile ("lw      $t1,     48($sp)");      /*  9  */               \
        asm volatile ("lw      $t2,     52($sp)");      /*  10 */               \
        asm volatile ("lw      $t3,     56($sp)");      /*  11 */               \
        asm volatile ("lw      $t4,     60($sp)");      /*  12 */               \
        asm volatile ("lw      $t5,     64($sp)");      /*  13 */               \
        asm volatile ("lw      $t6,     68($sp)");      /*  14 */               \
        asm volatile ("lw      $t7,     72($sp)");      /*  15 */               \
        asm volatile ("lw      $s0,     76($sp)");      /*  16 */               \
        asm volatile ("lw      $s1,     80($sp)");      /*  17 */               \
        asm volatile ("lw      $s2,     84($sp)");      /*  18 */               \
        asm volatile ("lw      $s3,     88($sp)");      /*  19 */               \
        asm volatile ("lw      $s4,     92($sp)");      /*  20 */               \
        asm volatile ("lw      $s5,     96($sp)");      /*  21 */               \
        asm volatile ("lw      $s6,    100($sp)");      /*  22 */               \
        asm volatile ("lw      $s7,    104($sp)");      /*  23 */               \
        asm volatile ("lw      $t8,    108($sp)");      /*  24 */               \
        asm volatile ("lw      $t9,    112($sp)");      /*  25 */               \
        asm volatile ("lw      $k0,    116($sp)");      /*  26 */               \
        asm volatile ("lw      $k1,    120($sp)");      /*  27 */               \
        asm volatile ("lw      $gp,    124($sp)");      /*  28 */               \
        asm volatile ("lw      $fp,    128($sp)");      /*  30 */               \
        asm volatile ("lw      $ra,    132($sp)");      /*  31 */               \
        asm volatile ("di");                                                    \
        asm volatile ("ehb");                                                   \
            \
        asm volatile ("lw      $k0,    0($sp)");        /* SR */                \
        asm volatile ("lw      $k1,    4($sp)");        /* EPC */               \
        asm volatile ("mtc0    $k1,    $14,    0");                             \
        asm volatile ("addiu   $sp,    $sp,    136");                           \
        asm volatile ("wrpgpr  $sp,    $sp");                                   \
        asm volatile ("mtc0    $k0,    $12,    0");                             \
            \
        asm volatile ("eret");                                                  \
        asm volatile (".set reorder");                                          \
        asm volatile (".set at");                                               \
    } __attribute((__noinline__)) void _func##v(void)



/* ================================================================================================================== */
#endif  /* __PORT_DEFS_H */
