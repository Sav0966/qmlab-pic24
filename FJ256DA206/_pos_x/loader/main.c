#include <p24Fxxxx.h>
#include <config.h>
#include <reset.h>
#include <uarts.h>
#include <pm.h>

#include "main.h"

extern __attribute__((space(prog))) int _resetPRI;

static unsigned long mk_long(unsigned char* str)
{
	union {
		unsigned long l;
		struct { char llo; char lhi;
				 char hlo; char hhi; } ch;
	} u;

	u.ch.llo = *str++; u.ch.lhi = *str++;
	u.ch.hlo = *str++; u.ch.hhi = *str;
	return (u.l);
}

int main(void)
{
	union { unsigned long l; unsigned char b[4]; } addr;
	/* volatile */ UARTBUF buf;

	pins_init(); // Initialize MCU pins first

	while (!IS_MCU_PROGRAMMED()); // Stay here
	// SIM doesn't clear SRbits.IPL, ICD2 clears it
	SET_CPU_IPL(MAIN_IPL); // Set default by hands

	// Initialize OSC

	if ((RCON & RCON_STATUS) == 0)
	{ // Another program clears RCON bits
	} else { /* No programms are in flash */ }

	uart_init(&buf); // Initialize UART
	hex_init(&buf); // Set invalid state

	for(;;) { // Main loop
		__asm__ volatile ("CLRWDT\n"); // Reset WDT

		// Check UART events:
		if (UART_IS_RXFLAG(UART_USED)) uart_rx(&buf);
		if (UART_IS_ERFLAG(UART_USED)) uart_er(&buf);
		if (UART_IS_TXFLAG(UART_USED)) uart_tx(&buf);

		// Overrun receiver buffer - set error
		if (buf.nrx >= RXBUF_SIZE) ++buf.err;

		// On any error - reset UART
		if (buf.err != 0) uart_init(&buf);

		if (buf.prog.p.pos > 0)
		{ // Flash is programmed
			continue; // RX-TX only
		}

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

		if (buf.rxd[0] == '#') trm_command(&buf);
		else if (buf.rxd[0] == ':') hex_command(&buf);
		else ++buf.err;  // Translate command

		// On command error - reset UART and start again
		if (buf.err != 0) { uart_init(&buf); continue; }

//		if (buf.err == 0)
		if (buf.prog.p.pos == 0)
		if (buf.prog.p.type == 0)
		{ // Programm flash
			addr.l = _user_addr(__builtin_tbladdress(&_resetPRI));

			if (_page_addr(addr.l) <= get_xaddr(&buf))
			{
				++buf.err; // Check address value
			}
			else
			{
				if (get_xaddr(&buf) == 0) {
					if (buf.prog.p.cb < 8) ++buf.err;
					else { // Change reset vector
						buf.prog.p.data[0] = addr.b[0];
						buf.prog.p.data[1] = addr.b[1];
						buf.prog.p.data[2] = 4; // goto
						buf.prog.p.data[3] = 0;
						buf.prog.p.data[4] = addr.b[2];
						buf.prog.p.data[5] = 0;
						buf.prog.p.data[6] = 0;
						buf.prog.p.data[7] = 0;
					}

					// Clear page 0
					if (!pm_erase_page(0)) ++buf.err;
				}
			}

			addr.l = _user_addr(get_xaddr(&buf));

			if (buf.err == 0)
			{ // Clear current page
				if (_page_addr(addr.l) != 0)
				if (buf.page != _page_addr(addr.l))
				{
					buf.page = _page_addr(addr.l);
					if (!pm_erase_page(buf.page)) ++buf.err;
				}
			}
			else if (addr.l >= // Don't erase last page
					_user_addr(__builtin_tbladdress(&_CONFIG4)))
					--buf.err; // Check config words later

			if (buf.err == 0)
			{ // Write current block of data
				for (; buf.err < buf.prog.p.cb; buf.err += 4)
				{
					addr.l = pm_write_pword(addr.l,
						mk_long(&buf.prog.p.data[buf.err]));
					if (addr.l == 0) break; // Writting error
				}

				if (buf.err == buf.prog.p.cb) buf.err = 0;
			}
		} // Programm

		if (buf.err == 0) // Answer OK
		{ buf.txd[0] = 'O'; buf.txd[1] = 'K'; }
		else // Answer ER
		{ buf.txd[0] = 'E'; buf.txd[1] = 'R'; }

		buf.err = 0; // Clear error
		buf.txd[2] = buf.rxd[buf.pos]; buf.tsiz = 3;

		if (buf.txd[2] == '\r') // for '\r\n' terminator
		{ buf.txd[3] = buf.rxd[buf.pos+1]; buf.tsiz = 4; }

		buf.ntx = 0;
		UART_SET_TXFLAG(UART_USED); // Start transmition

		buf.nrx = 0; buf.pos = 0; // Reset RX buffer
	} // Main loop

	return(0); /* Never return */
}
