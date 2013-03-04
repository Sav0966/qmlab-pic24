#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <uartui.h>
#include <clock.h>
#include <errno.h>

#define UART_USED		2	// Checked UART module
#define UART_RXBUF_SIZE	16	// Size of Receiver queue
#define UART_TXBUF_SIZE	16	// Size of Transmitter queue
//#include "uartui.c" Insert code directly at the bottom

DECL_UART_UI(UART_USED);

#if (UART_USED == 1)	// UART1 is used for tracing
#undef ASSERT
#undef TRACE
#define ASSERT(f)
#define TRACE(sz)
#endif

#define SMALL_STRING "Small string\n"

static int _buf[64]; // Read buffer
static int stage = 0; // Test stage
static int _ch, _cnt; // Temp vars

#ifndef ARSIZE
#define ARSIZE(buf) (sizeof(buf)/sizeof(buf[0]))
#endif

#ifdef __MPLAB_SIM // For MPLAB SIM
 #define U_LPBACK		0x0040 // Loopback Mode Select bit
#endif

#define uart_is_valid()	UART_IS_VALID(UART_USED)
#define uart_is_init()	UART_IS_INIT(UART_USED)
#define uart_done()		UART_PWOFF(UART_USED)

int uart_init(void)
{
	UART_INIT(UART_USED,	// Try to initialize UART

#if (defined(__MPLAB_SIM) && (UART_USED == 2))
/* =!= It works with UART2 too if set LPBACK here */	U_LPBACK |
#endif // SIM supports UART1 only (SIM: UART1 IO must be enabled)

		U_NOPARITY | U_EN,		// 8-bit, no parity; Enabled

		U_TXI_END | U_RXI_ANY | // Prevent dummy TX interrupt
		U_TXEN, FCY2BRG(FCY2, 9600), // TX Enabled; 9600 baud
		2, 2 // All interrupts are enabled
	);

	if (UART_IS_INIT(UART_USED)) {
		uart_tx_purge(UART_USED); uart_rx_purge(UART_USED);
	} else return(ENOENT); // Error: pin ~INVALID = '0'

	return(0);
}

void uart_test(void)
{ // Called from Main Loop more often than once per 10 ms
#ifdef __MPLAB_SIM // Poll error bits and set ERFLAG
 if (UART_IS_INIT(UART_USED)) // Check OERR and call IFR
  if (UART_IS_RXERR(UART_USED)) UART_SET_ERFLAG(UART_USED);
#endif // SIM doesn't check receiver errors, but set OERR

	if (sys_clock() & 0x3F) return;
	// Once per 0.64 seccond test UART

	if (!uart_is_init()) uart_init(); // Try init
	if (!uart_is_valid()) { // Power off UART if the
		uart_done(); return; } // pin ~INVALID = '0'

	// UART_IS_INIT() == TRUE

	// When the UTXEN bit is set, the UxTXIF flag bit will also be set,
	// after two cycles, if UTXISEL<1:0> = 00, since the transmit buffer
	// is not yet full (can move transmit data to the UxTXREG register).

	// =!= So, TX interrupt may be called (if UART initialization done)

	UART_SET_LPBACK(UART_USED); // Loopback mode (TxD->RxD)

#if (defined(__MPLAB_SIM) && (UART_USED > 2)) // UART3-4
/* The next code checks compiller errors only */ return;
#endif // SIM does not support UART3-4

	switch(stage) {
		case 0:
			TRACE("Transmission of BREAK sequence\n");

			while (!UART_IS_TXEND(UART_USED)); // Wait for TX to be Idle
			// The user should wait for the transmitter to be Idle (TRMT = 1)
			// before setting the UTXBRK. The UTXBRK overrides any other
			// transmitter activity. If the user clears the UTXBRK bit prior
			// to sequence completion, unexpected module behavior can result.
			// Sending a Break character does not generate a transmit interrupt
			UART_SET_BREAK(UART_USED); // Set UTXBRK bit to send Break char
			UART_WRITE(UART_USED, 0); // Write dummy character (Send Break)
			UART_WRITE(UART_USED, 0x55); // Write Synch character
			// Don't clear UTXBRK bit manualy

			stage = 1; break; // Next test

		case 1:
			if (!UART_IS_TXEND(UART_USED)) break; // Wait the sequence ends
			if (!UART_IS_RXIDLE(UART_USED)) break; // Wait for RX to be Idle

			_cnt = uart_rx_count(UART_USED); // Check number of received
			TRACE1("=> Received %i characters:", _cnt);   // characters
			while (uart_rx_count(UART_USED)) {
				if ((_ch = uart_getc(UART_USED)) == EOF) break;
				TRACE1(" %2.2X", _ch); // Dump receiver buffer
			} TRACE("\n");

#ifndef __MPLAB_SIM // Hardware test
			ASSERT(_cnt == 1); // First char '0' whith OERR error
			ASSERT(_ch = 0x55); // Only one byte 0x55 is received
			ASSERT(uart_er_count(UART_USED) == 1); // OERR
#endif // MPLAB SIM doesn't 
			uart_rx_purge(UART_USED); // Clear RX buffer errors

			__asm__ volatile ("nop\nnop\nnop"); // breakpoint
			stage = 2; break; // Next test

		case 2: break;

		default: ASSERT(!"Invalid stage of test");
	}

	return; // Stop here

	while (uart_er_count(UART_USED) <= 100) {
		if ((sizeof(SMALL_STRING)-1) !=
			uart_write(UART_USED, SMALL_STRING, sizeof(SMALL_STRING)-1))
		{
				uart_er_count(UART_USED);
				stage = 1; // TX queue is full
		}
		else return; // Overrun buffer later
	}
} 

// #include "uart.c"
// Insert code directly for debugging

#ifdef UART_USED
 #ifdef	RXB
  #undef	RXB
  #undef	TXB
 #endif
 // Define Buffers IDs
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
 #else	// UART_USED == 1-4
  #undef UART_USED // Error
 #endif
#else
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

// Receiver and transmitterqueue queues
static char QUEBUF(RXB, UART_RXBUF_SIZE);
static char QUEBUF(TXB, UART_TXBUF_SIZE);

#define _UART_ERR_NUM(n)	uart_##n##_RXERR_num
#define UART_ERR_NUM(n)	_UART_ERR_NUM(n)
static int UART_ERR_NUM(UART_USED) = 0;

IMPL_UBUF_COUNT(UART_USED, TX) { return QUE_BUF_LEN(TXB); }
IMPL_UBUF_COUNT(UART_USED, RX) { return QUE_BUF_LEN(RXB); }
IMPL_UBUF_COUNT(UART_USED, ER) { return UART_ERR_NUM(UART_USED); }

IMPL_UBUF_FULL(UART_USED, TX)  { return QUE_BUF_FULL(TXB); }
IMPL_UBUF_FULL(UART_USED, RX)  { return QUE_BUF_FULL(RXB); }

IMPL_UBUF_PURGE(UART_USED, RX)
{ // Reset RX queue and RX FIFO
	ASSERT(SRbits.IPL == MAIN_IPL); // Access from main thread only
	ASSERT(UART_IS_ENABLE_RXINT(UART_USED)); // UART must be init

	UART_DISABLE_RXINT(UART_USED); // Lock receiver thread and clear all
	QUE_BUF_RESET(RXB); while (UART_CAN_READ(UART_USED)) UART_READ9(UART_USED);
	UART_ERR_NUM(UART_USED) = 0; // Reset receiver error counter (no errors)
	UART_ENABLE_RXINT(UART_USED); // Unlock receiver thread
}

IMPL_UBUF_PURGE(UART_USED, TX)
{ // Reset TX queue and TX FIFO
	ASSERT(SRbits.IPL == MAIN_IPL); // Access from main thread only
	ASSERT(UART_IS_ENABLE_TXINT(UART_USED)); // UART must be init
	ASSERT(UART_IS_ENABLE_TX(UART_USED));   // UART must be init

	UART_DISABLE_TXINT(UART_USED); // Lock transmitter thread and clear all
	QUE_BUF_RESET(TXB); UART_DISABLE_TX(UART_USED); UART_ENABLE_TX(UART_USED);
	UART_ENABLE_TXINT(UART_USED); // Unlock receiver thread
}

IMPL_UART_WRITE(UART_USED)
{
	int n;
	ASSERT(SRbits.IPL == MAIN_IPL);

	// Access from main thread only
	for (n = 0; n != len; n++) {
		if (QUE_BUF_FULL(TXB)) break;
		QUE_BUF_PUSH(TXB, *buf++);
	} // Write n chars in TX queue
	UART_SET_TXFLAG(UART_USED);
	return(n);
}

IMPL_UART_GETC(UART_USED)
{
	int c;
	ASSERT(SRbits.IPL == MAIN_IPL);

	// Access from main thread only
	if (QUE_BUF_EMPTY(RXB)) c = EOF;
	else { c = QUE_BUF_POP(RXB); }
	return(c);
}

// Transmitter Interrupt Service Routine
void UART_INTFUNC(UART_USED, TX)(void)
{
	int i;
	// Clear Interrupt flag
	UART_CLR_TXFLAG(UART_USED);

	switch(QUE_BUF_LEN(TXB)) {
		case 0: i = U_TXI_END; break;
		case 1: i = U_TXI_READY; break;
		default: i = U_TXI_EMPTY; // We'll fill FIFO
	}

	UART_SET_TXI(UART_USED, i);

	while (!QUE_BUF_EMPTY(TXB)) {
	 // Load TX queue and fill TX FIFO
		if (UART_CAN_WRITE(UART_USED)) {
			i = QUE_BUF_POP(TXB);
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
	// Clear Interrupt flag
	UART_CLR_RXFLAG(UART_USED);

	if (UART_IS_RXERR(UART_USED)) { // It's not my job
				UART_SET_ERFLAG(UART_USED); return; }

	// No errors at the top of FIFO

	while (UART_CAN_READ(UART_USED)) {
	 // Read all bytes from FIFO
		if (!QUE_BUF_FULL(RXB)) {
			// and write readed bytes into RX buffer
			QUE_BUF_PUSH(RXB, UART_READ8(UART_USED));
		} else {
			// Receiver queue is full

			// Ignore received character
			// and leave it into RX FIFO

			break; // Calculate  errors in OERR handler
		}
	}
}

// Error Interrupt Service Routine
void UART_INTFUNC(UART_USED, Err)(void)
{
	// Clear Interrupt flag
	UART_CLR_ERFLAG(UART_USED);

	while (UART_IS_RXERR(UART_USED)) {
		if (UART_IS_OERR(UART_USED)) {
			// Rx FIFO Buffer overrun error:
			while (!QUE_BUF_FULL(RXB)) { // Try to store
				if (UART_CAN_READ(UART_USED)) { // RX FIFO
					QUE_BUF_PUSH(RXB, UART_READ8(UART_USED));
				} else break;
			}

			// Clear FIFO and OERR
			UART_CLR_OERR(UART_USED);

			// No errors at this point (FIFO is empty)

			// Calculate errors
			++UART_ERR_NUM(UART_USED);
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

	if (UART_CAN_READ(UART_USED)) // There is any bytes to read
		 UART_SET_RXFLAG(UART_USED); // Set interrupt flag only
}

#endif // UART_USED

// The same for UART3-4

#undef UART_USED
#define UART_USED	3
#undef UART_RXBUF_SIZE
#undef UART_TXBUF_SIZE
#define UART_RXBUF_SIZE	8
#define UART_TXBUF_SIZE	8	// Use new sizes
#include "uartui.c"

#undef UART_USED
#define UART_USED	4
#undef UART_RXBUF_SIZE
#undef UART_TXBUF_SIZE	// Use default sizes
#include "uartui.c"
