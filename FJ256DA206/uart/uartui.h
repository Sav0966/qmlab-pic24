/*
*	UART 1-4 user interface functions
*/
#ifndef _UARTUI_INCL_
#define _UARTUI_INCL_

#include <uarts.h>
/*
*	Template of UART Interrupt Service Routines
*/
#define _UART_INTFUNC(n, isr) /* isr - RX, TX and Err */\
__attribute__((__interrupt__, auto_psv)) _U##n##isr##Interrupt
#define UART_INTFUNC(n, isr) _UART_INTFUNC(n, isr)
/*
*	Templates of buffer counter functions: int func(void)
*/
#define _UBUF_COUNT(n, buf)		uart_##n##_##buf##_count
#define IMPL_UBUF_COUNT(n, buf)	int _UBUF_COUNT(n, buf)(void)
#define DECL_UBUF_COUNT(n, buf)	extern IMPL_UBUF_COUNT(n, buf)
#define _UBUF_FULL(n, buf)		uart_##n##_##buf##_full
#define IMPL_UBUF_FULL(n, buf)	int _UBUF_FULL(n, buf)(void)
#define DECL_UBUF_FULL(n, buf)	extern IMPL_UBUF_FULL(n, buf)
/*
*	Templates of buffer purging functions: void func(void)
*/
#define _UBUF_PURGE(n, buf)		uart_##n##_##buf##_purge
#define IMPL_UBUF_PURGE(n, buf)	void _UBUF_PURGE(n, buf)(void)
#define DECL_UBUF_PURGE(n, buf)	extern IMPL_UBUF_PURGE(n, buf)
/*
*	Template of user interface declaration
*/
#define DECL_UART_UI(n)\
DECL_UBUF_COUNT(n, TX); DECL_UBUF_COUNT(n, RX); DECL_UBUF_COUNT(n, ER);\
DECL_UBUF_FULL(n, TX); DECL_UBUF_FULL(n, RX); DECL_UBUF_PURGE(n, TX);\
DECL_UBUF_PURGE(n, RX); DECL_UART_WRITE(n); DECL_UART_READ(n);\
DECL_UART_GETC(n); DECL_UART_PUTC(n)
/*
*	User interface functions (n - ordinal number of the UART module)
*/
#define uart_tx_count(n)	_UBUF_COUNT(n, TX)() // TX buffer count
#define uart_rx_count(n)	_UBUF_COUNT(n, RX)() // RX buffer count
#define uart_er_count(n)	_UBUF_COUNT(n, ER)() // Number of errors 

#define uart_tx_full(n)		_UBUF_FULL(n, TX)() // TX buffer is full
#define uart_rx_full(n)		_UBUF_FULL(n, RX)() // RX buffer is full

#define uart_tx_empty(n)	(uart_tx_count(n) == 0)
#define uart_rx_empty(n)	(uart_rx_count(n) == 0)
#define uart_is_error(n)	(uart_er_count(n) != 0)

#define uart_tx_purge(n)	_UBUF_PURGE(n, TX)() // Purge TX buffer
#define uart_rx_purge(n)	_UBUF_PURGE(n, RX)() // Purge RX buffer
/*
* ~SHDN and ~INVALID lines of RS-232 Driver
*/
#define _UART_IS_VALID(n)	(_U##n##_VALID != 0)
#define _UART_IS_SHDN(n)	(_U##n##_SHDN == 0)
#define _UART_WAKEUP(n)		U##n##_SHDN_SET()
#define _UART_SHDN(n)		U##n##_SHDN_CLR()

#ifdef __MPLAB_SIM		// UARTs are valid for MPLAB SIM
#ifdef	_U1_VALID
#undef  _U1_VALID
#define _U1_VALID		1
#endif
#ifdef	_U2_VALID
#undef  _U2_VALID
#define _U2_VALID		1
#endif
#ifdef	_U3_VALID
#undef  _U3_VALID
#define _U3_VALID		1
#endif
#ifdef	_U4_VALID
#undef  _U4_VALID
#define _U4_VALID		1
#endif

#undef  _UART_IS_SHDN
#define _UART_IS_SHDN(n)	0 // UARTs are turned on
#endif //__MPLAB_SIM

#define UART_IS_VALID(n)	_UART_IS_VALID(n)
#define UART_IS_SHDN(n)		_UART_IS_SHDN(n)
#define UART_WAKEUP(n)		_UART_WAKEUP(n)
#define UART_SHDN(n)		_UART_SHDN(n)
/*
* UART Initialization
*
* n - UART number (1 - 4)
* mode - UART mode ([U_EN] | [U_PARITYx] | [U_RTSx]|[...])
* sta - UART status ([U_TXEN] | [U_TXIx]|[U_RXIx]|[...])
* brg - BRG register value (use FCY2BRG(FCY2, baud_rate))
* ipl - interrupt priority levels, if <= 0 - no unterrupt
*/
#define UART_INIT(n, mode, sta, brg, ipl) {\
	UART_DISABLE_TXINT(n); /* Disable UART interrupts */\
	UART_DISABLE_RXINT(n); UART_DISABLE_ERINT(n);\
\
	if (_UART_IS_VALID(n)) {/* Valid input levels */\
		_UMD(n) = 0;	/* Power on UART module */\
		_UART_WAKEUP(n); /* Wake-up RS-232 Driver */\
\
		/* Setup mode (UART disabled). Setup control */\
		/* bits, clear FIFO buffers and receiver errors */\
		UMODE(n) = (mode)&~UART_EN; USTA(n) = (sta)&~U_TXEN;\
		UBRG(n) = brg; /* Write appropriate baud rate value */\
\
		uart_rx_purge(n); uart_tx_purge(n); /* Clear buffers */\
		/* Clear all interrupt status flags (Rx, Tx and Error */\
		UART_CLR_RXFLAG(n); UART_CLR_TXFLAG(n); UART_CLR_ERFLAG(n); \
\
		if (ipl > 0) {\
			UART_SET_RXIPL(n, ipl); /* Receive IPL */\
			UART_SET_ERIPL(n, ipl); /* Set the same */\
			UART_ENABLE_ERINT(n); /* Enable interrupt */\
			UART_ENABLE_RXINT(n); /* Enable interrupt */\
\
			UART_SET_TXIPL(n, ipl); /* Transmit IPL */\
			UART_ENABLE_TXINT(n); /* Enable interrupt */\
		}\
\
		if ((mode) & UART_EN) { /* If it is set in 'mode' */\
			UMODEbits(n).UARTEN = 1;	/* Enable module */\
			/* The UTXEN bit should not be set until the */\
			/* UARTEN bit has been set; otherwise, UART */\
			/* transmissions will  not be enabled. */\
			/* Enable Transmitter if it's needed */\
			if ((sta) & U_TXEN) UART_ENABLE_TX(n);\
\
			/* Uart TX interrupt may be called */\
		}\
\
	} /* UART_IS_VALID() */\
} ((void)0)

#define UART_IS_INIT(n) /* Powered & Enabled */\
((_UMD(n) == 0) && (UMODEbits(n).UARTEN != 0))

#define UART_DONE(n)\
	UART_DISABLE_TXINT(n); /* Disable interrupts */\
	UART_DISABLE_RXINT(n); UART_DISABLE_ERINT(n);\
	UMODEbits(n).UARTEN = 0; /* Disable module */\
	/* Clear all interrupt status flags (Rx, Tx and Error */\
	UART_CLR_RXFLAG(n); UART_CLR_TXFLAG(n); UART_CLR_ERFLAG(n)

#define UART_PWOFF(n)\
	UART_DONE(n); /* Disable UART and interrupt */\
	_UART_SHDN(n); /* Shutdown RS-232 Driver */\
	_UMD(n) = 1 /* Power off UART module */

#endif /*_UARTUI_INCL_*/
