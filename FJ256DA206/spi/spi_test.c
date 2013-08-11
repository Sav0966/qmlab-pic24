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

static const int _mode[9] = { 0,
 _SPI_MODE(1000), _SPI_MODE(2000), 0,
 _SPI_MODE(4000), 0, 0, 0, _SPI_MODE(8000) };

static int stage = 0; // Test stage
static int speed = 1; // Speed in MHz

int  packet_siz = PACKET_SIZE;
static char buf[PACKET_SIZE];
static char _extra_buf[8]; // Check pointers

static int i, min, max, percent, over;
static unsigned long ltime, lfree, lbusy;

#define SPI_MIN_CLK(siz, speed)\
	(16 * ((8L * siz) / speed))

void spi_test(void)
{ // Called from Main Loop more often than once per 10 ms

	if ((sys_clock() & 0x3F) == 0) {
	 // Once per 0.64 seccond test SPI
		if (!spim_isinit(SPI_MASTER)) {
			spim_init(SPI_MASTER, SPI_MODE, SYSCLK_IPL+1);

//			SPI_ESLAVE_INIT(SPI_SLAVE, /* Disable SDO */
//				SPI_MODE, SPI_EN | S_RXI_ANY, 3);

			stage = 0; // Start test
		}
	}

	if (!spim_isinit(SPI_MASTER) /*||
		!SPI_IS_INIT(SPI_SLAVE)*/) return;

	switch(stage) {
		case 0: // Prepare buffer: spim_shift() & spim_load()
			for (i = 0; i < ARSIZE(buf); i++) buf[i] = i;
			++stage; break; // Next test

		case 1: // Profile
			i = sys_clock(); PROFILE_START(SYS_TIMER);

			spim_shift(SPI_MASTER, buf, packet_siz);

			while (!spim_ready(SPI_MASTER));
			PROFILE_END(SYS_TIMER, ltime);
			i = sys_clock() - i;

			ltime *= 8; // Timer1 @ 2MHz
			ltime += i * (16 * 10000L);

			++stage; break; // Next test

		case 2: lfree = 0; // Free time

			spim_shift(SPI_MASTER, buf, packet_siz);

			while (!spim_ready(SPI_MASTER)) lfree++;
			lfree *= 11; // 11 clk in loop

			++stage; break; // Next test

		case 3: // FIFO size

			min = 0x0FFF; max = 0;
			VERIFY(packet_siz ==
				spim_shift(SPI_MASTER, buf, packet_siz));

			while (!spim_ready(SPI_MASTER)) {
				i = SPISTAT(SPI_MASTER) & 0x0702;
				if (i < min) min = i;
				if (i > max) max = i; }

			++stage; break; // Next test

		case 4: // View and analyse (spim_shift)

			lbusy = ltime - lfree;
			percent = (int)((100*lfree)/ltime);

			// Check of overruning buffer boundaries
			for (i = 0; i < ARSIZE(_extra_buf); i++)
			{ ASSERT(_extra_buf[i] == 0); }

			for (i = 0; i < packet_siz; i++)	
			{ ASSERT(buf[i] == (char)i); } // Check buf

			ASSERT(!spim_iserr(SPI_MASTER));

			over = (int)(ltime - SPI_MIN_CLK(packet_siz, speed));

			// No overtime
			// Free CPU time spim_shift/_load(4K packet):
			// 87% 1MHz, 76% @ 2MHz, 57% @ 4MHz, 26% @ 8MHz

			__asm__ volatile ("nop\nnop");
			++stage; break; // Next test

		case 5: // Test spim_load()
			spim_load(SPI_MASTER, buf, packet_siz);

			while (!spim_ready(SPI_MASTER));
			for (i = 0; i < packet_siz; i++)	
			{ ASSERT(buf[i] == 0); } // Check buf

			_S2DO = 1;
			spim_load(SPI_MASTER, buf, ARSIZE(buf));

			while (!spim_ready(SPI_MASTER));
			for (i = 0; i < packet_siz; i++)	
			{ ASSERT(buf[i] == -1); } // Check buf

			_S2DO = 0;

			__asm__ volatile ("nop\nnop");
			++stage; break; // Next test

		case 6: // Clear buffer: test spim_send()
			for (i = 0; i < ARSIZE(buf); i++) buf[i] = 0;
			++stage; break; // Next test

		case 7: // Profile
			i = sys_clock(); PROFILE_START(SYS_TIMER);

			spim_send(SPI_MASTER, buf, packet_siz);

			while (!spim_ready(SPI_MASTER));
			PROFILE_END(SYS_TIMER, ltime);
			i = sys_clock() - i;

			ltime *= 8; // Timer1 @ 2MHz
			ltime += i * (16 * 10000L);

			++stage; break; // Next test

		case 8: lfree = 0; // Free time

			_S2DO = 1; // Try to send 0xFF
			SPI_DISABLE(SPI_MASTER); // Use IO
			SPICON1bits(SPI_MASTER).DISSDO = 1;
			SPI_ENABLE(SPI_MASTER);

			spim_send(SPI_MASTER, buf, packet_siz);

			while (!spim_ready(SPI_MASTER)) lfree++;
			lfree *= 11; // 11 clk in loop

			_S2DO = 0; // Restore state
			SPI_DISABLE(SPI_MASTER); // Use SDO
			SPICON1bits(SPI_MASTER).DISSDO = 0;
			SPI_ENABLE(SPI_MASTER);

			++stage; break; // Next test

		case 9: // View and analyse

			lbusy = ltime - lfree;
			percent = (int)((100*lfree)/ltime);

			// Check of overruning buffer boundaries
			for (i = 0; i < ARSIZE(_extra_buf); i++)
			{ ASSERT(_extra_buf[i] == 0); }

			for (i = 0; i < packet_siz; i++)	
			{ ASSERT(buf[i] == 0); } // Check buf

			ASSERT(!spim_iserr(SPI_MASTER));

			over = (int)(ltime - SPI_MIN_CLK(packet_siz, speed));

			// No overtime
			// Free CPU time spim_shift/_load(4K packet):
			// 87% 1MHz, 76% @ 2MHz, 58% @ 4MHz, 29% @ 8MHz

			__asm__ volatile ("nop\nnop");
			++stage; break; // Next test

		case 10: // Test for any length of packet
			{
				int ipl_T1 = TIMER_GET_IPL(SYS_TIMER);
				int ipl_SPI = SPI_GET_IPL(SPI_MASTER);
				TIMER_SET_IPL(SYS_TIMER, ipl_SPI); 
				SPI_SET_IPL(SPI_MASTER, ipl_T1);
			} // Timer1 IPL > SPI IPL

			for (packet_siz = 1; packet_siz < PACKET_SIZE;
				++packet_siz) {

				for (i = 0; i < packet_siz; i++) buf[i] = i;

				VERIFY(packet_siz == 
					spim_shift(SPI_MASTER, buf, packet_siz));
				while (!spim_ready(SPI_MASTER));

				ASSERT(!spim_iserr(SPI_MASTER));

				for (i = 0; i < packet_siz; i++)	
				{ ASSERT(buf[i] == (char)i); }
				for (; i < ARSIZE(buf); i++)
				{ ASSERT(buf[i] == 0); } // Check buf
			}

			{
				int ipl_T1 = TIMER_GET_IPL(SYS_TIMER);
				int ipl_SPI = SPI_GET_IPL(SPI_MASTER);
				TIMER_SET_IPL(SYS_TIMER, ipl_SPI); 
				SPI_SET_IPL(SPI_MASTER, ipl_T1);
			} // Restore IPLs

			__asm__ volatile ("nop\nnop");

			++stage; break; // Next test

		default: // Increase SPI speed
			stage = 0;
			speed *= 2; if (speed > 8) speed = 1;
			SPI_DISABLE(SPI_MASTER); // Set speed
			SPICON1(SPI_MASTER) = _mode[speed];
			SPI_ENABLE(SPI_MASTER);
//			spim_pwoff(SPI_MASTER); SPI_PWOFF(SPI_SLAVE);
			break;
	} // switch(stage)
}

#undef	SPI_MASTER
#define	SPI_MASTER	1
#include "spimui.c"
