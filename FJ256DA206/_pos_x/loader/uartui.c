#include <p24Fxxxx.h>
#include <config.h>
#include <uarts.h>
#include "main.h"

#ifndef _U_SHDN
#define __U_SHDN(n)	_U##n##_SHDN
#define _U_SHDN(n)	__U_SHDN(n)
#endif

void uart_init(PUARTBUF buf)
{ // Initialize UART module
	UART_DISABLE_TXINT(UART_USED);
	UART_DISABLE_RXINT(UART_USED);
	UART_DISABLE_ERINT(UART_USED);
	_UMD(UART_USED) = 0;	// Power on UART module
	_U_SHDN(UART_USED) = 1;	// Wake-up RS-232 driver

	// Setup mode (UART disabled). Setup control
	// bits, clear FIFO buffers and receiver errors
	UMODE(UART_USED) = U_NOPARITY; // 8-bit no parity

	USTA(UART_USED) = U_TXI_READY | U_RXI_ANY; // Setup status
	UBRG(UART_USED) = FCY2BRG(FCY2, 9600);  // 9600 baud rate

	buf->nrx = 0; // Clear buffers
	buf->ntx = 0; buf->err = 0; buf->pos = 0;

	UART_CLR_RXFLAG(UART_USED); UART_CLR_TXFLAG(UART_USED);
	UART_CLR_ERFLAG(UART_USED);	// Clear interrupt flags

	UMODEbits(UART_USED).UARTEN = 1; // Enable module
	UART_ENABLE_TX(UART_USED); // Enable Transmitter
}

void uart_rx(PUARTBUF buf)
{ // First clear interrupt flag
	UART_CLR_RXFLAG(UART_USED);

	while (buf->nrx < RXBUF_SIZE)
	{ // Read bytes from FIFO to buffer
		if (UART_CAN_READ(UART_USED)) {
			if (UART_IS_RXERR(UART_USED)) {
				UART_SET_ERFLAG(UART_USED);
				break; // It's not my job
			} else { // No errors at the top of FIFO
				// Push received bytes into RX buffer
				buf->rxd[buf->nrx++] = UART_READ8(UART_USED);
			}
		} else break; // FIFO is empty
	} // while (nrx != RXBUF_SIZE)

	// If receiver queue is full:
	//  ignore received character
	//  and leave it in RX FIFO
}

void uart_er(PUARTBUF buf)
{ // First clear interrupt flag
	UART_CLR_ERFLAG(UART_USED);

	while (UART_IS_RXERR(UART_USED))
	{
		if (UART_IS_OERR(UART_USED)) {
			// Rx FIFO Buffer overrun error:

			// Clear FIFO and OERR
			UART_CLR_OERR(UART_USED);

			// No errors at this point (FIFO is empty)

		} else {
			if (UART_IS_FERR(UART_USED)) {
				// Frame error at the top of FIFO:
				// Read Data and check Break codition
				if (0 == UART_READ9(UART_USED)) {
					// Break character is received:
					__asm__("nop"); // TODO: AutoBaud
					__asm__("nop"); // Mode can be started
					__asm__("nop"); // in this section
	 			}	
			} else /* if (UART_IS_PERR(UART_USED)) */ {
				// Parity error at the top of FIFO:
				// Read the byte from the top of FIFO
				UART_READ9(UART_USED);  // Dummy read
			}
		}

		++buf->err; // Point to errors
	} // while (UART_IS_ERR(UART_USED))
}

void uart_tx(PUARTBUF buf)
{
	unsigned char ch;

	// First clear interrupt flag
	UART_CLR_TXFLAG(UART_USED);

	while (buf->ntx != 0) {
	 // Load TX queue and fill TX FIFO
		if (UART_CAN_WRITE(UART_USED)) {
			ch = buf->txd[buf->ntx--];
			UART_WRITE(UART_USED, ch);
		} else break; // FIFO is full
	}
}
