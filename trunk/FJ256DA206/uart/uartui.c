/*
*	Template code of user interface functions for UART1-4
*/
#include <p24Fxxxx.h>
#include <config.h>

#ifdef UART_USED
 #ifdef	RXB
  #undef	RXB
  #undef	TXB
 #endif
 // Define Buffers IDs
 #if (UART_USED == 1)
  #define RXB		U1RXB
  #define TXB		U1TXB
 #elif (UART_USED == 2)
  #define RXB		U2RXB
  #define TXB		U2TXB
 #elif (UART_USED == 3)
  #define RXB		U3RXB
  #define TXB		U3TXB
 #elif (UART_USED == 4)
  #define RXB		U4RXB
  #define TXB		U4TXB
 #else	// UART_USED == 1-4
  #undef UART_USED // Error
 #endif
#else
// No UART is used - no code will be produced
#endif

#ifdef UART_USED // Only for used UART

#include <_tools.h>
#include <buffer.h>
#include <uartui.h>

#ifndef UART_TXBUF_SIZE
// Default size of Transmitter queue
#define UART_TXBUF_SIZE				32
#endif

#ifndef UART_RXBUF_SIZE
// Default size of Receiver queue
#define UART_RXBUF_SIZE				32
#endif

#ifndef EOF
#define EOF -1
#endif

// Receiver and transmitterqueue queues
static volatile unsigned char QUEBUF(RXB, UART_RXBUF_SIZE);
static volatile unsigned char QUEBUF(TXB, UART_TXBUF_SIZE);

#define _UART_ERR_NUM(n)	uart_##n##_rxerr
#define UART_ERR_NUM(n)	_UART_ERR_NUM(n)

static volatile int UART_ERR_NUM(UART_USED) = 0;

IMPL_UBUF_COUNT(UART_USED, TX) { return QUEBUF_LEN(TXB); }
IMPL_UBUF_COUNT(UART_USED, RX) { return QUEBUF_LEN(RXB); }
IMPL_UBUF_COUNT(UART_USED, ER) { return UART_ERR_NUM(UART_USED); }

IMPL_UBUF_FULL(UART_USED, TX)  { return QUEBUF_FULL(TXB); }
IMPL_UBUF_FULL(UART_USED, RX)  { return QUEBUF_FULL(RXB); }

IMPL_UBUF_PURGE(UART_USED, RX)
{ // Reset RX queue, FIFO and errors
	while (UART_CAN_READ(UART_USED))
		UART_READ9(UART_USED); // Clear FIFO

	INTERLOCKED( // Clear buffer and errors
		_QUEBUF_INIT(RXB);
		UART_ERR_NUM(UART_USED) = 0;
	);
}

IMPL_UBUF_PURGE(UART_USED, TX)
{ // Reset TX queue and TX FIFO
	QUEBUF_INIT(TXB); // Clear buffer (locked)

	{ // Disable transmition and then restore TXEN
		register int _usta_to_save = USTA(UART_USED);
		UART_DISABLE_TX(UART_USED); // Clear FIFO by TXEN = 0
		if (_usta_to_save & U_TXEN) UART_ENABLE_TX(UART_USED);
	}
}

IMPL_UART_GETC(UART_USED)
{
	int c;
	// Access from main thread only
	ASSERT(SRbits.IPL == MAIN_IPL);

	if (QUEBUF_EMPTY(RXB)) c = EOF;
	else { c = QUEBUF_POP(RXB); }

	return(c);
}

IMPL_UART_READ(UART_USED)
{
	int c, n;
	for (n = 0; n != len; n++) {
		if ((c = uart_getc(UART_USED)) == EOF) break;
		*buf++ = (char)c;
	}

	return(n);
}

IMPL_UART_PUTC(UART_USED)
{
	// Access from main thread only
	ASSERT(SRbits.IPL == MAIN_IPL);

	if (QUEBUF_FULL(TXB)) c = EOF;
	else {
		QUEBUF_PUSH(TXB, (char)c);
		UART_SET_TXFLAG(UART_USED);
	}

	return(c);
}

IMPL_UART_WRITE(UART_USED)
{
	int n;
	for (n = 0; n != len; n++) {
		if (uart_putc(*buf++, UART_USED) == EOF) break;
	}

	return(n);
}

// Transmitter Interrupt Service Routine
void UART_INTFUNC(UART_USED, TX)(void)
{
	int i;
	// Clear Interrupt flag
	UART_CLR_TXFLAG(UART_USED);

	switch (QUEBUF_LEN(TXB)) {
		case 0: i = U_TXI_END; break;
		case 1: i = U_TXI_READY; break;
		default: i = U_TXI_EMPTY; // We'll fill FIFO
	}

	UART_SET_TXI(UART_USED, i);

	while (!QUEBUF_EMPTY(TXB)) {
	 // Load TX queue and fill TX FIFO
		if (UART_CAN_WRITE(UART_USED)) {
			i = QUEBUF_IPOP(TXB);
			UART_WRITE(UART_USED, i);
		} else break; // FIFO is full
	}

#ifdef __MPLAB_SIM // Poll error bits and set ERFLAG
 if (UART_IS_RXERR(UART_USED)) UART_SET_ERFLAG(UART_USED);
#endif // SIM doesn't check receiver errors, but set OERR
}

// Receiver Interrupt Service Routine
void UART_INTFUNC(UART_USED, RX)(void)
{
	UART_DISABLE_ERINT(UART_USED); // Lock ER

	// Clear Interrupt flag
	UART_CLR_RXFLAG(UART_USED);

	while (!QUEBUF_FULL(RXB)) {
	 // Read bytes from FIFO to buffer
		if (UART_CAN_READ(UART_USED)) {
			if (UART_IS_RXERR(UART_USED)) {
				UART_SET_ERFLAG(UART_USED);
				break; // It's not my job
			} else { // No errors at the top of FIFO
				// Write readed bytes into RX buffer
				QUEBUF_IPUSH(RXB, UART_READ8(UART_USED));
			}
		} else break; // FIFO is empty
	} // while (!QUEBUF_FULL(RXB))

	// If receiver queue is full:
	//  ignore received character
	//  and leave it into RX FIFO

	UART_ENABLE_ERINT(UART_USED); // Unlock ER
}

// Error Interrupt Service Routine
void UART_INTFUNC(UART_USED, Err)(void)
{
	UART_DISABLE_RXINT(UART_USED); // Lock RX

	// Clear Interrupt flag
	UART_CLR_ERFLAG(UART_USED);

	while (UART_IS_RXERR(UART_USED)) {
		if (UART_IS_OERR(UART_USED)) {
			// Rx FIFO Buffer overrun error:
			while (!QUEBUF_FULL(RXB)) { // Try to store
				if (UART_CAN_READ(UART_USED)) { // RX FIFO
					QUEBUF_IPUSH(RXB, UART_READ8(UART_USED));
				} else break;
			}

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

		// Calculate errors
		++UART_ERR_NUM(UART_USED);
	} //  while (UART_IS_ERR(UART_USED))

	UART_ENABLE_RXINT(UART_USED); // Unlock RX

	if (UART_CAN_READ(UART_USED)) // There is any bytes to read
		 UART_SET_RXFLAG(UART_USED); // Set interrupt flag only
}

#endif // UART_USED
