/*
*	MCU Osccillator functions
*/
#ifndef _OSC_INCL_
#define _OSC_INCL_
#include "p24Fxxxx.h"

#ifndef FOSC
#include "fosc.h"	/* Main oscillator frequency */
#endif

typedef enum tagOSC_MODE { // Valid mode selection values
	FRC, FRCPLL, PRI, PRIPLL, SOSC, LPRC, FRC16, FRCDIV
} OSC_MODE;
/*
* Definitions of FCY value in different modes
*/
#define FCY_FRC		8000000UL
#define FCY_FRC16	(FCY_FRC/16UL)
/* Define it for RCDIV = 2 only */
#define FCY_FRCDIV	(FCY_FRC/2UL)
#define FCY_FRCPLL	(8UL*FCY_FRCDIV)

#define FCY_LPRC	31000UL
#define FCY_SOSC	32768UL

#define FCY_PRI	((unsigned long)FOSC)

#if (FOSC > 47990000L) // 48 to 32 MHz
 #define FCY_RPIPLL		((2*FCY_PRI)/3UL)
#elif (FOSC > 31990000L)  // 32 to 32 MHz
 #define FCY_PRIPLL		FCY_PRI
#elif (FOSC > 23990000L)  // 24 to 32 MHz
 #define FCY_PRIPLL		((4*FCY_PRI)/3UL)
#elif (FOSC > 19990000L)  // 20 to 32 MHz
 #define FCY_PRIPLL		((8*FCY_PRI)/5UL)
#elif (FOSC > 15990000L)  // 16 to 32 MHz
 #define FCY_PRIPLL		(2UL*FCY_PRI)
#elif (FOSC > 11990000L)  // 12 to 32 MHz
 #define FCY_PRIPLL		((8*FCY_PRI)/3UL)
#elif (FOSC > 7990000L)   //  8 to 32 MHz
 #define FCY_PRIPLL		(4UL*FCY_PRI)
#else // For 4 MHz PLL96MHZ clock input
 #define FCY_PRIPLL		(8UL*FCY_PRI)
#endif

unsigned long get_fcy(OSC_MODE mode);
/*
* Clock source switching functions
*/
void _osc_switch(OSC_MODE mode); // in osc_.asm
int osc_mode(OSC_MODE mode); // Set new clock mode
#define GET_OSC_MODE()	((OSC_MODE)(OSCCON >> 12))
#define GET_FCY()	get_fcy(GET_OSC_MODE())
/*
* Turn on/off external oscillator, return 0 if success
*/
int osc_ec_on(int turn_on);
/* If OSCO/RC15 is not output in low state - off */
#define IS_OSC_EC_OFF()	((TRISC | LATC) & 0x8000)
#define IS_OSC_EC_ON()	(!IS_OSC_EC_OFF())
/*
* Turn on/off PLL96MHZ (CLKDIVbits.PLLEN bit only)
* To prevent PLL turning set PLL96MHZ_ON in CONFIG2
* Return 0 if success
*/
int osc_pll_on(int turn_on);
/*
*	Frequency undepended delay
*/
void delay_ms(int ms);

#endif /*_OSC_INCL_*/
