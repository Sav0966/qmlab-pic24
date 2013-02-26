#include <p24Fxxxx.h>
#include <config.h>
#include <buffer.h>
#include <_tools.h>
#include <clock.h>
#include <uart.h>
#include "main.h"

extern void uart1_rx_purge(void);
extern void uart_tx_purge(void);
extern int uart_tx_count(void);
extern int uart_rx_count(void);
extern int uart_er_count(void);

extern int uart_tx_empty(void);
extern int uart_rx_empty(void);
extern int uart_tx_full(void);
extern int uart_rx_full(void);
extern int uart_write (char *buf, int len);

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
		UART_INIT(UART_USED,	// Try to initialize UART

#if (defined(__MPLAB_SIM) && (UART_USED == 2))
/* =!= It works with UART2 too if set LPBACK here */	U_LPBACK |
#endif // SIM supports UART1 only (SIM: UART1 IO must be enabled)

			U_NOPARITY | U_EN,		// 8-bit, no parity; Enabled

			U_TXEN, FCY2BRG(FCY2, 9600), // TX Enabled; 9600 baud
			1, 1, 1); // All interrupts are enabled

		if (!UART_IS_INIT(UART_USED)) return; // ~INVALID = '0'

		uart_tx_purge(); uart1_rx_purge();
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

	if (uart_er_count() == 0) // Once after reset
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

	while (uart_er_count() == 0) {
		if ((sizeof(SMALL_STRING)-1) !=
			uart_write(SMALL_STRING, sizeof(SMALL_STRING)-1))
		{
				stage = uart_er_count(); // Break and view here
				stage = 1; // TX queue is full
		}
		else return; // Overrun buffer later
	}

	// Wait end of trnsmittion of the TX queue
	if ((stage == 1) && uart_tx_full()) return;

	ASSERT(0,"?");
} 
