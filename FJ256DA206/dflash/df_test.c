#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <timers.h>
#include <clock.h>
#include <spimui.h>

#ifndef	SPI_MASTER
#define	SPI_MASTER	1
#endif

DECL_SPIM_UI(SPI_MASTER);

#define PACKET_SIZE	4096	// Packet size

#define _SPI_MODE(speed)	(S_MSTEN | S_CKP | S_##speed)
#define SPI_MODE			_SPI_MODE(1000)

#ifndef ARSIZE
#define ARSIZE(buf) (sizeof(buf)/sizeof(buf[0]))
#endif

static const int _mode[9] = { 0,
 _SPI_MODE(1000), _SPI_MODE(2000), 0,
 _SPI_MODE(4000), 0, 0, 0, _SPI_MODE(8000) };

static int stage = 0; // Test stage
static int speed = 1; // Speed in MHz

//int  packet_siz = PACKET_SIZE;
//static char buf[PACKET_SIZE];
//static char _extra_buf[8]; // Check pointers

//static int i, min, max, percent, over;
//static unsigned long ltime, lfree, lbusy;

#define SPI_MIN_CLK(siz, speed)\
	(16 * ((8L * siz) / speed))

void spi_test(void)
{ // Called from Main Loop more often than once per 10 ms

	if ((sys_clock() & 0x3F) == 0) {
	 // Once per 0.64 seccond test SPI
		if (!spim_isinit(SPI_MASTER)) {
			spim_init(SPI_MASTER, SPI_MODE, SYSCLK_IPL+1);
			stage = 0; // Start test
		}
	}

	if (!spim_isinit(SPI_MASTER)) return;

	switch(stage) {
		case 0:

			++stage; break; // Next test

		case 1:

			__asm__ volatile ("nop\nnop");
			++stage; break; // Next test

		default: // Increase SPI speed
			stage = 0;
			speed *= 2; if (speed > 8) speed = 1;
			SPI_DISABLE(SPI_MASTER); // Set speed
			SPICON1(SPI_MASTER) = _mode[speed];
			SPI_ENABLE(SPI_MASTER);
			break;
	} // switch(stage)
}

#include "spimui.c"
