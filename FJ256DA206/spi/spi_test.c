#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <clock.h>
#include <spi.h>

#include <stdio.h>		// EOF

#define SPI_USED		1	// Checked SPI module

#ifndef ARSIZE
#define ARSIZE(buf) (sizeof(buf)/sizeof(buf[0]))
#endif

//static unsigned char _buf[64];
//static int _ch, _cnt; // Temp vars
static int stage = 0; // Test stage

void SPI_INTFUNC(SPI_USED)(void)
{

}

void SPI_ERR_INTFUNC(SPI_USED)(void)
{

}

void spi_test(void)
{ // Called from Main Loop more often than once per 10 ms

	if (!SPI_IS_INIT(SPI_USED)) {
		if (sys_clock() & 0x3F) return;
		// Once per 0.64 seccond test SPI

		SPI_EMASTER_INIT(SPI_USED,
			S_CKP | S_1000, SPI_EN | S_TXI_READY, 1);
		stage = 0; // Start test
	}

	switch(stage) {
		case 0:

			++stage; break; // Next test

		case 1: // BREAK sequence receiving

			__asm__ volatile ("nop\nnop\nnop"); // breakpoint
			++stage; break; // Next test

		case 2: TRACE("Overrun transmitter FIFO");

			__asm__ volatile ("nop\nnop\nnop"); // breakpoint
			++stage; break; // Next test

		case 3: TRACE("Try to overrun transmitter buffer");
			
			++stage; break; // Next test

		case 4: // Receve transmitted string

			__asm__ volatile ("nop\nnop\nnop"); // breakpoint
			++stage; break; // Next test

		case 5:	++stage; break; // Skip it
		case 6: // RX buffer must be empty

			__asm__ volatile ("nop\nnop\nnop"); // breakpoint
			++stage; break; // Next test

		default: SPI_PWOFF(SPI_USED); break;
	} // switch(stage)
} 
