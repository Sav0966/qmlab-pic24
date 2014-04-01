#include <p24Fxxxx.h>
#include <config.h>
#include <reset.h>
#include <uarts.h>
#include "main.h"

volatile UARTBUF __attribute__((persistent)) buf;

int main(void)
{
	pins_init(); // Initialize MCU pins first

	while (!IS_MCU_PROGRAMMED()); // Stay here
	// SIM doesn't clear SRbits.IPL, ICD2 clears it
	SET_CPU_IPL(MAIN_IPL); // Set default by hands

	// Initialize OSC

	if ((RCON & RCON_STATUS) == 0)
	{ // Another program clears RCON bits
	} else { /* No programms are in flash */ }

	uart_init(); // Initialize UART

	for(;;) { // Main loop

		// Check UART events:
		if (UART_IS_RXFLAG(UART_USED)) uart_rx();
		if (UART_IS_ERFLAG(UART_USED)) uart_er();
		if (UART_IS_TXFLAG(UART_USED)) uart_tx();

		// Overrun receiver buffer
		if (buf.nrx >= RXBUF_SIZE) ++buf.err;

		// On any error - reset UART
		if (buf.err != 0) uart_init(); 

		for (; buf.pos < buf.nrx; ++buf.pos)
		{ // Try to find '\n' or ZERO character
			if ((buf.rxd[buf.pos] == '\n') ||
				(buf.rxd[buf.pos] == 0)) break;
		}

		if (buf.pos == buf.nrx) continue;

		// At this point we have a new command line

		// buf.pos points to '\n', '\r\n' or ZERO
		if ((buf.rxd[buf.pos] != 0) && (buf.pos > 0)
			&& (buf.rxd[buf.pos-1] == '\r')) --buf.pos;

		if (buf.rxd[0] == '#') trm_command();
		else if (buf.rxd[0] == ':') hex_command();
		else ++buf.err;  // Translate command

		// On any error - reset UART
		if (buf.err != 0) uart_init();
		else { buf.nrx = 0; buf.pos = 0; } // Reset buffer

		__asm__ volatile ("CLRWDT\n"); // Reset WDT
	} // Main loop

	return(0); /* Never return */
}
