/*
*	MCU Osccillator functions
*/
#ifndef _OSC_INCL_
#define _OSC_INCL_
#include "p24Fxxxx.h"
/*
* Clock source switching function
*	VALUE - FRC, FRCPLL, FRCDIV,
*		PRI, PRIPLL, SOSC, LPRC
*/
void osc_switch(int FNOSC_VALUE);

#endif /*_OSC_INCL_*/
