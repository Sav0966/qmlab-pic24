#include <p24Fxxxx.h>
#include <config.h>
#include <buffer.h>
#include <_tools.h>
#include <uart.h>
#include "main.h"

#ifndef UART_TXBUF_SIZE
// Default size of Transmitter queue
#define UART_TXBUF_SIZE				32
#endif

#ifndef UART_RXBUF_SIZE
// Default size of Receiver queue
#define UART_RXBUF_SIZE				32
#endif

static char QUEUE(URX, UART_RXBUF_SIZE); // Receiver queue
static char QUEUE(UTX, UART_TXBUF_SIZE); // Transmitter queue
static int uart_rxerr_num;    // = 0; Receiver Error counter

#define _UART_RX_PURGE(n)	uart##n##_rx_purge
#define UART_RX_PURGE(n)	_UART_RX_PURGE(n)

void UART_RX_PURGE(UART_USED)(void)
{ // Reset RX queue and RX FIFO
	ASSERT(SRbits.IPL == MAIN_IPL, "Access from main thread only");
	ASSERT(UART_IS_ENABLE_RXINT(UART_USED), "UART must be init");

	UART_DISABLE_RXINT(UART_USED); // Lock receiver thread and clear all
	QUE_RESET(URX); while (UART_CAN_READ(UART_USED)) UART_READ9(UART_USED);
	UART_ENABLE_RXINT(UART_USED); // Unlock receiver thread
}

void uart_tx_purge(void)
{ // Reset TX queue and TX FIFO
	ASSERT(SRbits.IPL == MAIN_IPL, "Access from main thread only");
	ASSERT(UART_IS_ENABLE_TXINT(UART_USED), "UART must be init");
	ASSERT(UART_IS_ENABLE_TX(UART_USED), "UART must be init");

	UART_DISABLE_TXINT(UART_USED); // Lock transmitter thread and clear all
	QUE_RESET(UTX); UART_DISABLE_TX(UART_USED); UART_ENABLE_TX(UART_USED);
	UART_ENABLE_TXINT(UART_USED); // Unlock receiver thread
}

int uart_tx_count(void) { return QUE_SIZE(UTX); }
int uart_rx_count(void) { return QUE_SIZE(URX); }
int uart_er_count(void) {return uart_rxerr_num; }

int uart_tx_empty(void) { return QUE_EMPTY(UTX); }
int uart_rx_empty(void) { return QUE_EMPTY(URX); }

int uart_tx_full(void) { return QUE_FULL(UTX); }
int uart_rx_full(void) { return QUE_FULL(URX); }

int uart_write (char *buf, int len)
{
	int n = 0;
	ASSERT(SRbits.IPL == MAIN_IPL, "Access from main thread only");

	while (n != len) {
		if (QUE_FULL(UTX)) break;
		QUE_PUSH(UTX, *buf++); ++n;
	} // Write n chars in TX queue
	UART_SET_TXFLAG(UART_USED);
	return(n);
}

// Error Interrupt Service Routine
void UART_INTFUNC(UART_USED, Err)(void)
{
	// Clear Interrupt flag
	UART_CLR_ERFLAG(UART_USED);

	while (UART_IS_ERR(UART_USED)) {
		if (UART_IS_OERR(UART_USED)) {
			// Rx FIFO Buffer overrun error:
			__asm__("nop"); // TODO: You can read
			__asm__("nop"); // and store FIFO before
			__asm__("nop"); // clear buffer and OERR
			UART_CLR_OERR(UART_USED);  // Clear flag
			// No errors at this point (FIFO is empty)
			++uart_rxerr_num; // Calculate errors
		} else {
			if (UART_IS_FERR(UART_USED)) {
				// Frame error at the top of FIFO:
				// Read Data and check Break codition
				if (0 == UART_READ9(UART_USED)) {
					// Break character is received
					__asm__("nop"); // TODO: AutoBaud
					__asm__("nop"); // Mode can be started
					__asm__("nop"); // in this section
	 			}	

				++uart_rxerr_num; // Calculate errors
			}

			if (UART_IS_PERR(UART_USED)) {
				// Parity error at the top of FIFO:
				// Read the byte from the top of FIFO
				UART_READ9(UART_USED);  // Dummy read
				++uart_rxerr_num; // Calculate errors
			}
		}
	} //  while (UART_IS_ERR(UART_USED))
}

// Receiver Interrupt Service Routine
void UART_INTFUNC(UART_USED, RX)(void)
{
	if (UART_IS_ERR(UART_USED)) { // It's not my job
				UART_SET_ERFLAG(UART_USED); return; }

	// No errors at the top of FIFO

	// Clear Interrupt flag
	UART_CLR_RXFLAG(UART_USED);

	if (QUE_FULL(URX)) {
		// Receiver queue is full
		// Can't reset it here (have not right)
		++uart_rxerr_num; // Calculate errors only
	} else {

	}
}

// Transmitter Interrupt Service Routine
void UART_INTFUNC(UART_USED, TX)(void)
{
	int i;
	// Clear Interrupt flag
	UART_CLR_TXFLAG(UART_USED);

	if (QUE_SIZE(UTX) < 2) i = U_TXI_READY;
	else i = U_TXI_EMPTY; // We'll fill FIFO
	UART_SET_TXI(UART_USED, i);

	while (!QUE_EMPTY(UTX)) {
	 // Load TX queue and fill TX FIFO
		if (UART_CAN_WRITE(UART_USED)) {
			i = QUE_POP(UTX);
			UART_WRITE(UART_USED, i);
		} else break; // FIFO is full
	}

#ifdef __MPLAB_SIM // Poll error bits and set ERFLAG
 if (UART_IS_ERR(UART_USED)) UART_SET_ERFLAG(UART_USED);
#endif // SIM doesn't check receiver errors, but set OERR
}
