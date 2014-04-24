#include <p24Fxxxx.h> /* PIC24F only */
#include <osc.h>
#include "trap.h"

extern unsigned int // For trap handlers
__attribute__((persistent)) __reset_state;

#define __do_reset(uParam)		\
{	__reset_state = uParam;		\
	__asm__ volatile ("reset"); } ((void)0)

void __attribute__((__interrupt__, no_auto_psv))
_OscillatorFail(void)
{
	// Now we have FRC oscillator mode
	osc_mode(OSCCON >> 8); // Try to restore NOSC mode
	INTCON1bits.OSCFAIL = 0; // Clear interrupt flag
}

void __attribute__((__interrupt__, no_auto_psv))
_AddressError(void)
{
	INTCON1bits.ADDRERR = 0;
	__do_reset(0); // Reset
}

void __attribute__((__interrupt__, no_auto_psv))
_StackError(void)
{
	INTCON1bits.STKERR = 0;
	__do_reset(0); // Reset
}

void __attribute__((__interrupt__, no_auto_psv))
_MathError(void)
{
	INTCON1bits.MATHERR = 0;
	__do_reset(0); // Reset
}
