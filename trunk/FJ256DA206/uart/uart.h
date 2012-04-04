/*
*	UART 1-4 management functions
*/
/*
*	UART 1-4 management functions
*/
#ifndef _UART_INCL_
#define _UART_INCL_
/*
* ~SHDN and ~INVALID lines of RS-232 Driver (if present)
*/
#define UART_IS_VALID(n)	!(U##n##_INVALID == 0)
#define UART_IS_SHDN(n)		(U##n##_SHDN == 0)
#define UART_WAKEUP(n)		U##n##_SHDN = 1
#define UART_SHDN(n)		U##n##_SHDN = 0

/* PMD register definitions */
#define PMDU1bits	PMD1bits
#define PMDU2bits	PMD1bits
#define PMDU3bits	PMD3bits
#define PMDU4bits	PMD4bits
/*
* Power management of UART module
*/
#define UART_PMD(n)		PMDU##n##bits.U##n##MD
#define UART_PWON(n)	UART_PMD(n) = 0
#define UART_PWOFF(n)	UART_PMD(n) = 1

#define UART_INIT(n)	{\
	if (UART_IS_VALID(n)) { /* Valid input levels */\
		UART_WAKEUP(n); /* Wake-up RS-232 Driver */\
		UART_PWON(n);   /* Power on UART module */\
	} else { /* RS-232 por isn't connected  to PC*/\
		UART_SHDN(n); /* Shutdown RS-232 Driver */\
		UART_PWOFF(n); /* Power off UART module */\
	}\
}

#endif /*_UART_INCL_*/
