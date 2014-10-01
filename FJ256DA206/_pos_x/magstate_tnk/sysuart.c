#include <p24Fxxxx.h>
#include <config.h>
#include <buffer.h>
#include <uarts.h>

#include "sysuart.h"

#define UART_USED			SYS_UART

#define UART_RXBUF_SIZE		SYS_UART_RXBUF_SIZE
#define UART_TXBUF_SIZE		SYS_UART_TXBUF_SIZE

#define _DISABLE_()			DISABLE()
#define _ENABLE_()			ENABLE()

#ifndef DISPATCH
#define DISPATCH()
#endif

// Receiver and transmitter queues and error flag
static volatile unsigned char QUEBUF(RXB, UART_RXBUF_SIZE);
static volatile unsigned char QUEBUF(TXB, UART_TXBUF_SIZE);
static volatile int U_(UART_USED, rxerr) /* = 0 */;
static volatile int U_(UART_USED, txevt) /* = 0 */;

int sysu_error(void) { return( U_(UART_USED, rxerr) ); }

int sysu_txevt(void) { return( U_(UART_USED, txevt) ); }

int sysu_is_init(void) { return( UART_IS_INIT(UART_USED) ); }

void sysu_done(void) { UART_PWOFF(UART_USED); }

int sysu_init(void)
{
	sysu_done(); // Disable all
	QUEBUF_INIT(RXB); QUEBUF_INIT(TXB);
	U_(UART_USED, rxerr) = 0;

	UART_INIT(UART_USED, // Try to initialize UART

#if (defined(__MPLAB_SIM) && (UART_USED == 2))
/* =!= It works with UART2 too if set LPBACK here */	U_LPBACK |
#endif // SIM supports UART1 only (SIM: UART1 IO must be enabled)

		U_NOPARITY | UART_EN,			// 8-bit, no parity; Enabled
		U_TXI_READY | U_RXI_ANY |		// Defaul event settings
		U_TXEN, FCY2BRG(FCY2, 9600),	// TX Enabled; 9600 baud
		SYS_UART_IPL					// All interrupts are enabled
	); // UART_INIT()

	return( sysu_is_init() );
}
//
// Transmitter section
//
int sysu_txsize(void) { return( QUEBUF_SIZE(TXB) ); }

int sysu_txcount(void) { return( QUEBUF_LEN(TXB) ); }

void sysu_txpurge(void)
{ // Reset TX queue and TX FIFO
	_DISABLE_();
	{
		QUEBUF_INIT(TXB); // Clear buffer
		if (UART_IS_ENABLE_TX(UART_USED)) { // Clear FIFO
			UART_DISABLE_TX(UART_USED); // Clear FIFO by TXEN = 0
			UART_ENABLE_TX(UART_USED); } // Then restore TXEN
	}
	_ENABLE_();
}

static int _sysu_putc(const int c)
{
	int i = -1;
	_DISABLE_();
	{
		if (!QUEBUF_FULL(TXB))
			_QUEBUF_PUSH(TXB, i = (unsigned char)c);
	}
	_ENABLE_();
	return( i );
}

int sysu_putc(const int c)
{
	int i = _sysu_putc(c);
	if (i != -1) UART_SET_TXFLAG(UART_USED);
	return( i );
}

int sysu_write(const char* buf, int len)
{
	int n;

	for (n = 0; n != len; n++)
		if (_sysu_putc((unsigned char)*buf++) == -1) break;

	if (n != 0) UART_SET_TXFLAG(UART_USED);

	return( n );
}

// Transmitter Interrupt Service Routine
void UART_INTFUNC(UART_USED, TX, no_auto_psv)(void)
{
	register int i;
	// Clear Interrupt flag
	UART_CLR_TXFLAG(UART_USED);

	switch (QUEBUF_LEN(TXB)) {
		case 0:  ++U_(UART_USED, txevt);
				 i = U_TXI_END; break;
		case 1:  i = U_TXI_READY; break;
		default: i = U_TXI_EMPTY; // We'll fill FIFO
	} UART_SET_TXI(UART_USED, i);

	while (!QUEBUF_EMPTY(TXB))
	{ // Load TX queue and fill TX FIFO
		if (UART_CAN_WRITE(UART_USED)) {
			_QUEBUF_POP(TXB, i);
			UART_WRITE(UART_USED, i);
		} else break; // FIFO is full
	}

	if (QUEBUF_EMPTY(TXB)) DISPATCH();

#ifdef __MPLAB_SIM // Poll error bits and set ERFLAG
 if (UART_IS_RXERR(UART_USED)) UART_SET_ERFLAG(UART_USED);
#endif // SIM doesn't check receiver errors, but set OERR
}
//
// Receiver section
//
int sysu_rxsize(void) { return(QUEBUF_SIZE(RXB)); }

int sysu_rxcount(void) { return(QUEBUF_LEN(RXB)); }

void sysu_rxpurge(void)
{
	_DISABLE_();
	{
		// Clear buffer, errors and FIFO
		U_(UART_USED, rxerr) = 0; QUEBUF_INIT(RXB);
		while (UART_CAN_READ(UART_USED)) UART_READ9(UART_USED);
	}
	_ENABLE_();
}

int sysu_getc(void)
{
	int i = -1;
	_DISABLE_();
	{
		if (!QUEBUF_EMPTY(RXB)) _QUEBUF_POP(RXB, i);
	}
	_ENABLE_();
	return( i );
}

// Receiver Interrupt Service Routine
void UART_INTFUNC(UART_USED, RX, no_auto_psv)(void)
{
	// Clear Interrupt flag
	UART_CLR_RXFLAG(UART_USED);

	while (!QUEBUF_FULL(RXB))
	{ // Read bytes from FIFO to buffer
		if (UART_CAN_READ(UART_USED)) {
			// Push received bytes into RX buffer
			_QUEBUF_PUSH(RXB, UART_READ8(UART_USED));
		} else break; // FIFO is empty
	} // while (!QUEBUF_FULL(RXB))

	// If receiver queue is full:
	//  ignore received character
	//  and leave it into RX FIFO (and ++errors)
	if (QUEBUF_FULL(RXB)) ++U_(UART_USED, rxerr);

	DISPATCH();
}

// Error Interrupt Service Routine
void UART_INTFUNC(UART_USED, Err, no_auto_psv)(void)
{
	// Clear Interrupt flag
	UART_CLR_ERFLAG(UART_USED);

	if (UART_IS_OERR(UART_USED))
	{ // Overrun FIFO
		UART_CLR_OERR(UART_USED); // Clear FIFO and OERR
		// No errors at this point (FIFO is empty)
	}
	else
	{ // Frame or parity error - dummy read
		int i = UART_READ9(UART_USED);
		if (UART_IS_FERR(UART_USED) && (i == 0))
		{	// Break character is received:
			__asm__("nop"); // TODO: AutoBaud
			__asm__("nop"); //    Mode can be
			__asm__("nop"); //    started here
		} // Break condition
	}

	// Calculate errors
	++U_(UART_USED, rxerr);

	DISPATCH();
}
