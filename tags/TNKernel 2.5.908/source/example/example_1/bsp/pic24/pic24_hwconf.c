
#include "bsp.h"            /* BSP      */

_CONFIG2(IESO_OFF & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS);
#if defined(__DEBUG)
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_ON & COE_OFF & ICS_PGx2 & FWDTEN_OFF);
#else
_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & BKBUG_OFF & COE_OFF & ICS_PGx2 & FWDTEN_OFF);
#endif

#define _HALT()  __asm__ volatile(".pword 0xDA4000")

void __attribute__((interrupt, no_auto_psv)) _AddressError (void)
{
    _HALT();
    for (;;);
}

void __attribute__((interrupt, no_auto_psv)) _StackError (void)
{
    _HALT();
    for (;;);
}

void __attribute__((interrupt, no_auto_psv)) _MathError (void)
{
    _HALT();
    for (;;);
}

void __attribute__((interrupt, no_auto_psv)) _OscillatorFail (void)
{
    _HALT();
    for (;;);
}
