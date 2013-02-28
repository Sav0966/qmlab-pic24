/*
*	UART 1-4 user interface functions
*/
#ifndef _UARTUI_INCL_
#define _UARTUI_INCL_

#include <uart.h>
/*
*	Template of UART Interrupt Service Routines
*/
#define _UART_INTFUNC(n, isr) /* isr - RX, TX and Err */\
__attribute__((__interrupt__, no_auto_psv)) _U##n##isr##Interrupt
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
*	Templates of writing and reading functions
*/
#define _UART_WRITE(n)		uart_##n##_write
#define _UART_READ(n)		uart_##n##_read
#define IMPL_UART_WRITE(n)	int _UART_WRITE(n)(const char* buf, int len)
#define IMPL_UART_READ(n)	int _UART_READ(n)(char* buf, int len)
#define DECL_UART_WRITE(n)	extern IMPL_UART_WRITE(n)
#define DECL_UART_READ(n)	extern IMPL_UART_READ(n)
/*
*	Template of user interface declaration
*/
#define DECL_UART_UI(n)\
DECL_UBUF_COUNT(n, TX);\
DECL_UBUF_COUNT(n, RX);\
DECL_UBUF_COUNT(n, ER);\
DECL_UBUF_FULL(n, TX);\
DECL_UBUF_FULL(n, RX);\
DECL_UBUF_PURGE(n, TX);\
DECL_UBUF_PURGE(n, RX);\
DECL_UART_WRITE(n);\
DECL_UART_READ(n)
/*
*	User interface functions (n - number of UART module)
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

#define uart_write(n, buf, len)		_UART_WRITE(n)(buf, len)
#define uart_read(n, buf, len)		_UART_READ(n)(buf, len)
/*
* UART Initialization
*
* n - UART number (1 - 4)
* mode - UART mode ([U_EN] | [U_PARITYx] | [U_RTSx]|[...])
* sta - UART status ([U_TXEN] | [U_TXIx]|[U_RXIx]|[...])
* brg - BRG register value (use FCY2BRG(FCY2, baud_rate))
* ipl - interrupt priority levels, if <= 0 - no unterrupt
*/
#define UART_INIT(n, mode, sta, brg, rx_ipl, tx_ipl, er_ipl) {\
	UART_DISABLE_RXINT(n); /* Disable UART interrupts */\
	UART_DISABLE_TXINT(n); UART_DISABLE_ERINT(n);\
\
	if (UART_IS_VALID(n)) { /* Valid input levels */\
		_UMD(n) = 0;	/* Power on UART module */\
		UART_WAKEUP(n); /* Wake-up RS-232 Driver */\
\
		/* Setup mode (UART disabled). Setup control */\
		/* bits, clear FIFO buffers and receiver errors */\
		UMODE(n) = (mode) & ~U_EN; USTA(n) = (sta) & ~U_TXEN;\
		UBRG(n) = brg; /* Write appropriate baud rate value */\
		/* Clear all interrupt status flags (Rx, Tx and Error */\
		UART_CLR_RXFLAG(n); UART_CLR_TXFLAG(n); UART_CLR_ERFLAG(n); \
\
		if (rx_ipl > 0) {\
			UART_SET_RX_IPL(n, rx_ipl); /* Receive IPL */\
			UART_ENABLE_RXINT(n); /* Enable interrupt */\
		}\
\
		if (tx_ipl > 0) {\
			UART_SET_TX_IPL(n, tx_ipl); /* Transmit IPL */\
			UART_ENABLE_TXINT(n); /* Enable interrupt */\
		}\
\
		if (er_ipl > 0) {\
			UART_SET_ER_IPL(n, er_ipl); /* Error IPL */\
			UART_ENABLE_ERINT(n); /* Enable interrupt */\
		}\
\
		if ((mode) & U_EN) { /* If it is set in 'mode' */\
			UMODEbits(n).UARTEN = 1; /* Enable module */\
			/* The UTXEN bit should not be set until the */\
			/* UARTEN bit has been set; otherwise, UART */\
			/* transmissions will  not be enabled. */\
			/* Enable Transmitter if it's needed */\
			if ((sta) & U_TXEN) UART_ENABLE_TX(n);\
		}\
\
	} else { /* The RS-232 port isn't connected */\
		UART_SHDN(n); /* Shutdown RS-232 Driver */\
		_UMD(n) = 1; /* Power off UART module */\
	} /* UART_IS_VALID() */\
}

#define UART_IS_INIT(n) /* Powered & Enabled */\
((_UMD(n) == 0) && (UMODEbits(n).UARTEN != 0))

#endif /*_UARTUI_INCL_*/