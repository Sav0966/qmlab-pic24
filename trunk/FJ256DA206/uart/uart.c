/*
*	Template code of user interface functions for UART1-4
*/
#include <p24Fxxxx.h>
#include <config.h>

#ifdef UART_USED
 #if (UART_USED == 1)
  #define RXB		UBUF1_RX
  #define TXB		UBUF1_TX
 #elif (UART_USED == 2)
  #define RXB		UBUF2_RX
  #define TXB		UBUF2_TX
 #elif (UART_USED == 3)
  #define RXB		UBUF3_RX
  #define TXB		UBUF3_TX
 #elif (UART_USED == 4)
  #define RXB		UBUF4_RX
  #define TXB		UBUF4_TX
 #else	// UART_USED != 1-4
  #undef UART_USED
 #endif
#else
#endif

#ifdef UART_USED // Only for used UART
#include <buffer.h>
#include <_tools.h>
#include <uartui.h>

#ifndef UART_TXBUF_SIZE
// Default size of Transmitter queue
#define UART_TXBUF_SIZE				32
#endif

#ifndef UART_RXBUF_SIZE
// Default size of Receiver queue
#define UART_RXBUF_SIZE				32
#endif

static char QUEUE(RXB, UART_RXBUF_SIZE); // Receiver queue
static char QUEUE(TXB, UART_TXBUF_SIZE); // Transmitter queue

#define _UART_ERR_NUM(n)	uart_##n##_RXERR_num
#define UART_ERR_NUM(n)		_UART_ERR_NUM(n)
static int UART_ERR_NUM(UART_USED); // = 0

IMPL_UBUF_COUNT(URAT_USED, TX) { return QUE_SIZE(TXB); }
IMPL_UBUF_COUNT(UART_USED, RX) { return QUE_SIZE(RXB); }
IMPL_UBUF_COUNT(UART_USED, ER) { return UART_ERR_NUM(UART_USED); }

IMPL_UBUF_FULL(UART_USED, TX)  { return QUE_FULL(TXB); }
IMPL_UBUF_FULL(UART_USED, RX)  { return QUE_FULL(RXB); }

IMPL_UBUF_PURGE(UART_USED, RX)
{ // Reset RX queue and RX FIFO
	ASSERT(SRbits.IPL == MAIN_IPL, "Access from main thread only");
	ASSERT(UART_IS_ENABLE_RXINT(UART_USED), "UART must be init");

	UART_DISABLE_RXINT(UART_USED); // Lock receiver thread and clear all
	QUE_RESET(RXB); while (UART_CAN_READ(UART_USED)) UART_READ9(UART_USED);
	UART_ERR_NUM(UART_USED) = 0; // Reset receiver error counter (no errors)
	UART_ENABLE_RXINT(UART_USED); // Unlock receiver thread
}

IMPL_UBUF_PURGE(UART_USED, TX)
{ // Reset TX queue and TX FIFO
	ASSERT(SRbits.IPL == MAIN_IPL, "Access from main thread only");
	ASSERT(UART_IS_ENABLE_TXINT(UART_USED), "UART must be init");
	ASSERT(UART_IS_ENABLE_TX(UART_USED), "UART must be init");

	UART_DISABLE_TXINT(UART_USED); // Lock transmitter thread and clear all
	QUE_RESET(TXB); UART_DISABLE_TX(UART_USED); UART_ENABLE_TX(UART_USED);
	UART_ENABLE_TXINT(UART_USED); // Unlock receiver thread
}

IMPL_UART_WRITE(UART_USED)
{
	int n = 0;
	ASSERT(SRbits.IPL == MAIN_IPL, "Access from main thread only");

	while (n != len) {
		if (QUE_FULL(TXB)) break;
		QUE_PUSH(TXB, *buf++); ++n;
	} // Write n chars in TX queue
	UART_SET_TXFLAG(UART_USED);
	return(n);
}

// Transmitter Interrupt Service Routine
void UART_INTFUNC(UART_USED, TX)(void)
{
	int i;
	// Clear Interrupt flag
	UART_CLR_TXFLAG(UART_USED);

	if (QUE_SIZE(TXB) < 2) i = U_TXI_READY;
	else i = U_TXI_EMPTY; // We'll fill FIFO
	UART_SET_TXI(UART_USED, i);

	while (!QUE_EMPTY(TXB)) {
	 // Load TX queue and fill TX FIFO
		if (UART_CAN_WRITE(UART_USED)) {
			i = QUE_POP(TXB);
			UART_WRITE(UART_USED, i);
		} else break; // FIFO is full
	}

#ifdef __MPLAB_SIM // Poll error bits and set ERFLAG
 if (UART_IS_ERR(UART_USED)) UART_SET_ERFLAG(UART_USED);
#endif // SIM doesn't check receiver errors, but set OERR
}

// Receiver Interrupt Service Routine
void UART_INTFUNC(UART_USED, RX)(void)
{
	// Clear Interrupt flag
	UART_CLR_RXFLAG(UART_USED);

	if (UART_IS_ERR(UART_USED)) { // It's not my job
				UART_SET_ERFLAG(UART_USED); return; }

	// No errors at the top of FIFO

	if (QUE_FULL(RXB)) {
		// Receiver queue is full
		// Can't reset it here (have not right)

		// Calculate errors only
		++UART_ERR_NUM(UART_USED);
	} else {

	}
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

			// Calculate errors
			++UART_ERR_NUM(UART_USED);
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

				// Calculate errors
				++UART_ERR_NUM(UART_USED);
			}

			if (UART_IS_PERR(UART_USED)) {
				// Parity error at the top of FIFO:
				// Read the byte from the top of FIFO
				UART_READ9(UART_USED);  // Dummy read

				// Calculate errors
				++UART_ERR_NUM(UART_USED);
			}
		}
	} //  while (UART_IS_ERR(UART_USED))

	// There is any bytes to read - set interrupt flag only
	if (UART_CAN_READ(UART_USED)) UART_SET_RXFLAG(UART_USED);
}

#endif // UART_USED
