#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <clock.h>
#include <spimui.h>
#include <spisui.h>

#define SPI_MODE		(S_CKP | S_4000)

#ifndef ARSIZE
#define ARSIZE(buf) (sizeof(buf)/sizeof(buf[0]))
#endif

static int stage = 0; // Test stage
static int cnt, i;
extern volatile int len;

unsigned char _buf[1024];

int spi_send(unsigned char* buf, int n);

void spi_test(void)
{ // Called from Main Loop more often than once per 10 ms

	if ((sys_clock() & 0x3F) == 0) {
	 // Once per 0.64 seccond test SPI
		if (!SPI_IS_INIT(SPI_MASTER)) {
			SPI_EMASTER_INIT(SPI_MASTER,
				SPI_MODE, SPI_EN | S_RXI_ANY, 2);

//			SPI_ESLAVE_INIT(SPI_SLAVE, /* Disable SDO */
//				SPI_MODE, SPI_EN | S_RXI_ANY, 3);

			stage = 0; // Start test
		}
	}

	if (!SPI_IS_INIT(SPI_MASTER) /*||
		!SPI_IS_INIT(SPI_SLAVE)*/) return;

if (SPI_TX_COUNT(SPI_MASTER) > 4)
	__asm__ volatile ("nop\nnop"); // breakpoint


	switch(stage) {
		case 0:
			{ i = 0; for (; i < 512; i++) _buf[i] = i; }
			cnt = 0; spi_send(_buf, 512);

			++stage; break; // Next test

		case 1:
			{ i = SPI_TX_COUNT(SPI_MASTER);
					if (i > cnt) cnt = i; } // Save it
			for (i = 0; len > 0; i++); i *=7;
			__asm__ volatile ("nop\nnop"); // breakpoint
			++stage; break; // Next test

		case 2: // Overrun transmitter FIFO

			__asm__ volatile ("nop\nnop"); // breakpoint
			++stage; break; // Next test

		case 3: // Try to overrun transmitter buffer
			
			++stage; break; // Next test

		case 4: // Receve transmitted string

			__asm__ volatile ("nop\nnop"); // breakpoint
			++stage; break; // Next test

		case 5:	++stage; break; // Skip it
		case 6: // RX buffer must be empty

			__asm__ volatile ("nop\nnop"); // breakpoint
			++stage; break; // Next test

		default:
//			SPI_PWOFF(SPI_MASTER); SPI_PWOFF(SPI_SLAVE);
			break;
	} // switch(stage)
} 
