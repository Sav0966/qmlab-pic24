/*
*	MCU Configuration Words
*/
#include "config.h"
#if (defined(__PIC24FK__)) /* PIC24FJ only */
#error -- For PIC24FJ256DA210 Family only --
#endif

_CONFIG3(CONFIG3_INIT);

// Select PLLDIV value from FOSC
#if (FOSC > 47990000L) // 48 MHz
 #define PLLDIV_VALUE PLLDIV_DIV12
#elif (FOSC > 31990000L)  // 32 MHz
 #define PLLDIV_VALUE	PLLDIV_DIV8
#elif (FOSC > 23990000L)  // 24 MHz
 #define PLLDIV_VALUE	PLLDIV_DIV6
#elif (FOSC > 19990000L)  // 20 MHz
 #define PLLDIV_VALUE	PLLDIV_DIV5
#elif (FOSC > 15990000L)  // 16 MHz
 #define PLLDIV_VALUE	PLLDIV_DIV4
#elif (FOSC > 11990000L)  // 12 MHz
 #define PLLDIV_VALUE	PLLDIV_DIV3
#elif (FOSC > 7990000L) //  8 MHz
 #define PLLDIV_VALUE PLLDIV_DIV2
#else // 4 MHz PLL96MHZ clock input
 #define PLLDIV_VALUE	PLLDIV_NODIV
#endif

// Reset IESO bit (Errata DS80505)
// Use OSCO/RC15 pin as digital IO to turn on/off
//  external ocsillator (low -> on), wich is connected to OSCI
// Manage PLL96MHZ by software (PLLEN bit), reset state -> off
// Enable primary oscillator and select External-Clock mode
// Clock switching is enabled, fail-safe monitor - disabled
_CONFIG2(CONFIG2_INIT & IESO_OFF & OSCIOFNC_ON & POSCMOD_EC\
	& PLL96MHZ_OFF & PLLDIV_VALUE & FCKSM_CSECMD);

_CONFIG1(CONFIG1_INIT);
