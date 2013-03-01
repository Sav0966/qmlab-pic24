/* =================================================================================================

    TNKernel RTOS port for Microchip PIC32
    (c)2010, Alex Borisov

    File:        port.S

    Component:   PORT, assembler part
    Description: Internal routine for disabling interrupt checking

    Compiler:    Microchip C32 v.1.11A
 
    Ver. 2.5
 
    Copyright © 2004,2008 Yuri Tiomkin
    Copyright © 2008-2010 Alex Borisov (PIC24/dsPIC, PIC32 port)

    All rights reserved.

    Permission to use, copy, modify, and distribute this software in source and binary forms and
    its documentation for any purpose and without fee is hereby granted, provided that the above
    copyright notice appear in all copies and that both that copyright notice and this permission
    notice appear in supporting documentation.

    THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
    AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL YURI TIOMKIN OR 
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
    OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

 ===========================================================================================-=====*/

        /* Executing code section */

        .section .text,"ax",@progbits
        .set    nomips16
        .set    noreorder
        .set    noat


        .extern tn_system_state
        .extern tn_curr_run_task
        .extern tn_next_task_to_run
        .extern tn_sys_context


        //.global tn_is_irq_disabled
        .global tn_enter_critical
        .global tn_exit_critical
        .global ffs_asm
        .global tn_cpu_restore_sr
        .global tn_stack_init
        .global tn_cpu_save_sr
        .global tn_start_exe
        .global tn_switch_context_exit
        .global tn_switch_context


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
#define     STK_OFFSET_CURRENT(a)   136(a)      // Stack position after task push


/* ********************************************************************************************** */
//              Check disabled interrupts 
/* ********************************************************************************************** */
            

//        .ent tn_is_irq_disabled
//tn_is_irq_disabled:
//
//        .end tn_is_irq_disabled


/* ********************************************************************************************** */
//              Enter Critical Section
/* ********************************************************************************************** */

        .ent tn_enter_critical

tn_enter_critical:
        
        la      $t0,    tn_sys_context
        lw      $t0,    0($t0)
        bne     $t0,    $zero,      1f          /* return if non-task context */
        nop
    
        di                                      /* disable interrupts */

        addiu   $t1,    $zero,      2           /* save context variable */
        la      $t0,    tn_sys_context
        sw      $t1,    0($t0)

1:
        jr      $ra                             /* return to caller */
        nop

        .end tn_enter_critical


/* ********************************************************************************************** */
//              Exit Critical Section
/* ********************************************************************************************** */

        .ent tn_exit_critical

tn_exit_critical:

        la      $t0,    tn_sys_context
        lw      $t0,    0($t0)
        addiu   $t1,    $zero,      2
        bne     $t0,    $t1,        1f          /* return if task context */
        nop

        ei                                      /* enable interrupts */

        la      $t0,    tn_sys_context
        sw      $zero,  0($t0)                  /* save context variable */

1:
        jr      $ra                             /* return to caller */
        nop

        .end tn_exit_critical


/* ********************************************************************************************** */
//              Find first task
/* ********************************************************************************************** */

    /* Нахождение первой старшей единицы в слове */

        .ent ffs_asm
ffs_asm:
        move    $t0,    $a0
        subu    $t0,    $zero,  $t0
        and     $a0,    $a0,    $t0

        clz     $a0,    $a0
        addiu   $t0,    $zero,  32
        subu    $v0,    $t0,    $a0

        jr      $ra                             /* return to caller */
        nop

        .end ffs_asm

/* ********************************************************************************************** */
//              Save CPU Status Register
/* ********************************************************************************************** */

        .ent tn_cpu_save_sr
tn_cpu_save_sr:
        jr      $ra                             /* return to caller */
        nop
        .end tn_cpu_save_sr

/* ********************************************************************************************** */
//              Restore CPU Status register
/* ********************************************************************************************** */

        .ent tn_cpu_restore_sr
tn_cpu_restore_sr:
        jr      $ra                             /* return to caller */
        nop
        .end tn_cpu_restore_sr

/* ********************************************************************************************** */
//              Stack Init
/* ********************************************************************************************** */

        .ent tn_stack_init

tn_stack_init:
        
        // a0 - task function address
        // a1 - stack low address
        // a2 - stack size
        // a3 - task parameter        
        
        sll     $t0,    $a2,    2                       /* $t0 = stack size in bytes            */
        addu    $t0,    $t0,    $a1
        addiu   $t0,    $t0,    -(STK_CONTEXT_SIZE+4)   /* get start of stack frame for save    */

        /* Initialize stack */        
    
        mfc0    $t1,    $12                     /* load SR                              */
        ori     $t1,    $t1,    3
        sw      $t1,    STK_OFFSET_SR($t0)      /* save SR to task stack                */

        sw      $a0,    STK_OFFSET_EPC($t0)     /* epc <- task address                  */
        sw      $zero,  STK_OFFSET_MLO($t0)     /* mlo  */
        sw      $zero,  STK_OFFSET_MHI($t0)     /* mhi  */

        addiu   $t1,    $zero,  1
        sw      $t1,    STK_OFFSET_AT($t0)      /* $1   */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_V0($t0)      /* $2   */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_V1($t0)      /* $3   */
        sw      $a3,    STK_OFFSET_A0($t0)      /* $4 - task argument   */
        addiu   $t1,    $t1,    2
        sw      $t1,    STK_OFFSET_A1($t0)      /* $5   */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_A2($t0)      /* $6   */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_A3($t0)      /* $7   */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_T0($t0)      /* $8   */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_T1($t0)      /* $9   */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_T2($t0)      /* $10  */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_T3($t0)      /* $11  */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_T4($t0)      /* $12  */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_T5($t0)      /* $13  */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_T6($t0)      /* $14  */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_T7($t0)      /* $15  */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_S0($t0)      /* $16  */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_S1($t0)      /* $17  */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_S2($t0)      /* $18  */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_S3($t0)      /* $19  */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_S4($t0)      /* $20  */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_S5($t0)      /* $21  */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_S6($t0)      /* $22  */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_S7($t0)      /* $23  */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_T8($t0)      /* $24  */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_T9($t0)      /* $25  */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_K0($t0)      /* $26  */
        addiu   $t1,    $t1,    1
        sw      $t1,    STK_OFFSET_K1($t0)      /* $27  */
        sw      $gp,    STK_OFFSET_GP($t0)      /* $28  */
        addiu   $t1,    $zero,  0x30
        sw      $t1,    STK_OFFSET_FP($t0)      /* $30  */
        sw      $a0,    STK_OFFSET_RA($t0)      /* $31 <- Task Function Address */
        sw      $zero,  STK_OFFSET_CURRENT($t0) 

        move    $v0,    $t0                     /* Return Task stack pointer    */
    
        jr      $ra                             /* return to caller             */
        nop

        .end tn_stack_init


/* ********************************************************************************************** */
//              Start OS Execution
/* ********************************************************************************************** */

        .ent tn_start_exe

tn_start_exe:

        addi    $t0,    $zero,  1
        la      $t1,    tn_system_state
        sb      $t0,    0($t1)

        .end tn_start_exe

        // Switch context to the first task 

        .ent tn_switch_context_exit

tn_switch_context_exit:

        la      $t0,    tn_curr_run_task
        la      $t1,    tn_next_task_to_run

        lw      $t1,    0($t1)                  // TCB address of next task to t1

        lw      $sp,    0($t1)                  // Next Task stack address to stack pointer
        sw      $t1,    0($t0)                  // Save Next task TCB pointer to Current task pointer

        /* Restore Task Stack */

        lw      $t0,    STK_OFFSET_SR($sp)      // SR
        mtc0    $t0,    $12,    0

        lw      $t0,    STK_OFFSET_EPC($sp)     // EPC
        mtc0    $t0,    $14,    0

        lw      $t0,    STK_OFFSET_MLO($sp)     // LO
        lw      $t1,    STK_OFFSET_MHI($sp)     // HI
        mtlo    $t0
        mthi    $t1

        lw      $at,    STK_OFFSET_AT($sp)      //  1
        lw      $v0,    STK_OFFSET_V0($sp)      //  2
        lw      $v1,    STK_OFFSET_V1($sp)      //  3
        lw      $a0,    STK_OFFSET_A0($sp)      //  4   - task argument
        lw      $a1,    STK_OFFSET_A1($sp)      //  5
        lw      $a2,    STK_OFFSET_A2($sp)      //  6
        lw      $a3,    STK_OFFSET_A3($sp)      //  7
        lw      $t0,    STK_OFFSET_T0($sp)      //  8
        lw      $t1,    STK_OFFSET_T1($sp)      //  9
        lw      $t2,    STK_OFFSET_T2($sp)      //  10
        lw      $t3,    STK_OFFSET_T3($sp)      //  11
        lw      $t4,    STK_OFFSET_T4($sp)      //  12
        lw      $t5,    STK_OFFSET_T5($sp)      //  13
        lw      $t6,    STK_OFFSET_T6($sp)      //  14
        lw      $t7,    STK_OFFSET_T7($sp)      //  15
        lw      $s0,    STK_OFFSET_S0($sp)      //  16
        lw      $s1,    STK_OFFSET_S1($sp)      //  17
        lw      $s2,    STK_OFFSET_S2($sp)      //  18
        lw      $s3,    STK_OFFSET_S3($sp)      //  19
        lw      $s4,    STK_OFFSET_S4($sp)      //  20
        lw      $s5,    STK_OFFSET_S5($sp)      //  21
        lw      $s6,    STK_OFFSET_S6($sp)      //  22
        lw      $s7,    STK_OFFSET_S7($sp)      //  23
        lw      $t8,    STK_OFFSET_T8($sp)      //  24
        lw      $t9,    STK_OFFSET_T9($sp)      //  25
        lw      $k0,    STK_OFFSET_K0($sp)      //  26
        lw      $k1,    STK_OFFSET_K1($sp)      //  27
        lw      $gp,    STK_OFFSET_GP($sp)      //  28
        lw      $fp,    STK_OFFSET_FP($sp)      //  30
        lw      $ra,    STK_OFFSET_RA($sp)      //  31

        jr      $ra
        addi    $sp,    $sp,    STK_CONTEXT_SIZE

        .end tn_switch_context_exit


/* ********************************************************************************************** */
//              Switch OS Context
/* ********************************************************************************************** */


        .ent tn_switch_context

tn_switch_context:
    
        /* SAVE CURRENT CONTEXT */

        addiu   $sp,    $sp,    -(STK_CONTEXT_SIZE)

        sw      $at,    STK_OFFSET_AT($sp)      //  1
        sw      $v0,    STK_OFFSET_V0($sp)      //  2
        sw      $v1,    STK_OFFSET_V1($sp)      //  3
        sw      $a0,    STK_OFFSET_A0($sp)      //  4   - task argument
        sw      $a1,    STK_OFFSET_A1($sp)      //  5
        sw      $a2,    STK_OFFSET_A2($sp)      //  6
        sw      $a3,    STK_OFFSET_A3($sp)      //  7
        sw      $t0,    STK_OFFSET_T0($sp)      //  8
        sw      $t1,    STK_OFFSET_T1($sp)      //  9
        sw      $t2,    STK_OFFSET_T2($sp)      //  10
        sw      $t3,    STK_OFFSET_T3($sp)      //  11
        sw      $t4,    STK_OFFSET_T4($sp)      //  12
        sw      $t5,    STK_OFFSET_T5($sp)      //  13
        sw      $t6,    STK_OFFSET_T6($sp)      //  14
        sw      $t7,    STK_OFFSET_T7($sp)      //  15
        sw      $s0,    STK_OFFSET_S0($sp)      //  16
        sw      $s1,    STK_OFFSET_S1($sp)      //  17
        sw      $s2,    STK_OFFSET_S2($sp)      //  18
        sw      $s3,    STK_OFFSET_S3($sp)      //  19
        sw      $s4,    STK_OFFSET_S4($sp)      //  20
        sw      $s5,    STK_OFFSET_S5($sp)      //  21
        sw      $s6,    STK_OFFSET_S6($sp)      //  22
        sw      $s7,    STK_OFFSET_S7($sp)      //  23
        sw      $t8,    STK_OFFSET_T8($sp)      //  24
        sw      $t9,    STK_OFFSET_T9($sp)      //  25
        sw      $k0,    STK_OFFSET_K0($sp)      //  26
        sw      $k1,    STK_OFFSET_K1($sp)      //  27
        sw      $gp,    STK_OFFSET_GP($sp)      //  28
        sw      $fp,    STK_OFFSET_FP($sp)      //  30
        sw      $ra,    STK_OFFSET_RA($sp)      //  31

        mflo    $t0
        mfhi    $t1
        sw      $t0,    STK_OFFSET_MLO($sp)     // MLO
        sw      $t1,    STK_OFFSET_MHI($sp)     // MHI

        //mfc0    $t0,    $14,    0               /* Save the EPC                                         */
        //???? addi    $t0,    $t0,    4               /* EPC will reference the instruction following syscall */
        sw      $ra,    STK_OFFSET_EPC($sp)

        mfc0    $t0,    $12,    0
        ori     $t0,    $t0,    2
        sw      $t0,    STK_OFFSET_SR($sp)      // SR

    

        di                                      // disable interrupts 
    
        /* Check for next task TCB */
        
        la      $t0,    tn_curr_run_task
        la      $t1,    tn_next_task_to_run

        lw      $t0,    0($t0)                  // TCB address of current task
        lw      $t1,    0($t1)                  // TCB address of next task

        beq     $t1,    $t0,    1f              // Restore context if next task is current task
        nop
        beq     $t1,    $zero,  1f              // Restore context if next task == NULL
        nop

        /* Exchange task pointers */

        sw      $sp,    0($t0)                  // Save SP of current task
        lw      $sp,    0($t1)                  // Load SP of next task

        la      $t0,    tn_curr_run_task
        sw      $t1,    0($t0)                  // Save Next task TCB pointer to Current task pointer

1:
        ei                                      // enable interrupts 

        lw      $t0,    STK_OFFSET_MLO($sp)     // LO
        lw      $t1,    STK_OFFSET_MHI($sp)     // HI
        mtlo    $t0
        mthi    $t1
        lw      $at,    STK_OFFSET_AT($sp)      //  1
        lw      $v0,    STK_OFFSET_V0($sp)      //  2
        lw      $v1,    STK_OFFSET_V1($sp)      //  3
        lw      $a0,    STK_OFFSET_A0($sp)      //  4   - task argument
        lw      $a1,    STK_OFFSET_A1($sp)      //  5
        lw      $a2,    STK_OFFSET_A2($sp)      //  6
        lw      $a3,    STK_OFFSET_A3($sp)      //  7
        lw      $t0,    STK_OFFSET_T0($sp)      //  8
        lw      $t1,    STK_OFFSET_T1($sp)      //  9
        lw      $t2,    STK_OFFSET_T2($sp)      //  10
        lw      $t3,    STK_OFFSET_T3($sp)      //  11
        lw      $t4,    STK_OFFSET_T4($sp)      //  12
        lw      $t5,    STK_OFFSET_T5($sp)      //  13
        lw      $t6,    STK_OFFSET_T6($sp)      //  14
        lw      $t7,    STK_OFFSET_T7($sp)      //  15
        lw      $s0,    STK_OFFSET_S0($sp)      //  16
        lw      $s1,    STK_OFFSET_S1($sp)      //  17
        lw      $s2,    STK_OFFSET_S2($sp)      //  18
        lw      $s3,    STK_OFFSET_S3($sp)      //  19
        lw      $s4,    STK_OFFSET_S4($sp)      //  20
        lw      $s5,    STK_OFFSET_S5($sp)      //  21
        lw      $s6,    STK_OFFSET_S6($sp)      //  22
        lw      $s7,    STK_OFFSET_S7($sp)      //  23
        lw      $t8,    STK_OFFSET_T8($sp)      //  24
        lw      $t9,    STK_OFFSET_T9($sp)      //  25
        lw      $k0,    STK_OFFSET_K0($sp)      //  26
        lw      $k1,    STK_OFFSET_K1($sp)      //  27
        lw      $gp,    STK_OFFSET_GP($sp)      //  28
        lw      $fp,    STK_OFFSET_FP($sp)      //  30
        lw      $ra,    STK_OFFSET_RA($sp)      //  31        

        /* Like interrupt... */

        di
        ehb

        lw      $k0,    STK_OFFSET_EPC($sp)     // EPC
        lw      $k1,    STK_OFFSET_SR($sp)      // SR
        mtc0    $k0,    $14,    0
        addiu   $sp,    $sp,    STK_CONTEXT_SIZE
        mtc0    $k1,    $12,    0
        

        eret

        .end tn_switch_context

/* ============================================================================================== */

