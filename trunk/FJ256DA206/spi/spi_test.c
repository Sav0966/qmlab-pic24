#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <timers.h>
#include <clock.h>
#include <spimui.h>
#include <spisui.h>

#ifndef SPI_SELECT
#define __SPI_CS(n)	SPI##n##_CS
#define _SPI_CS(n)	__SPI_CS(n)
extern volatile int _SPI_CS(SPI_MASTER) __attribute__((near));
#define SPI_READY(n)	(_SPI_CS(n))
#endif

int spi_msend(unsigned char* buf, int n);

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

static unsigned char buf[1024];
static unsigned char _test_only[8];

static int i, min, max, percent;
static long ltime, lfree, lbusy;

#define SPI_MIN_CLK(speed)\
 (16 * ((8 * ARSIZE(buf)) / speed))

void spi_test(void)
{ // Called from Main Loop more often than once per 10 ms

	if ((sys_clock() & 0x3F) == 0) {
	 // Once per 0.64 seccond test SPI
		if (!SPI_IS_INIT(SPI_MASTER)) {
			SPI_EMASTER_INIT(SPI_MASTER,
				SPI_MODE, SPI_EN | S_TXI_END, 2);

//			SPI_ESLAVE_INIT(SPI_SLAVE, /* Disable SDO */
//				SPI_MODE, SPI_EN | S_RXI_ANY, 3);

			stage = 0; // Start test
		}

		SPI_DISABLE(SPI_MASTER); // Set speed
		SPICON1(SPI_MASTER) = _mode[speed];
		SPI_ENABLE(SPI_MASTER);
	}

	if (!SPI_IS_INIT(SPI_MASTER) /*||
		!SPI_IS_INIT(SPI_SLAVE)*/) return;

	switch(stage) {
		case 0: // Prepare buffer
			for (i = 0; i < ARSIZE(buf); i++) buf[i] = i;
			++stage; break; // Next test

		case 1: PROFILE_START(1); // Profile

			spi_msend(buf, ARSIZE(buf));

			while (!SPI_READY(SPI_MASTER));
			PROFILE_END(1, ltime);
			ltime *= 8; // Timer1 @ 2MHz

			++stage; break; // Next test

		case 2: lfree = 0; // Free time

			spi_msend(buf, ARSIZE(buf));

			while (!SPI_READY(SPI_MASTER)) lfree++;
			lfree *= 10; // 10 clk

			++stage; break; // Next test

		case 3: // FIFO size

			min = 0x0FFF; max = 0;
			VERIFY(ARSIZE(buf) ==
				spi_msend(buf, ARSIZE(buf)));

			while (!SPI_READY(SPI_MASTER)) {
				i = SPISTAT(SPI_MASTER) & 0x0702;
				if (i < min) min = i;
				if (i > max) max = i; }

			++stage; break; // Next test

		case 4: // View and analyse
			lbusy = ltime - lfree;
			percent = (int)((100*lfree)/ltime);
			i = (int)(ltime - SPI_MIN_CLK(speed));

			for (i = 0; i < ARSIZE(buf); i++)	
			{ ASSERT(buf[i] == (unsigned char)i); }

			__asm__ volatile ("nop\nnop");
			++stage; break; // Next test

		default: // Increase SPI speed
			speed *= 2; if (speed > 8) speed = 1;
			SPI_PWOFF(SPI_MASTER); // SPI_PWOFF(SPI_SLAVE);
			break;
	} // switch(stage)
}
