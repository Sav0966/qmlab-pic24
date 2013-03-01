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

        PRESERVE8

        /* Executing code section */

    IMPORT tn_system_state
    IMPORT tn_curr_run_task
    IMPORT tn_next_task_to_run
    IMPORT tn_sys_context
    IMPORT tn_int_counter
    IMPORT tnec_task_exit
    IMPORT tn_context_switch_request


    EXPORT tn_enter_critical
    EXPORT tn_exit_critical
    EXPORT ffs_asm
    EXPORT tn_cpu_restore_sr
    EXPORT tn_stack_init
    EXPORT tn_cpu_save_sr
    EXPORT tn_cpu_save_sr_idis
    EXPORT tn_start_exe
    EXPORT tn_switch_context_exit
    EXPORT tn_switch_context
    EXPORT PendSV_Handler
    EXPORT tn_int_exit

// --  Interrupt Control State Register Address
ICSR_ADDR        EQU     0xE000ED04

// --  pendSV bit in the Interrupt Control State Register
PENDSVSET        EQU     0x10000000

// --  System Handlers 12-15 Priority Register Address
PR_12_15_ADDR    EQU     0xE000ED20

// --  PRI_14 (PendSV) priority in the System Handlers 12-15 Priority Register Address
// --  PendSV priority is minimal (0xFF)
PENDS_VPRIORITY  EQU     0x00FF0000



        AREA    |.text|, CODE, READONLY, ALIGN=3
        THUMB
        REQUIRE8
        PRESERVE8


/* ********************************************************************************************** */
//              Check disabled interrupts 
/* ********************************************************************************************** */
            


/* ********************************************************************************************** */
//              Enter Critical Section
/* ********************************************************************************************** */

tn_enter_critical
     cpsid  I
     bx     lr

/* ********************************************************************************************** */
//              Exit Critical Section
/* ********************************************************************************************** */

tn_exit_critical
     cpsie I
     bx   lr

/* ********************************************************************************************** */
//              Find first task
/* ********************************************************************************************** */

ffs_asm

       mov    r1, r0                    ;-- tmp = in
       rsbs   r0, r1, #0                ;-- in = -in
       ands   r0, r0, r1                ;-- in = in & tmp
       CLZ.W  r0, r0
       rsb    r0, r0, #0x20             ;-- 32 - in
       bx     lr


/* ********************************************************************************************** */
//              Save CPU Status Register
/* ********************************************************************************************** */

tn_cpu_save_sr

       mrs    r0, PRIMASK
       //cpsid  I
       bx     lr

/* ********************************************************************************************** */
//              Save CPU Status Register and Disable interrupts 
/* ********************************************************************************************** */

tn_cpu_save_sr_idis

       mrs    r0, PRIMASK
       cpsid  I
       bx     lr

/* ********************************************************************************************** */
//              Restore CPU Status register
/* ********************************************************************************************** */

tn_cpu_restore_sr

       msr    PRIMASK, r0
       bx     lr


/* ********************************************************************************************** */
//              Stack Init
/* ********************************************************************************************** */

tn_stack_init
        // r0 - task function address
        // r1 - stack low address
        // r2 - stack size
        // r3 - task parameter

        PUSH     {r4-r5,lr}
        MOV      r4, r0
        SUBS     r5, r2, #1
        LSL      r5, r5, #2        
        ADD      r0, r1, r5
        //LDR      r0, [r1, r5, LSL #2]

        //*stk = 0x01000000L
        MOV      r5, #0x1000000
        STR      r5, [r0, #0x00]
        SUBS     r0,r0,#4

        //*stk = ((unsigned int)task_func) | 1;    // Entry Point (1 for THUMB mode) 
        ORR      r5,r4,#0x01
        STR      r5,[r0,#0x00]
        SUBS     r0,r0,#4

        //*stk = ((unsigned int)tn_task_exit) | 1; // R14 (LR) (1 for THUMB mode) 
        LDR      r5, =tnec_task_exit
        ORR      r5, r5, #0x01
        STR      r5, [r0, #0x00]
        SUBS     r0, r0, #4

        //*stk = 0x12121212L;                      // R12 
        MOV      r5,#0x12121212
        STR      r5,[r0,#0x00]
        SUBS     r0,r0,#4

        //*stk = 0x03030303L;                      // R3 
        MOV      r5,#0x3030303
        STR      r5,[r0,#0x00]
        SUBS     r0,r0,#4

        //*stk = 0x02020202L;                      // R2 
        MOV      r5,#0x2020202
        STR      r5,[r0,#0x00]
        SUBS     r0,r0,#4

        //*stk = 0x01010101L;                      // R1 
        ASRS     r5,r5,#1
        STR      r5,[r0,#0x00]
        SUBS     r0,r0,#4

        //*stk = (unsigned int)param;              // R0 : task's function argument 
        STR      r3,[r0,#0x00]
        SUBS     r0,r0,#4

        //*stk = 0x11111111L;                      // R11 
        MOV      r5,#0x11111111
        STR      r5,[r0,#0x00]
        SUBS     r0,r0,#4

        //*stk = 0x10101010L;                      // R10 
        MOV      r5,#0x10101010
        STR      r5,[r0,#0x00]
        SUBS     r0,r0,#4

        //*stk = 0x09090909L;                      // R9 
        MOV      r5,#0x9090909
        STR      r5,[r0,#0x00]
        SUBS     r0,r0,#4

        //*stk = 0x08080808L;                      // R8 
        MOV      r5,#0x8080808
        STR      r5,[r0,#0x00]
        SUBS     r0,r0,#4

        //*stk = 0x07070707L;                      // R7 
        MOV      r5,#0x7070707
        STR      r5,[r0,#0x00]
        SUBS     r0,r0,#4

        //*stk = 0x06060606L;                      // R6 
        MOV      r5,#0x6060606
        STR      r5,[r0,#0x00]
        SUBS     r0,r0,#4

        //*stk = 0x05050505L;                      // R5 
        MOV      r5,#0x5050505
        STR      r5,[r0,#0x00]
        SUBS     r0,r0,#4

        //*stk = 0x04040404L;                      // R4 
        MOV      r5,#0x4040404
        STR      r5,[r0,#0x00]

        POP      {r4-r5,pc}


/* ********************************************************************************************** */
//              Start OS Execution
/* ********************************************************************************************** */

tn_start_exe

       ldr    r1, =PR_12_15_ADDR                ;-- Load the System 12-15 Priority Register
       ldr    r0, [r1]
       orr    r0, r0, #PENDS_VPRIORITY          ;-- set PRI_14 (PendSV) to 0xFF - minimal
       str    r0, [r1]

       ldr    r1,=tn_system_state               ;-- Indicate that system has started
       mov    r0,#1                             ;-- 1 -> TN_SYS_STATE_RUNNING
       strb   r0,[r1]

  ;---------------

       ldr    r0, =tn_context_switch_request    ; set the context switch
       mov    r1, #1
       str    r1, [r0]

       ldr    r1, =tn_curr_run_task             ; =tn_next_task_to_run
       ldr    r2, [r1]
       ldr    r0, [r2]                          ;-- in r0 - new task SP

       ldmia  r0!, {r4-r11}
       msr    PSP, r0
       orr    lr, lr, #0x04                     ;-- Force to new process PSP

tn_switch_context_exit

       ldr    r1, =ICSR_ADDR                    ;-- Trigger PendSV exception
       ldr    r0, =PENDSVSET
       str    r0, [r1]
       cpsie  I                                 ;-- Enable core interrupts

  ;--- Never reach this

       b  .
    ;  bx lr




/* ********************************************************************************************** */
//              Switch OS Context
/* ********************************************************************************************** */

tn_switch_context
    
      ;--- set the context switch request

       ldr    r0, =tn_context_switch_request
       mov    r1, #1
       str    r1, [r0]

     ;---  Just Invoke PendSV exception

       ldr    r1, =ICSR_ADDR
       ldr    r0, =PENDSVSET
       str    r0, [r1]

       bx     lr



PendSV_Handler

       cpsid  I                         ;-- Disable core int

  ;--- The context switching - to do or not

       ldr    r0, =tn_context_switch_request
       ldr    r1, [r0]
       cmp    r1, #0                    ;-- if there is no context
       beq    exit_context_switch       ;-- switching - return
       mov    r1, #0
       str    r1, [r0]

  ;----------------------------------------

       mrs    r0, PSP                   ;-- in PSP - process(task) stack pointer
       stmdb  r0!, {r4-r11}

       ldr    r3, =tn_curr_run_task
       mov    r1,  r3
       ldr    r1, [r1]
       str    r0, [r1]                  ;-- save own SP in TCB

       ldr    r1, =tn_next_task_to_run
       ldr    r2, [r1]
       str    r2, [r3]                  ;-- in r3 - =tn_curr_run_task

       ldr    r0, [r2]                  ;-- in r0 - new task SP

       ldmia  r0!, {r4-r11}
       msr    PSP, r0
       orr    lr, lr, #0x04             ;-- Force to new process PSP

  ;----------------------------------------

exit_context_switch

       cpsie  I                         ;-- enable core int
       bx     lr







tn_int_exit

       ldr    r1, =ICSR_ADDR            ;-- Invoke PendSV exception
       ldr    r0, =PENDSVSET
       str    r0, [r1]

       bx     lr


/* ============================================================================================== */

       nop                             ; -- just for align
       END
