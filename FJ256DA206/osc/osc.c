/*
*	MCU Osccillator functions
*/
#include "p24Fxxxx.h"
#include "mcu_id.h"
#include "osc.h"

#ifndef FCY_UP
#define FCY_UP	2000000L /* Default unprogrammed FCY/2 */
#endif

int osc_ec_on(int turn_on)
{
	if (!(MCU_CONFIG2 & ~OSCIOFNC_ON)) // Must be IO
	{	/* OSCO/CLKO/RC15 is used as output pin for */
		if (turn_on) /* power managing of oscillator */
		{
			if (IS_OSC_EC_OFF()) { // delay once only
				/* Turn on external oscillator FOX924B: */
				__asm__ volatile (/* OSCO/CLKO/RC15 pin */\
				"	bclr	LATC, #15	; Clear latch RC15\n"
				"	bclr	TRISC, #15	; Enables output\n"
				); // Set OSCO pin as output in low state
				__delay32(// Wait FOX924 startup time - 5ms
					(unsigned long)((5)*(FCY_UP)/1000ULL));
			} // else -> oscillator already is turned on
		}
		else
		{
			/* Turn off external oscillator FOX924B: */
			__asm__ volatile (/* OSCO/CLKO/RC15 pin */\
			"	bset	LATC, #15	; Set latch RC15\n"
			"	bclr	TRISC, #15	; Enables output\n"
			); // Set OSCO pin as output in hight state
		}
	}

	return(TRISC & 0x8000); // Must be 0 (output pin)
}

int osc_mode(FNOSC_VALUE mode)
{
	int iRet = -1, cfg = MCU_CONFIG2;

	for(;;)
	{
		if (cfg & 0x80) break; // FCKSM<1> must be 0
		// Do it only if mode switching is enabled
		if (!(mode & 4))
		{ // For PRIPLL, PRI, FRCPLL, FRC only
			// For PRIPLL, FRCPLL enable PLL96MHZ
			if (mode & 1) CLKDIVbits.PLLEN = 1;

			if (mode & 2) // For PRIPLL, PRI and EC mode
				if (!(cfg & ~POSCMOD_EC)) // Turn on EC
					if (osc_ec_on(1)) break; // or error
		}

		iRet = 0; break; // OK
	}

	return(iRet);
}
