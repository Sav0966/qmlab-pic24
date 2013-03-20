#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <clock.h>
#include <spimui.h>
#include <spisui.h>

#define SPI_MODE		(S_CKP | S_1000)

#ifndef ARSIZE
#define ARSIZE(buf) (sizeof(buf)/sizeof(buf[0]))
#endif

static int stage = 0; // Test stage

//static unsigned char _buf[1024];

void spi_test(void)
{ // Called from Main Loop more often than once per 10 ms

	if ((sys_clock() & 0x3F) == 0) {
	 // Once per 0.64 seccond test SPI
		if (!SPI_IS_INIT(SPI_MASTER)) {
			SPI_EMASTER_INIT(SPI_MASTER,
				SPI_MODE, SPI_EN | S_TXI_READY, 2);

			// Map SPI2 SDI and SCK to SPI1 SDO, SCK pins

			SPI_ESLAVE_INIT(SPI_SLAVE, /* Disable SDO */
				SPI_MODE | S_DISSDO, SPI_EN | S_RXI_ANY, 2);

			stage = 0; // Start test
		}
	}

	if (!SPI_IS_INIT(SPI_MASTER) ||
		!SPI_IS_INIT(SPI_SLAVE)) return;

	switch(stage) {
		case 0:
			SPI_WRITE(SPI_MASTER, 0x9F);
			++stage; break; // Next test

		case 1:
			__asm__ volatile ("nop\nnop\nnop"); // breakpoint
			++stage; break; // Next test

		case 2: // Overrun transmitter FIFO

			__asm__ volatile ("nop\nnop\nnop"); // breakpoint
			++stage; break; // Next test

		case 3: // Try to overrun transmitter buffer
			
			++stage; break; // Next test

		case 4: // Receve transmitted string

			__asm__ volatile ("nop\nnop\nnop"); // breakpoint
			++stage; break; // Next test

		case 5:	++stage; break; // Skip it
		case 6: // RX buffer must be empty

			__asm__ volatile ("nop\nnop\nnop"); // breakpoint
			++stage; break; // Next test

		default:
//			SPI_PWOFF(SPI_MASTER); SPI_PWOFF(SPI_SLAVE);
			break;
	} // switch(stage)
} 
