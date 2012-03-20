/*
* Definition of FCY for selected oscillator
* Define valid __OSC__ value to obtain FCY
*/
#ifndef _FCY_INCL_
#define _FCY_INCL_
#include <p24Fxxxx.h> /* PIC24F only */

#ifndef __OSC__
 #error Define __OSC__ simbol first
#endif /*__OSC__*/

#include "oscdef.h"

#if (__OSC__ == FRC)
 #define FCY  FCY_FRC
#elif (__OSC__ == FRCPLL)
 #define FCY  FCY_FRCPLL
#elif (__OSC__ == PRI)
 #define FCY  FCY_PRI
#elif (__OSC__ == PRIPLL)
 #define FCY  FCY_PRIPLL
#elif (__OSC__ == SOSC)
 #define FCY  FCY_SOSC
#elif (__OSC__ == LPRC)
 #define FCY  FCY_LPRC
#elif (__OSC__ == FRC16)
 #define FCY  FCY_FRC16
#elif (__OSC__ == FRCDIV)
 #define FCY  FCY_FRCDIV
#else
 #error Invalid value of __OSC__
#endif

#define FCY2	(FCY/2)

#endif /*_FCY_INCL_*/
