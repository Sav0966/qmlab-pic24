/*
*	Reference Clock Output module functions
*/
#ifndef _REFO_INCL_
#define _REFO_INCL_
#include "p24Fxxxx.h"

#define RO_SSLP	0x0 /* REFO is disabled in Sleep mode (def) */
#define RO_RSLP	0x20 /* REFO continues to run in Sleep mode */
#define RO_PRI	0x10 /* Primary oscillator used as input clock */
#define RO_SYS	0x0 /* System clock used as input clock (def) */

typedef enum tagREFODIV {
	RODIV_NONE, RODIV_2, RODIV_4, RODIV_8, RODIV_16, RODIV_32,
	RODIV_64, RODIV_128, RODIV_256, RODIV_512, RODIV_1024,
	RODIV_2048, RODIV_4096, RODIV_8192, RODIV_16384, RODIV_32768
} REFODIV; // REFO Divisor Select bits

/*
*	REFO module initialization, return 0 if success
*	Usage: if (refo_init(RO_RSLP|RO_PRI|RODIV_4)) error();
*/
int refo_init(int mask);

void refo_on(void); // REFO enabled on output pin
#define refo_on()	{ REFOCONbits.ROON = 1; }

void refo_off(void); // REFO disabled (no output)
#define refo_off()	{ REFOCONbits.ROON = 0; }

void refo_div(REFODIV val); // Set REFO Divisor value
#define refo_div(val) { REFOCONbits.RODIV = val; }

#endif /*_REFO_INCL_*/
