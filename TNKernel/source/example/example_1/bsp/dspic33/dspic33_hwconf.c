
#include "bsp.h"            /* BSP      */


_FBS(RBS_NO_BOOT_RAM & BSS_NO_BOOT_CODE & BWRP_WRPROTECT_OFF);
_FSS(RSS_NO_SEC_RAM & SSS_NO_SEC_CODE & SWRP_WRPROTECT_OFF);
_FGS(GSS_OFF & GCP_OFF & GWRP_OFF);
_FOSCSEL(FNOSC_PRIPLL & IESO_OFF);
_FOSC(FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMD_HS);
_FWDT(FWDTEN_OFF);
_FPOR(FPWRT_PWR128);
_FICD(JTAGEN_OFF & ICS_PGD1);


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
