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
static int i, min, max, cnt;
extern volatile int len;

unsigned char _buf[4096];

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

	switch(stage) {
		case 0:
			for (i = 0; i < 1024+8; i++) _buf[i] = i;
			cnt = 0; min = 0x7FFF; max = 0;

			spi_send(_buf, 1024+8);

			while (len > 8) {
				i = SPISTAT(SPI_MASTER) & 0x702;
				if (i < min) min = i;
				if (i > max) max = i;
				cnt += 23; // ~23 clk
			}

			++stage; break; // Next test

		case 1:
			for (i = 0; i < 1024+8; i++)	
			{ ASSERT(_buf[i] == (unsigned char)i); }

			// View and analyse min, max and cnt
			__asm__ volatile ("nop\nnop"); // breakpoint
			++stage; break; // Next test

		default:
//			SPI_PWOFF(SPI_MASTER); SPI_PWOFF(SPI_SLAVE);
			break;
	} // switch(stage)
} 
