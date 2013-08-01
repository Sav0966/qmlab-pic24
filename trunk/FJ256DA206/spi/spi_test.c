#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <timers.h>
#include <clock.h>
#include <spimui.h>
#include <spisui.h>

extern volatile int len;
int spi_msend(unsigned char* buf, int n);
int spi_mload(unsigned char* buf, int n);

#define _SPI_MODE(speed)	(S_MSTEN | S_CKP | S_##speed)
#define SPI_MODE			_SPI_MODE(1000)

#ifndef ARSIZE
#define ARSIZE(buf) (sizeof(buf)/sizeof(buf[0]))
#endif

static unsigned char buf[1024];

static int stage = 0; // Test stage
static int speed = 1; // Speed in MHz

static const int _mode[9] = { 0,
 _SPI_MODE(1000), _SPI_MODE(2000), 0,
 _SPI_MODE(4000), 0, 0, 0, _SPI_MODE(8000) };

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
				SPI_MODE, SPI_EN | S_RXI_ANY, 2);

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

			while (len);
			while (!SPI_SR_EMPTY(SPI_MASTER));
			PROFILE_END(1, ltime);
			ltime *= 8; // Timer1 @ 2MHz

			ASSERT(!SPI_IS_ENABLE_INT(SPI_MASTER));

			++stage; break; // Next test

		case 2: lfree = 0; // Free time

			spi_msend(buf, ARSIZE(buf));

			while (len)
			{ lfree++;__asm__ volatile ("nop\nnop"); }
			while (!SPI_SR_EMPTY(SPI_MASTER)) lfree++;
			lfree *= 12; // 12 clk

			ASSERT(!SPI_IS_ENABLE_INT(SPI_MASTER));

			++stage; break; // Next test

		case 3: // FIFO size

			min = 0xFF; max = 0;
			spi_msend(buf, ARSIZE(buf));

			while (len) {
				i = SPISTAT(SPI_MASTER) & 0x702;
				if (i < min) min = i;
				if (i > max) max = i; }
			while (!SPI_SR_EMPTY(SPI_MASTER));

			ASSERT(!SPI_IS_ENABLE_INT(SPI_MASTER));

			++stage; break; // Next test

		case 4: // View and analyse
			for (i = 0; i < ARSIZE(buf); i++)	
			{ ASSERT(buf[i] == (unsigned char)i); }

			lbusy = ltime - lfree;
			percent = (int)((100*lfree)/ltime);
			i = (int)(ltime - SPI_MIN_CLK(speed));

			__asm__ volatile ("nop\nnop");
			++stage; break; // Next test

		case 5: PROFILE_START(1); // Profile

			spi_mload(buf, ARSIZE(buf));

			while (len);
			while (!SPI_SR_EMPTY(SPI_MASTER));
			PROFILE_END(1, ltime);
			ltime *= 8; // Timer1 @ 2MHz

			ASSERT(!SPI_IS_ENABLE_INT(SPI_MASTER));

			++stage; break; // Next test

		case 6: // View and analyse
//			for (i = 0; i < ARSIZE(buf); i++)	
//			{ ASSERT(buf[i] == 0x68); } // == SPIBUF

			lbusy = ltime - lfree;
			percent = (int)((100*lfree)/ltime);
			i = (int)(ltime - SPI_MIN_CLK(speed));

			__asm__ volatile ("nop\nnop");
			++stage; break; // Next test

		default: // Increase SPI speed
			speed *= 2; if (speed > 8) speed = 1;
			SPI_PWOFF(SPI_MASTER); // SPI_PWOFF(SPI_SLAVE);
			break;
	} // switch(stage)
} 
