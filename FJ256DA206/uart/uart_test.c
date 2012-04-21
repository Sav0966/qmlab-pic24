#include <p24Fxxxx.h>
#include <config.h>
#include <buffer.h>
#include <_tools.h>
#include <clock.h>
#include <uart.h>


// You must enable UART1 IO in SIM Simulator Settings Dialog
// Check 'Enable UART1 IO', 'Rewind input', Output 'Window'

#define UART_USED	1 // Checked URAT module (1-4)

#define TX_BUF_SIZE	32 // Size of Transmitter queue
#define RX_BUF_SIZE	16 // Size of Receiver queue
static int stage = 0; // Tesg Flag

#define SMALL_STRING "\nThere is a small string"

#if (UART_USED > 1) // No ~SHDN & ~INVALID pins
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

static char QUEUE(RX, RX_BUF_SIZE); // Receiver queue
static char QUEUE(TX, TX_BUF_SIZE); // Transmitter queue
static int _rx_err_num; // = 0; Receiver Error counter


static void rx_purge(void)
{ // Reset RX queue and RX FIFO
	ASSERT(SRbits.IPL == MAIN_IPL, "Access from main thread only");
	ASSERT(UART_IS_ENABLE_RXINT(UART_USED), "UART must be init");

	UART_DISABLE_RXINT(UART_USED); // Lock receiver thread and clear all
	QUE_RESET(RX); while (UART_CAN_READ(UART_USED)) UART_READ9(UART_USED);
	UART_ENABLE_RXINT(UART_USED); // Unlock receiver thread
}

static void tx_purge(void)
{ // Reset TX queue and TX FIFO
	ASSERT(SRbits.IPL == MAIN_IPL, "Access from main thread only");
	ASSERT(UART_IS_ENABLE_TXINT(UART_USED), "UART must be init");
	ASSERT(UART_IS_ENABLE_TX(UART_USED), "UART must be init");

	UART_DISABLE_TXINT(UART_USED); // Lock transmitter thread and clear all
	QUE_RESET(TX); UART_DISABLE_TX(UART_USED); UART_ENABLE_TX(UART_USED);
	UART_ENABLE_TXINT(UART_USED); // Unlock receiver thread
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
			++_rx_err_num; // Calculate errors
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

				++_rx_err_num; // Calculate errors
			}

			if (UART_IS_PERR(UART_USED)) {
				// Parity error at the top of FIFO:
				// Read the byte from the top of FIFO
				UART_READ9(UART_USED); // Dummy read
				++_rx_err_num; // Calculate errors
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

	if (QUE_FULL(RX)) {
		// Receiver queue is full
		// Can't reset it here (have not right)
		++_rx_err_num; // Calculate errors only
	} else {

	}
}

// Transmitter Interrupt Service Routine
void UART_INTFUNC(UART_USED, TX)(void)
{
	int i;
	// Clear Interrupt flag
	UART_CLR_TXFLAG(UART_USED);

	if (QUE_SIZE(TX) < 2) i = U_TXI_READY;
	else i = U_TXI_EMPTY; // We'll fill FIFO
	UART_SET_TXI(UART_USED, i);

	while (!QUE_EMPTY(TX)) {
	 // Load TX queue and fill TX FIFO
		if (UART_CAN_WRITE(UART_USED)) {
			i = QUE_POP(TX);
			UART_WRITE(UART_USED, i);
		} else break; // FIFO is full
	}

#ifdef __MPLAB_SIM // Poll error bits and set ERFLAG
 if (UART_IS_ERR(UART_USED)) UART_SET_ERFLAG(UART_USED);
#endif // SIM doesn't check receiver errors, but set OERR
}

static int uart_write (char *buf, int len)
{
	int n = 0;
	ASSERT(SRbits.IPL == MAIN_IPL, "Access from main thread only");

	while (n != len) {
		if (QUE_FULL(TX)) break;
		QUE_PUSH(TX, *buf++); ++n;
	} // Write n chars in TX queue
	UART_SET_TXFLAG(UART_USED);
	return(n);
}

void uart_test(void)
{ // Called from Main Loop once per 10 ms

#ifdef __MPLAB_SIM // Poll error bits and set ERFLAG
 if (UART_IS_INIT(UART_USED)) // Check OERR and call IFR
  if (UART_IS_ERR(UART_USED)) UART_SET_ERFLAG(UART_USED);
#endif // SIM doesn't check receiver errors, but set OERR

	if (sys_clock() & 0x3F) return;
	// Once per 0.64 seccond test UART

	if (!UART_IS_INIT(UART_USED))
	{
		QUE_INIT(RX); // RX and TX queues initialization
		QUE_INIT(TX); // May be called - UART Disabled

		UART_INIT(UART_USED,	// Try to initialize UART

#if (defined(__MPLAB_SIM) && (UART_USED == 2))
/* =!= It works with UART2 too if set LPBACK here */	U_LPBACK |
#endif // SIM supports UART1 only (SIM: UART1 IO must be enabled)

			U_NOPARITY | U_EN,		// 8-bit, no parity; Enabled

			U_TXEN, FCY2BRG(FCY2, 9600), // TX Enabled; 9600 baud
			1, 1, 1); // All interrupts are enabled

		if (!UART_IS_INIT(UART_USED)) return; // ~INVALID = '0'

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

	if (_rx_err_num == 0) // Once after reset
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

	if (_rx_err_num < 100) {
		if ((sizeof(SMALL_STRING)-1) !=
			uart_write(SMALL_STRING, sizeof(SMALL_STRING)-1))
			{_rx_err_num = 100; stage = 1; } // TX queue is full
		else return; // Overrun later
	}

	// Wait end of trnsmittion of the TX queue
	if ((stage == 1) && QUE_FULL(TX)) return;

	ASSERT(0,"?");
} 
