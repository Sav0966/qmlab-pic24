#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <timers.h>
#include <clock.h>
#include <spimui.h>
#include <spisui.h>

DECL_SPIM_UI(SPI_MASTER);

#define PACKET_SIZE	4096	// Packet size

#define _SPI_MODE(speed)	(S_MSTEN | S_CKP | S_##speed)
#define SPI_MODE			_SPI_MODE(1000)

#ifndef ARSIZE
#define ARSIZE(buf) (sizeof(buf)/sizeof(buf[0]))
#endif

static int stage = 0; // Test stage
static int speed = 1; // Speed in MHz

static const int _mode[9] = { 0,
 _SPI_MODE(1000), _SPI_MODE(2000), 0,
 _SPI_MODE(4000), 0, 0, 0, _SPI_MODE(8000) };

static char buf[PACKET_SIZE];
static char _extra_buf[8]; // Check pointers

static int i, min, max, percent;
static unsigned long ltime, lfree, lbusy;

#define SPI_MIN_CLK(speed)\
 (16 * ((8 * ARSIZE(buf)) / speed))

void spi_test(void)
{ // Called from Main Loop more often than once per 10 ms

	if ((sys_clock() & 0x3F) == 0) {
	 // Once per 0.64 seccond test SPI
		if (!spim_isinit(SPI_MASTER)) {
			spim_init(SPI_MASTER, SPI_MODE, 2);

//			SPI_ESLAVE_INIT(SPI_SLAVE, /* Disable SDO */
//				SPI_MODE, SPI_EN | S_RXI_ANY, 3);

			stage = 0; // Start test
		}

		SPI_DISABLE(SPI_MASTER); // Set speed
		SPICON1(SPI_MASTER) = _mode[speed];
		SPI_ENABLE(SPI_MASTER);
	}

	if (!spim_isinit(SPI_MASTER) /*||
		!SPI_IS_INIT(SPI_SLAVE)*/) return;

	switch(stage) {
		case 0: // Prepare buffer

			for (i = 0; i < ARSIZE(buf); i++) buf[i] = i;
			++stage; break; // Next test

		case 1: // Profile
			i = sys_clock(); PROFILE_START(1);

			spim_send(SPI_MASTER, buf, ARSIZE(buf));

			while (!SPI_READY(SPI_MASTER));
			PROFILE_END(1, ltime);
			i = sys_clock() - i;

			ltime *= 8; // Timer1 @ 2MHz
			ltime += i * (16 * 10000L);

			++stage; break; // Next test

		case 2: lfree = 0; // Free time

			spim_send(SPI_MASTER, buf, ARSIZE(buf));

			while (!SPI_READY(SPI_MASTER)) lfree++;
			lfree *= 12; // 12 clk in loop

			++stage; break; // Next test

		case 3: // FIFO size

			min = 0x0FFF; max = 0;
			VERIFY(ARSIZE(buf) ==
				spim_send(SPI_MASTER, buf, ARSIZE(buf)));

			while (!SPI_READY(SPI_MASTER)) {
				i = SPISTAT(SPI_MASTER) & 0x0702;
				if (i < min) min = i;
				if (i > max) max = i; }

			++stage; break; // Next test

		case 4: // View and analyse

			// Check of overruning buffer boundaries
			for (i = 0; i < ARSIZE(_extra_buf); i++)	
			{ ASSERT(_extra_buf[i] == 0); }

			for (i = 0; i < ARSIZE(buf); i++)	
			{ ASSERT(buf[i] == (char)i); } // Check buf

			lbusy = ltime - lfree;
			percent = (int)((100*lfree)/ltime);
			i = (int)(ltime - SPI_MIN_CLK(speed));

			// 0,0% over and 88% free time @ 1MHz SPI (4K)
			// 0,0% over and 78% free time @ 4MHz SPI (4K)
			// 1,1% over and 53% free time @ 4MHz SPI (4K)
			// 6,5% over and 33% free time @ 8MHz SPI (4K)

			__asm__ volatile ("nop\nnop");
			++stage; break; // Next test

		default: // Increase SPI speed
			speed *= 2; if (speed > 8) speed = 1;
			spim_pwoff(SPI_MASTER); // SPI_PWOFF(SPI_SLAVE);
			break;
	} // switch(stage)
}

#undef	SPI_MASTER
#define	SPI_MASTER	1
//#include "spimui.c"
