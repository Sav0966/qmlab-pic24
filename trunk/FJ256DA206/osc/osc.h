/*
*	MCU Osccillator functions
*/
#ifndef _OSC_INCL_
#define _OSC_INCL_
#include "p24Fxxxx.h"

typedef enum tagOSC_MODE { // Valid mode selection values
	FRC, FRCPLL, PRI, PRIPLL, SOSC, LPRC, FRC16, FRCDIV
} OSC_MODE;

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
