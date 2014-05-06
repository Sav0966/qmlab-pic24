#include <p24Fxxxx.h>
#include <config.h>
#include <buffer.h>
#include <_tools.h>

#include "sysuart.h"

#define UART_USED	SYS_UART

#define UART_RXBUF_SIZE		32
#define UART_TXBUF_SIZE		32

// Receiver and transmitterqueue queues and error flag
static volatile unsigned char QUEBUF(RXB, UART_RXBUF_SIZE);
static volatile unsigned char QUEBUF(TXB, UART_TXBUF_SIZE);
static volatile int U_(UART_USED, rxerr) /* = 0 */;

int sysu_error(void) { return(U_(UART_USED, rxerr)); }

int sysu_is_init(void) { return(UART_IS_INIT(UART_USED)); }

void sysu_done(void) { UART_PWOFF(UART_USED); }

int sysu_init(void)
{
	sysu_done(); // clear buffers
	sysu_txpurge(); sysu_rxpurge();
	U_(UART_USED, rxerr) = 0;

	UART_INIT(UART_USED, // Try to initialize UART

#if (defined(__MPLAB_SIM) && (UART_USED == 2))
/* =!= It works with UART2 too if set LPBACK here */	U_LPBACK |
#endif // SIM supports UART1 only (SIM: UART1 IO must be enabled)

		U_NOPARITY | UART_EN,			// 8-bit, no parity; Enabled
		U_TXI_READY | U_RXI_ANY |		// Defaul event settings
		U_TXEN, FCY2BRG(FCY2, 9600),	// TX Enabled; 9600 baud
		UART_IPL					// All interrupts are enabled
	); // UART_INIT()

	return( sysu_is_init() );
}
//
// Transmitter section
//
#undef INTERLOCKED
#define INTERLOCKED(f)	UART_DISABLE_TXINT(UART_USED);\
						f; UART_ENABLE_TXINT(UART_USED)

int sysu_txsize(void) { return(QUEBUF_SIZE(TXB)); }

int sysu_txcount(void) { return(QUEBUF_LEN(TXB)); }

void sysu_txpurge(void)
{ // Reset TX queue and TX FIFO
	QUEBUF_INIT(TXB); // Clear buffer (locked)

	if (UART_IS_ENABLE_TX(UART_USED)) { // Clear FIFO if needed 
		UART_DISABLE_TX(UART_USED); // Clear FIFO by TXEN = 0
		UART_ENABLE_TX(UART_USED);	// Then restore TXEN
	}
}

// Transmitter Interrupt Service Routine
void UART_INTFUNC(UART_USED, TX, no_auto_psv)(void)
{
	// Clear Interrupt flag
	UART_CLR_TXFLAG(UART_USED);

	switch (QUEBUF_LEN(TXB)) {
		case 0:  UART_SET_TXI(UART_USED, U_TXI_END); break;
		case 1:  UART_SET_TXI(UART_USED, U_TXI_READY); break;
		default: UART_SET_TXI(UART_USED, U_TXI_EMPTY);
			break; // We'll fill FIFO in this case
	}

	while (!QUEBUF_EMPTY(TXB))
	{ // Load TX queue and fill TX FIFO
		if (UART_CAN_WRITE(UART_USED)) {
			UART_WRITE(UART_USED, QUEBUF_IPOP(TXB));
		} else break; // FIFO is full
	}

#ifdef __MPLAB_SIM // Poll error bits and set ERFLAG
 if (UART_IS_RXERR(UART_USED)) UART_SET_ERFLAG(UART_USED);
#endif // SIM doesn't check receiver errors, but set OERR
}
//
// Receiver section
//
#undef INTERLOCKED
#define INTERLOCKED(f)	UART_DISABLE_RXINT(UART_USED);\
						f; UART_ENABLE_RXINT(UART_USED)

int sysu_rxsize(void) { return(QUEBUF_SIZE(RXB)); }

int sysu_rxcount(void) { return(QUEBUF_LEN(RXB)); }

void sysu_rxpurge(void)
{
	while (UART_CAN_READ(UART_USED))
		UART_READ9(UART_USED); // Clear FIFO

	// Clear buffer and errors
	QUEBUF_INIT(RXB); U_(UART_USED, rxerr) = 0;
}

// Receiver Interrupt Service Routine
void UART_INTFUNC(UART_USED, RX, no_auto_psv)(void)
{
	// Clear Interrupt flag
	UART_CLR_RXFLAG(UART_USED);

	while (!QUEBUF_FULL(RXB))
	{ // Read bytes from FIFO to buffer
		if (UART_CAN_READ(UART_USED)) {
			if (UART_IS_RXERR(UART_USED)) {
				UART_SET_ERFLAG(UART_USED);
				break; // It's not my job
			} else { // No errors at the top of FIFO
				// Push received bytes into RX buffer
				QUEBUF_IPUSH(RXB, UART_READ8(UART_USED));
			}
		} else break; // FIFO is empty
	} // while (!QUEBUF_FULL(RXB))

	// If receiver queue is full:
	//  ignore received character
	//  and leave it into RX FIFO
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

	if (UART_CAN_READ(UART_USED)) // There is any bytes to read
		UART_SET_RXFLAG(UART_USED); // Set interrupt flag only
}
