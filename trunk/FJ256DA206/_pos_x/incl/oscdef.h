/*
* Definitions of FCY values in different oscillator modes
*/
#ifndef _OSCDEF_INCL_
#define _OSCDEF_INCL_
#include <p24Fxxxx.h> /* PIC24F only */
/*
* Valid values of oscillator mode
*/
#define FRC		0
#define FRCPLL	1
#define PRI		2
#define PRIPLL	3
#define SOSC	4
#define LPRC	5
#define FRC16	6
#define FRCDIV	7

#ifndef FOSC
#include <fosc.h>	/* Main oscillator frequency FOSC */
#endif

#define FCY_PRI		((unsigned long)FOSC)
/*
* Defaul values for RC, LPRC and Secondary oscillators
*/
#ifndef FCY_FRC
#define FCY_FRC		8000000UL
#endif

#ifndef FCY_LPRC
#define FCY_LPRC	31000UL
#endif

#ifndef FCY_SOSC
#define FCY_SOSC	32768UL
#endif
/*
* Define FCY_PRIPLL for CPDIV = 0
* and select PLLDIV value from FOSC
*/
#if (FOSC > 47990000L) // 48 MHz
 #define FCY_RPIPLL		((2*FCY_PRI)/3UL)
 #define PLLDIV_MASK	PLLDIV_DIV12
 #define PLLDIV_N		12
#elif (FOSC > 31990000L)  // 32 to 48 MHz
 #define FCY_PRIPLL		FCY_PRI
 #define PLLDIV_MASK	PLLDIV_DIV8
 #define PLLDIV_N		8
#elif (FOSC > 23990000L)  // 24 to 32 MHz
 #define FCY_PRIPLL		((4*FCY_PRI)/3UL)
 #define PLLDIV_MASK	PLLDIV_DIV6
 #define PLLDIV_N		6
#elif (FOSC > 19990000L)  // 20 to 24 MHz
 #define FCY_PRIPLL		((8*FCY_PRI)/5UL)
 #define PLLDIV_MASK	PLLDIV_DIV5
 #define PLLDIV_N		5
#elif (FOSC > 15990000L)  // 16 to 20 MHz
 #define FCY_PRIPLL		(2UL*FCY_PRI)
 #define PLLDIV_MASK	PLLDIV_DIV4
 #define PLLDIV_N		4
#elif (FOSC > 11990000L)  // 12 to 16 MHz
 #define FCY_PRIPLL		((8*FCY_PRI)/3UL)
 #define PLLDIV_MASK	PLLDIV_DIV3
 #define PLLDIV_N		3
#elif (FOSC > 7990000L)   //  8 to 12 MHz
 #define FCY_PRIPLL		(4UL*FCY_PRI)
 #define PLLDIV_MASK	PLLDIV_DIV2
 #define PLLDIV_N		2
#else // For 4 MHz PLL96MHZ clock input
 #define FCY_PRIPLL		(8UL*FCY_PRI)
 #define PLLDIV_MASK	PLLDIV_NODIV
 #define PLLDIV_N		1
#endif
/*
* FCY for different modes of RC oscillator
*/
#define FCY_FRC16	(FCY_FRC/16UL)
/* Define it for RCDIV = 2 only */
#define FCY_FRCDIV	(FCY_FRC/2UL)
/* Use {RCDIV, PLLDIV} = {2, 1} or {1, 2} only */
#define FCY_FRCPLL	((8UL*FCY_FRCDIV)/PLLDIV_N)
/*
* Default unprogrammed clock
*/
#define FCY_UP2		(FCY_FRCDIV/2)

#endif /*_OSCDEF_INCL_*/
