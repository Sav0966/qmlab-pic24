#include <p24Fxxxx.h>
#include <config.h>
#include <clock.h>
#include <uart.h>

#define UART_CHECKED	1 // Checked URAT module (1-4)

#if (UART_CHECKED > 1)
// No SHDN & INVALID pins
 #undef UART_SHDN
 #undef UART_WAKEUP
 #undef UART_IS_VALID
 #define UART_IS_VALID(n) 1
 #define UART_WAKEUP(n)
 #define UART_SHDN(n)
#endif

#if (defined(__DEBUG) && !defined(__MPLAB_DEBUGGER_ICD2))
 /* For MPLAB SIM and unknown debugger tools (not ICD2) */
 #define _DEBUG_SIM
 #undef  U1_INVALID
 #define U1_INVALID		1
 #define U_LPBACK		0x0040 // Loopback Mode Select bit
#endif

static int _rx_err_num; // = 0; Receiver Errors counter

UART_ER_INTFUNC(UART_CHECKED)
{
	// Clear Interrupt flag first
	UART_CLR_ERFLAG(UART_CHECKED);
	++_rx_err_num; // Calculate errors

	// Rx FIFO Buffer oveflow error
	if (UART_IS_OERR(UART_CHECKED)) {
			__asm__("nop"); // TODO: You can read
			__asm__("nop"); // and store FIFO before
			__asm__("nop"); // clear buffer and OERR
		UART_CLR_OERR(UART_CHECKED);  // Clear flag
		// No errors at this point (FIFO is empty)
	} else {
		// Frame error at the top of FIFO
		if (UART_IS_FERR(UART_CHECKED)) {
			// Read Data and check Break codition
			if (0 == UART_READ9(UART_CHECKED)) {
				// Break character is received
				__asm__("nop"); // TODO: AutoBaud
				__asm__("nop"); // Mode can be started
				__asm__("nop"); // in this section
 			}	
		}

		// Parity error at the top of FIFO
		if (UART_IS_PERR(UART_CHECKED)) {
			// Read the byte from the top of FIFO
			UART_READ9(UART_CHECKED); // Dummy read
		}
	}
}

UART_RX_INTFUNC(UART_CHECKED) { UART_CLR_RXFLAG(UART_CHECKED); }

UART_TX_INTFUNC(UART_CHECKED)
{
#ifdef _DEBUG_SIM // Poll error bits and set ERFLAG manually 
// if (UART_IS_INIT(UART_CHECKED)) // Check OERR and call IFR
  if (UART_IS_ERR(UART_CHECKED)) UART_SET_ERFLAG(UART_CHECKED);
#endif // SIM doesn't check receiver errors, but set OERR right

	UART_CLR_TXFLAG(UART_CHECKED);
}

void uart_test(void)
{ // Called from Main Loop once per 10 ms

#ifdef _DEBUG_SIM // Poll error bits and set ERFLAG manually 
	if (UART_IS_INIT(UART_CHECKED))
  	 if (UART_IS_ERR(UART_CHECKED))
	  UART_SET_ERFLAG(UART_CHECKED); // Check OERR and call IFR
#endif // SIM doesn't check receiver errors, but set OERR right

	if (sys_clock() & 0x3F) return;
	// Once per 0.64 seccond check UART

	if (!UART_IS_INIT(UART_CHECKED))
		UART_INIT(
			UART_CHECKED,	// Try to initialize UART

#if (defined(_DEBUG_SIM) && (UART_CHECKED == 2))
/* =!= It works with UART2 if set LPBACK here */ U_LPBACK |
#endif // SIM supports UART1 only
			U_NOPARITY | U_EN,		// 8-bit, no parity; Enabled

			U_TXEN, FCY2BRG(FCY2, 9600), // TX Enabled; 9600 baud
			1, 1, 2); // All interrupts are enabled (error level 2)

	if (!UART_IS_INIT(UART_CHECKED)) return;

	// When the UTXEN bit is set, the UxTXIF flag bit will also be set,
	// after two cycles, if UTXISEL<1:0> = 00, since the transmit buffer
	// is not yet full (can move transmit data to the UxTXREG register).

	// =!= So, TX interrupt may be called (if UART initialization done)

	UART_SET_LPBACK(UART_CHECKED);

#if (defined(_DEBUG_SIM) && (UART_CHECKED > 2))
/* The next code checks compiller errors */ return;
#endif // SIM supports UART1 only

	// Transmission of Break Characters:
	while (!UART_IS_TXEND(UART_CHECKED)); // Wait for TX to be Idle
	// The user should wait for the transmitter to be Idle (TRMT = 1)
	// before setting the UTXBRK. The UTXBRK overrides any other
	// transmitter activity. If the user clears the UTXBRK bit prior
	// to sequence completion, unexpected module behavior can result.
	// Sending a Break character does not generate a transmit interrupt
	UART_SET_BREAK(UART_CHECKED); // Set UTXBRK bit to send Break char
	UART_WRITE(UART_CHECKED, -1); // Write dummy character (Send Break)
	UART_WRITE(UART_CHECKED, 0x55); // Write Synch character
	// Don't clear UTXBRK bit manualy
			
	UART_WRITE(UART_CHECKED, 0x3F); // Write separator - '?'
	while (!UART_IS_TXEND(UART_CHECKED)); // Wait (no OERR yet)
	while (UART_CAN_WRITE(UART_CHECKED)) // Overflow RX FIFO
		UART_WRITE(UART_CHECKED, 0x55);
}
