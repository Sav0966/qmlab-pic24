/*
*	MCU Osccillator functions
*/
#include "p24Fxxxx.h"
#include "config.h"
#include "mcu_id.h"
#include "osc.h"

static const unsigned long arFCY[] =
{	FCY_FRC, FCY_FRCPLL, FCY_PRI, FCY_PRIPLL,
	FCY_SOSC, FCY_LPRC, FCY_FRC16, FCY_FRCDIV };

unsigned long get_fcy(OSC_MODE mode) { return(arFCY[mode]); }

#define MS(fcy) ((unsigned int)(fcy/2000UL))

static const unsigned int const ar_ms[] =
{	MS(FCY_FRC), MS(FCY_FRCPLL), MS(FCY_PRI), MS(FCY_PRIPLL),
	MS(FCY_SOSC), MS(FCY_LPRC), MS(FCY_FRC16), MS(FCY_FRCDIV) };

void delay_ms(int ms)
{
	unsigned long cur_ms = ar_ms[GET_OSC_MODE()];
	while (ms--) __delay32(cur_ms);
}

int osc_ec_on(int turn_on)
{
	int iRet = -1;

	for(;;)
	{
		int cfg = MCU_CONFIG2;
		if (cfg & ~OSCIOFNC_ON) break; // Must be IO
		
		/* OSCO/CLKO/RC15 is used as output pin for */
		if (turn_on) /* power managing of oscillator */
		{
			/* Turn on external oscillator FOX924B: */
			if (IS_OSC_EC_OFF()) { // delay once only
				__asm__ volatile (/* OSCO/CLKO/RC15 pin */\
				"	bclr	LATC, #15	; Clear latch RC15\n"
				"	bclr	TRISC, #15	; Enables output\n"
				); // Set OSCO pin as output in low state
				delay_ms(5);// Wait FOX924 startup time 5ms
			} // else -> oscillator already is turned on
		}
		else
		{
			// Get current oscillator mode
			OSC_MODE mode = GET_OSC_MODE();

			// Can't turn off if EC or ECPLL mode is used
			if (!(mode & 4)) // For PRIPLL, PRI, FRCPLL, FRC
				if (mode & 2) // For PRIPLL, PRI in EC mode
					if (!(cfg & ~POSCMOD_EC)) break;

			/* Turn off external oscillator FOX924B: */
			__asm__ volatile (/* OSCO/CLKO/RC15 pin */\
			"	bset	LATC, #15	; Set latch RC15\n"
			"	bclr	TRISC, #15	; Enables output\n"
			); // Set OSCO pin as output in hight state
		}

		iRet = 0; break; // OK
	} // for(;;)

	return(iRet);
}

int osc_pll_on(int turn_on)
{
	int iRet = 0;

	if (turn_on) CLKDIVbits.PLLEN = 1; // Enable PLL96MHZ
	else for(;;)
	{
		// Get current oscillator mode
		OSC_MODE mode = GET_OSC_MODE();
		iRet = -1; // May be we can't do it

		// Can't turn off PPL if PRIPLL or FRCPLL is used
		if (!(mode & 4)) // For PRIPLL, PRI, FRCPLL, FRC
			if (mode & 1) break; // For PRIPLL, FRCPLL

		// Don't turn off PPL for USB or Graphics controller
		if (U1CONbits.HOSTEN) break; //  USB host is enabled
		if (U1CONbits.USBEN) break; // USB device is enabled
		if (U1OTGCONbits.OTGEN) break; // USB OTG is enabled
		if (G1CON1bits.G1EN) break; // Use Graphics controller

		CLKDIVbits.PLLEN = 0; // Disable PLL96MHZ
		iRet = 0;  break;	 // OK
	} // else for(;;)

	return(iRet);
}

/* Wait OSWEN bit of OSCCON register for hardware only */
#if (!defined(__DEBUG) || defined(__MPLAB_DEBUGGER_ICD2))
 #define WAIT_OSWEN() while (OSCCONbits.OSWEN != 0)
#else // For MPLAB SIM and other tools don't wait
 #define WAIT_OSWEN()
#endif

int osc_mode(OSC_MODE mode)
{
	int iRet = -1;

	for(;;)
	{
		int cfg = MCU_CONFIG2;
		if (cfg & 0x80) break; // FCKSM<1> must be 0
		// Do it only if mode switching is enabled

		if (!(mode & 4))
		{ // For PRIPLL, PRI, FRCPLL, FRC only
			if (mode & 2) // For PRIPLL, PRI in EC mode
				if (!(cfg & ~POSCMOD_EC)) // Turn on EC
					if (osc_ec_on(1)) break; // or error

			if (mode & 1)
			{ // For PRIPLL, FRCPLL
				OSC_MODE cur_mode = GET_OSC_MODE();	// Get current mode
				if (!(cur_mode & 4)) if (cur_mode & 1) if (cur_mode != mode)
				{ // Must switch through FRC between two different PLL modes
						_osc_switch(FRC); WAIT_OSWEN(); /* Hardware wait */ }

				CLKDIVbits.PLLEN = 1; // Enable PLL96MHZ
			}
		}

		_osc_switch(mode); // Select needed mode
		/*
		* Do something, that is not clock-sensitive
		*/
		WAIT_OSWEN(); // Hardware wait while OSWEN != 0

//		Do it directly in power management procedure:
//		osc_pll_on(0); // Try to turn off PLL96MHz
//		osc_ec_on(0); // Try to turn off FOX924

		iRet = 0; break; // OK
	} // for(;;)

	return(iRet);
}
