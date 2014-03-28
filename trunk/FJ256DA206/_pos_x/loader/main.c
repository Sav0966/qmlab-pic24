#include <p24Fxxxx.h>
#include <libpic30.h>
#include <config.h>
#include <reset.h>
#include <uarts.h>
#include "main.h"

#define UART_USED	SYS_UART
#define RXBUF_SIZE	128
#define TXBUF_SIZE	16

#ifndef _U_SHDN
#define __U_SHDN(n)	_U##n##_SHDN
#define _U_SHDN(n)	__U_SHDN(n)
#endif

typedef struct tagUARTBUF {
	unsigned char nrx;
	unsigned char ntx;
	unsigned char err;
	unsigned char reserved;
	unsigned char txd[TXBUF_SIZE];
	unsigned char rxd[RXBUF_SIZE];
} UARTBUF, *PUARTBUF;

static void uart_init(PUARTBUF pbuf)
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
	UBRG(UART_USED) = FCY2BRGH(FCY2, 9600);  // 9600 baud rate

	pbuf->nrx = 0; // Clear buffer
	pbuf->ntx = 0; pbuf->err = 0;

	UART_CLR_RXFLAG(UART_USED); UART_CLR_TXFLAG(UART_USED);
	UART_CLR_ERFLAG(UART_USED);	// Clear interrupt flags

	UMODEbits(UART_USED).UARTEN = 1; // Enable module
	UART_ENABLE_TX(UART_USED); // Enable Transmitter
}

static void uart_rx(PUARTBUF pbuf)
{ // First clear interrupt flag
	UART_CLR_RXFLAG(UART_USED);

	while (pbuf->nrx != RXBUF_SIZE)
	{ // Read bytes from FIFO to buffer
		if (UART_CAN_READ(UART_USED)) {
			if (UART_IS_RXERR(UART_USED)) {
				UART_SET_ERFLAG(UART_USED);
				break; // It's not my job
			} else { // No errors at the top of FIFO
				// Push received bytes into RX buffer
				pbuf->rxd[pbuf->nrx++] = UART_READ8(UART_USED);
			}
		} else break; // FIFO is empty
	} // while (nrx != RXBUF_SIZE)

	// If receiver queue is full:
	//  ignore received character
	//  and leave it in RX FIFO
}

static void uart_er(PUARTBUF pbuf)
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

		pbuf->nrx = 0; // Clear buffer
		pbuf->err += 1; // Point to errors
	} // while (UART_IS_ERR(UART_USED))
}

static void uart_tx(PUARTBUF pbuf)
{
	unsigned char ch;

	// First clear interrupt flag
	UART_CLR_TXFLAG(UART_USED);

	while (pbuf->ntx != 0) {
	 // Load TX queue and fill TX FIFO
		if (UART_CAN_WRITE(UART_USED)) {
			ch = pbuf->txd[pbuf->ntx--];
			UART_WRITE(UART_USED, ch);
		} else break; // FIFO is full
	}
}

int main(void)
{
	UARTBUF uart_buf;

	pins_init(); // Initialize MCU pins first

	while (!IS_MCU_PROGRAMMED()); // Stay here
	// SIM doesn't clear SRbits.IPL, ICD2 clears it
	SET_CPU_IPL(MAIN_IPL); // Set default by hands

	if ((RCON & RCON_STATUS) == 0)
	{ // Another program clears RCON bits
	} else { /* No programms are in flash */ }

	uart_init(&uart_buf); // Initialize UART

	for(;;) { // Main loop

		// Check UART events:
		if (UART_IS_RXFLAG(UART_USED)) uart_rx(&uart_buf);
		if (UART_IS_ERFLAG(UART_USED)) uart_er(&uart_buf);
		if (UART_IS_TXFLAG(UART_USED)) uart_tx(&uart_buf);

		if (uart_buf.err != 0) { // Error

		}

		__asm__ volatile ("CLRWDT\n"); // Reset WDT
	} // Main loop

	return(0); /* Never return */
}
