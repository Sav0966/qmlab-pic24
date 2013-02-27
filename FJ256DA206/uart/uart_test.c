#include <p24Fxxxx.h>
#include <config.h>
#include <_tools.h>
#include <uartui.h>
#include <clock.h>

#define UART_USED	1			// Checked URAT module (1-4)
// You must enable UART1 IO in SIM Simulator Settings Dialog
// Check 'Enable UART1 IO', 'Rewind input', Output 'Window'
#define UART_TXBUF_SIZE	32 // Size of Transmitter queue
#define UART_RXBUF_SIZE	16 // Size of Receiver queue

//#include "uart.c"			Include it at the bottom

DECL_UART_UI(UART_USED);

static int stage = 0; // Tesg Flag

#define SMALL_STRING "\nThere is a small string"

#if (UART_USED != 1) // No ~SHDN & ~INVALID pins
 #undef UART_SHDN
 #undef UART_WAKEUP
 #undef UART_IS_VALID
 #define UART_IS_VALID(n) 1
 #define UART_WAKEUP(n)
 #define UART_SHDN(n)
#endif // No ~SHDN & ~INVALID pins

#ifdef __MPLAB_SIM // For MPLAB SIM
 #undef  U1_INVALID
 #define U1_INVALID		1 // UART1 ~INVALID input pin = 1
 #define U_LPBACK		0x0040 // Loopback Mode Select bit
#endif

void uart_test(void)
{ // Called from Main Loop once per 10 ms
	int err;

#ifdef __MPLAB_SIM // Poll error bits and set ERFLAG
 if (UART_IS_INIT(UART_USED)) // Check OERR and call IFR
  if (UART_IS_ERR(UART_USED)) UART_SET_ERFLAG(UART_USED);
#endif // SIM doesn't check receiver errors, but set OERR

	if (sys_clock() & 0x3F) return;
	// Once per 0.64 seccond test UART

	if (!UART_IS_INIT(UART_USED))
	{
		UART_INIT(UART_USED,	// Try to initialize UART

#if (defined(__MPLAB_SIM) && (UART_USED == 2))
/* =!= It works with UART2 too if set LPBACK here */	U_LPBACK |
#endif // SIM supports UART1 only (SIM: UART1 IO must be enabled)

			U_NOPARITY | U_EN,		// 8-bit, no parity; Enabled

			U_TXEN, FCY2BRG(FCY2, 9600), // TX Enabled; 9600 baud
			1, 1, 1); // All interrupts are enabled

		if (!UART_IS_INIT(UART_USED)) return; // ~INVALID = '0'

		uart_tx_purge(UART_USED); uart_rx_purge(UART_USED);
	}

	// UART_IS_INIT() == TRUE

	// When the UTXEN bit is set, the UxTXIF flag bit will also be set,
	// after two cycles, if UTXISEL<1:0> = 00, since the transmit buffer
	// is not yet full (can move transmit data to the UxTXREG register).

	// =!= So, TX interrupt may be called (if UART initialization done)

	UART_SET_LPBACK(UART_USED);

#if (defined(__MPLAB_SIM) && (UART_USED > 2))
/* The next code checks compiller errors only */ return;
#endif // SIM does not support UART3-4

	if (!uart_is_error(UART_USED)) // Once after reset
	{
		// Transmission of Break Characters:
		while (!UART_IS_TXEND(UART_USED)); // Wait for TX to be Idle
		// The user should wait for the transmitter to be Idle (TRMT = 1)
		// before setting the UTXBRK. The UTXBRK overrides any other
		// transmitter activity. If the user clears the UTXBRK bit prior
		// to sequence completion, unexpected module behavior can result.
		// Sending a Break character does not generate a transmit interrupt
		UART_SET_BREAK(UART_USED); // Set UTXBRK bit to send Break char
		UART_WRITE(UART_USED, -1); // Write dummy character (Send Break)
		UART_WRITE(UART_USED, 0x55); // Write Synch character
		// Don't clear UTXBRK bit manualy
			
		UART_WRITE(UART_USED, 0x3F); // Write separator - '?'
		while (!UART_IS_TXEND(UART_USED)); // Wait (no OERR yet)
		while (UART_CAN_WRITE(UART_USED)) // Overflow RX FIFO
			UART_WRITE(UART_USED, 0x55);

		return; // First time after reset
	}

	err = uart_er_count(UART_USED);

	while (err <= 100) {
		if ((sizeof(SMALL_STRING)-1) !=
			uart_write(UART_USED, SMALL_STRING, sizeof(SMALL_STRING)-1))
		{
				err = uart_er_count(UART_USED);
				stage = 1; // TX queue is full
		}
		else return; // Overrun buffer later
	}

	// Wait end of trnsmittion from the TX queue
	if ((stage == 1) && uart_tx_full(UART_USED)) return;

	ASSERT(0,"?");
} 

// #include "uart.c"

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
