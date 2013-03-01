
#include "bsp.h"            /* BSP      */

#pragma config FNOSC    = PRIPLL
#pragma config POSCMOD  = HS

#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL  = MUL_20
#pragma config FPLLODIV = DIV_1

#pragma config FCKSM    = CSDCMD
#pragma config IESO     = OFF

#pragma config OSCIOFNC = OFF
#pragma config FSOSCEN  = OFF

#pragma config FPBDIV   = DIV_2


#pragma config FWDTEN   = OFF
#pragma config WDTPS    = PS1048576


#pragma config CP       = OFF
#pragma config PWP      = OFF
#pragma config BWP      = OFF


#pragma config ICESEL   = ICS_PGx2
#if defined(__DEBUG)
#pragma config DEBUG    = ON
#else
#pragma config DEBUG    = OFF
#endif



typedef enum {
        EXCEP_IRQ        = 0,   // interrupt
        EXCEP_ADDR_ERR_L = 4,   // address error exception (load or ifetch)
        EXCEP_ADDR_ERR_S,       // address error exception (store)
        EXCEP_BUS_ERR_F,        // bus error (ifetch)
        EXCEP_BUS_ERR_LS,       // bus error (load/store)
        EXCEP_SYSCALL,          // syscall
        EXCEP_BREAKPOINT,       // breakpoint
        EXCEP_RES_OPCODE,       // reserved instruction
        EXCEP_CP_UNUSE,         // coprocessor unusable
        EXCEP_OVERFLOW,         // arithmetic overflow
        EXCEP_TRAP,             // trap (possible divide by zero)
        EXCEP_IS1        = 16,  // implementation specfic 1
        EXCEP_CEU,              // CorExtend Unuseable
        EXCEP_C2E               // coprocessor 2
} _EXCEP_CODE;

volatile _EXCEP_CODE _excep_code;
volatile void       *_excep_addr;    

void _general_exception_handler (void)
{
    _excep_code = (__builtin_mfc0(13, 0) & 0x0000007C) >> 2;
    _excep_addr = (void*)__builtin_mfc0(14, 0);
    
    //for (;;);
}

