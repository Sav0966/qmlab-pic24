;; ===========================================================================
;;
;;  TNKernel RTOS port for Microchip PIC24/dsPIC
;;
;;  (c)2006, 2010, Alex Borisov
;;
;;  File:        port_af_cirq.s
;;
;;  Component:   PORT, assembler part
;;  Description: Internal routine for disabling interrupt checking
;;
;;  Compiler:    Microchip C30 v.2.xx - 3.11 (PIC24/dsPIC)
;;
;;  Ver. 2.5
;;
;;  Copyright © 2004,2008 Yuri Tiomkin
;;  Copyright © 2008 Alex Borisov (PIC24/dsPIC port)
;;
;;  All rights reserved.
;;
;;  Permission to use, copy, modify, and distribute this software in source and binary forms and its documentation
;;  for any purpose and without fee is hereby granted, provided that the above copyright notice appear in all copies
;;  and that both that copyright notice and this permission notice appear in supporting documentation.
;;
;;  THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
;;  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;;  ARE DISCLAIMED. IN NO EVENT SHALL YURI TIOMKIN OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
;;  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
;;  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
;;  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
;;  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;;
;; ===========================================================================

        .extern _tn_system_state
        .extern _tn_curr_run_task
        .extern _tn_next_task_to_run


;; Executing code section

        .section tnk_code, code

        .global _tn_is_irq_disabled
        .global _tn_enter_critical
        .global _tn_exit_critical
        .global _ffs_asm
        .global _tn_cpu_restore_sr
        .global _tn_stack_init
        .global _tn_cpu_save_sr
        .global _tn_start_exe
        .global _tn_switch_context_exit
        .global _tn_switch_context
    
.ifndef __HAS_EDS
.equ	_PSVPAG_, _PSVPAG
.else
.equ	_PSVPAG_, _DSRPAG
.endif


_tn_is_irq_disabled:

        mov     #0xE0,          W0              ; check for IPL2:IPL0
        and     _SR,            WREG

        btsc    _CORCON,        #3              ; check for IPL3
        bset    W0,             #0

        btsc    _INTCON2,       #14             ; check for DISI
        bset    W0,             #1

        return







_tn_enter_critical:

        mov     #_tn_sys_context,       W0
        cp0     [W0]
        bra     NZ,             1f              ; only in taks context

        bset    _SR,            #5
        nop
        nop
        bset    [W0],           #1

1:      return











_tn_exit_critical:

        mov     #_tn_sys_context,       W0
        btss    [W0],           #1
        return

        bclr    [W0],           #1
        bclr    _SR,            #5
        return








_ffs_asm:

        ff1r    W0,         W0
        return





_tn_cpu_save_sr:

        mov     _SR,    W0
        bset    _SR,    #5

        return



_tn_cpu_restore_sr:

        mov     W0,     _SR
        return










_tn_stack_init:

        ;; init locals

        mov     W1,     W4              ; W4 - top of stack
        mov     W1,     W5              ; W5 - top of stack

        add     #44,    W4              ; W4 - pointer to safe stack space (this need for exit() services)

        sl      W2,     W2              ; W2 - task size in bytes
        add     W5,     W2,     W5      ; W5 - pointer to end of stack
        mov     W5,     W6              ; W6 - pointer to end of stack

        ; Check if stack pointer located in initialized stack space

        cpsne   W15,    W1              ; if stack pointer equal to top of stack - go to remove current stack
        goto    _tn_stack_remove
        cpsgt   W15,    W1              ; if stack pointer higher then top of stack - if no - begin to init task stack
        goto    _tn_fill_fstack
        cpslt   W15,    W5              ; if stack pointer lower then end of stack - if no - begin to init task stack
        goto    _tn_fill_fstack

_tn_stack_remove:

        ;; remove current task stack to safe task stack space
        ;; --------------------------------------------------
        ;; W1  - top of stack
        ;; W2  - task size in bytes

        ;; W4  - stack safe space address
        ;; W5  - end of stack + 1
        ;; W15 - stack pointer

        ; calc of source address        W6
        dec2    W15,    W6

        ; calc of destination addr      W4
        sub     W15,    W1,     W2
        add     W2,     W4,     W4
        mov     W4,     W7

        ; calc num of words to copy in safe space
        sub     W6,     W1,     W2
        lsr     W2,     W2

        ; DISABLE INTERRUPT ?

        repeat  W2
        mov     [W6--], [W4--]          ; coping current task stack to safe stack space

        ; remove stack pointer
        inc2    W7,     W15             ; remove stack pointer

        ; ENABLE INTERRUPT ?

_tn_fill_fstack:

        ;; fill task stack -----------------------
        ;; ---------------------------------------
        ;; W0 - task function addres
        ;; W1 - top of stack
        ;; W5 - end of stack + 1 (if last stack add = 0x0010, W6 = 0x0011)

        mov     W1,             W2              ; W2 - top of task stack

        mov     W0,             [W2++]          ; Task function address init
        clr                     [W2++]

        dec2    W5,             [W2++]          ; SPLIM

        mov     #0x0103,        W0              ; SR
        mov     W0,             [W2++]
        mov     #0x1414,        W0              ; W14
        mov     W0,             [W2++]
        mov     #0x1212,        W0              ; W12
        mov     W0,             [W2++]
        mov     #0x1313,        W0              ; W13
        mov     W0,             [W2++]
        mov     #0x1010,        W0              ; W10
        mov     W0,             [W2++]
        mov     #0x1111,        W0              ; W11
        mov     W0,             [W2++]
        mov     #0x0808,        W0              ; W08
        mov     W0,             [W2++]
        mov     #0x0909,        W0              ; W09
        mov     W0,             [W2++]
        mov     #0x0606,        W0              ; W06
        mov     W0,             [W2++]
        mov     #0x0707,        W0              ; W07
        mov     W0,             [W2++]
        mov     #0x0404,        W0              ; W04
        mov     W0,             [W2++]
        mov     #0x0505,        W0              ; W05
        mov     W0,             [W2++]
        mov     #0x0202,        W0              ; W02
        mov     W0,             [W2++]
        mov     #0x0303,        W0              ; W03
        mov     W0,             [W2++]
        mov     W3,             [W2++]          ; W0 - task function param
        mov     #0x0101,        W0              ; W1
        mov     W0,             [W2++]
        mov     _RCOUNT,        W0
        mov     W0,             [W2++]
        mov     _TBLPAG,        W0
        mov     W0,             [W2++]
        mov     _CORCON,        W0
        mov     W0,             [W2++]
        mov     _PSVPAG_,       W0
        mov     W0,             [W2++]

        mov     W2,             W0              ; return last stack enter

        return
































_tn_start_exe:

        ; enable kernel run

        mov     #1,                     W1
        mov     #_tn_system_state,      W2
        mov     W1,                     [W2]    ; set system state as RUNNING

        ;clr     W0                              ; for correct enter in tn_switch_context_exit

_tn_switch_context_exit:

        ;mov     W0,             _SR             ; Restore saved SR

        ;mov     _SR,            W3              ; save SR state and disable system interrupts
        ;bset    _SR,            #5

        mov     #0xFFFF,        W1              ; Prevent stack error exeption if any non OS interrupt will be
        mov     W1,             _SPLIM

        mov     #_tn_curr_run_task,      W1     ; move current task stack pointer to W1
        mov     #_tn_next_task_to_run,   W2     ; move next task stack pointer to W2

        mov     [W2],           W2
        mov     [W2],           W15

        mov     W2,             [W1]            ; exchange stack pointers, run next task

        ;mov     W3,             _SR             ; restore SR state and enable system interrupts

        ;; Restore new task context

        pop     _PSVPAG_
        pop     _CORCON
        pop     _TBLPAG
        pop     _RCOUNT
        pop.d   W0
        pop.d   W2
        pop.d   W4
        pop.d   W6
        pop.d   W8
        pop.d   W10
        pop.d   W12
        pop     W14
        pop     _SR
        pop     _SPLIM

        retfie

















_tn_switch_context:

        ;; Save current context

        mov     _SR,            W0
        sl      W0,     #8,     W0
        btsc    _CORCON,        #3
        bset    W0,             #7

        ior     W0,     [--w15], W0
        mov     W0,     [w15++]


        push    _SPLIM
        push    _SR
        push    W14
        push.d  W12
        push.d  W10
        push.d  W8
        push.d  W6
        push.d  W4
        push.d  W2
        push.d  W0
        push    _RCOUNT
        push    _TBLPAG
        push    _CORCON
        push    _PSVPAG_

        mov     _SR,            W2                      ; save SR state and disable system interrupts
        bset    _SR,            #5


        mov     #0xFFFF,        W0                      ; Prevent stack error exeption if any non OS interrupt will be
        mov     W0,             _SPLIM

        ; next task

        mov     #_tn_curr_run_task,      W0             ; move current task stack pointer to W0
        mov     #_tn_next_task_to_run,   W1             ; move next task stack pointer to W1
        mov     [W1],           W1
        mov     [W0],           W0
        cp0     W1
        bra     Z,              __tn_sw_restore         ; restore old context, if there is no next task
        cp      W1,             W0
        bra     Z,              __tn_sw_restore         ; restore old context, if next task is current task

        mov     W15,            [W0]                    ; exchange stack pointer
        mov     [W1],           W15

        mov     #_tn_curr_run_task,      W0
        mov     W1,             [W0]

        mov     W2,             _SR                     ; restore SR state and enable system interrupts

__tn_sw_restore:

        ; Restore context on stack pointer

        pop     _PSVPAG_
        pop     _CORCON
        pop     _TBLPAG
        pop     _RCOUNT
        pop.d   W0
        pop.d   W2
        pop.d   W4
        pop.d   W6
        pop.d   W8
        pop.d   W10
        pop.d   W12
        pop     W14
        pop     _SR
        pop     _SPLIM

        retfie






;; =====================================================================================================================
        .end
