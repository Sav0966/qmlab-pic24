/*
*	MCU Osccillator functions
*/
#ifndef _OSC_INCL_
#define _OSC_INCL_
#include "p24Fxxxx.h"
/*
* Clock source switching functions
*/
typedef enum tagFNOSC_VALUE { // Valid selection values
	FRC, FRCPLL, PRI, PRIPLL, SOSC, LPRC, FRC16, FRCDIV
} FNOSC_VALUE;
void _osc_switch(FNOSC_VALUE mode); // in osc_.asm
int osc_mode(FNOSC_VALUE mode);
/*
* Turn on/off external oscillator
* Return 0 in success
*/
int osc_ec_on(int turn_on);
/* If OSCO/RC15 is not output in low state - off */
#define IS_OSC_EC_OFF()	((TRISC | LATC) & 0x8000)
#define IS_OSC_EC_ON()	(!IS_OSC_EC_OFF())

#endif /*_OSC_INCL_*/
