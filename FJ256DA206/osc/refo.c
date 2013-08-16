/*
*	Reference Clock Output module functions
*/
#include <config.h>
#include <refo.h>

int refo_init(int mask)
{
#ifndef _REFO
	/* Select AN15/RP29/REFO/RB15 as output */
	__asm__ volatile (/* pin in HIGHT state */\
		"	bclr	ANSB, #15	; Disable analog\n"
//		"	bset	LATB, #15	; Set latch RB15\n"
		"	bclr	LATB, #15	; Clear latch RB15\n"
		"	bclr	TRISB, #15	; Enables output\n"
	); // It's not needed, but we set it by hand
#endif

	_REFOMD = 0; /* Turn on module power */
	REFOCON = mask << 8;  /* Setup mode */
	return(0);
}
