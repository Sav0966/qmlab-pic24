; ---------------------------------------------------------------------------
;
;    TNKernel startup hardware init for LPC176x processors
;
;    Assembler: ARMCC
;
; Copyright © 2004, 2009 Yuri Tiomkin
; All rights reserved.
;
;Permission to use, copy, modify, and distribute this software in source
;and binary forms and its documentation for any purpose and without fee
;is hereby granted, provided that the above copyright notice appear
;in all copies and that both that copyright notice and this permission
;notice appear in supporting documentation.
;
;THIS SOFTWARE IS PROVIDED BY THE YURI TIOMKIN AND CONTRIBUTORS ``AS IS'' AND
;ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;ARE DISCLAIMED. IN NO EVENT SHALL YURI TIOMKIN OR CONTRIBUTORS BE LIABLE
;FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
;DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
;OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
;HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
;LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
;OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
;SUCH DAMAGE.
;----------------------------------------------------------------------------- 

;--  ver 2.5


PCLKSEL0  equ   0x400FC1A8  ;-- PCLKSEL0

  ; -- Public functions declared in this file

     EXPORT  tn_startup_hardware_init
     EXPORT  tn_arm_disable_interrupts
     EXPORT  tn_arm_enable_interrupts

;-----------------------------------------------------------------------------

    AREA    HardwareInit, CODE, READONLY
    THUMB

; ----------------------------------------------------------------------------
;  This routine is called immediately after reset to setup hardware that is
; vital for processor's functionality (for instance,SDRAM controller setup,
; PLL setup,etc.)
;  It is assumed that other hardware's init routine(s) will be invoked later
; by C-language function call.
; ----------------------------------------------------------------------------

tn_startup_hardware_init

    ;-- PCLKSEL0 - UART0, SPI, Timer 1 (MAT1.1), PWM1 (PWM1.3)

     ldr    r1, =PCLKSEL0
     ldr    r0, [r1]
     orr    r0, r0, #0x01000000
     orr    r0, r0, #(1<<6)   ;-- UART0   - Bits 7:6   = 01;  PCLK_peripheral = CCLK
     orr    r0, r0, #(1<<16)  ;-- SPI     - Bits 17:16 = 01;  PCLK_peripheral = CCLK
     orr    r0, r0, #(1<<4)   ;-- Timer 1 - Bits 5:4   = 01;  PCLK_peripheral = CCLK
     orr    r0, r0, #(1<<12)  ;-- PWM1    - Bits 13:12 = 01;  PCLK_peripheral = CCLK
     str    r0, [r1]

     bx      lr

; -------------------------------------------------------------------------
tn_arm_disable_interrupts

     cpsid  I
     bx     lr


; -------------------------------------------------------------------------
tn_arm_enable_interrupts

     cpsie I
     bx   lr


     END

; -------------------------------------------------------------------------
; -------------------------------------------------------------------------
; -------------------------------------------------------------------------
; -------------------------------------------------------------------------





