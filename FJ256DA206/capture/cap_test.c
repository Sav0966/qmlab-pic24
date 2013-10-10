#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <timers.h>
#include <clock.h>
#include <refo.h>

#include <math.h>

#include "pmeter.h"

#define BUF_SIZE	3026
PM_BUFFER(IC_USED, BUF_SIZE+1); // +1 for checking
DECL_PMETER_UI(IC_USED); // Declare user interfase

// QMC calculation constants sqrt(pi)/(2 * n))
#define _pi		3.1415926535897932384626433832795
#define _rpi	1.7724538509055160272981674833411
#define QMC_PRE4	(float)(2*_rpi)		// n = 4
#define QMC_FALL	(float)(_rpi/2)		// n = 1
#define QMC_EDGE	(float)(_rpi)		// n = 1/2
// Number of periods into three correlation times
#define _CT3_	24 

static unsigned long long sum;
static unsigned long num;
static double period, qmc;

static int i, clk, stage = 0; // Test stage
#ifdef __DEBUG
static int tim;
#endif

void cap_test(void)
{ // Called from Main Loop more often than once per 10 ms

	if (!PM_IS_INIT(IC_USED)) {
		PM_INIT(IC_USED, IC_RXI_3DATA, 0, SYSCLK_IPL+1);
		PM_BUF(IC_USED, BUF_SIZE) = 0xABCD; // Check ptr
		stage = 0; // Start test
	}

	if (!PM_IS_INIT(IC_USED)) return;

	switch(stage) {
		case 0: // Start sampling

			refo_div(RODIV_8192);
			refo_on(); // == 256 us period on REFO
			PM_START(IC_USED, BUF_SIZE, ICM_RAISE);

			++stage; break; // Next test

		case 1: // Wait while buffer is not full

#ifdef __MPLAB_SIM
			{ // Fill buffer by software (SIM)
				static char __time__[] = __TIME__; // Rnd()

				_IC_(IC_USED, pcur).peds = _IC_(IC_USED, buf);
				_IC_(IC_USED, pend) =_IC_(IC_USED, pcur).p.addr +
											((BUF_SIZE) - 1);

				for (i = 0; __time__[i] != 0; ++i)
					PM_BUF(IC_USED, 0) += __time__[i];

				pm_math_init(IC_USED); // No data
				ASSERT(!pm_math23_start(IC_USED,_CT3_, 0));

				for (i = 1; i < _CT3_; ++i) // Not enough data
				 PM_BUF(IC_USED, i) = PM_BUF(IC_USED, i-1)+0x1000;
				_IC_(IC_USED, pcur).p.addr =
									(int*)&PM_BUF(IC_USED, _CT3_-1);

				ASSERT((_CT3_-3) ==
						pm_math23_start(IC_USED, _CT3_, 4000));
				ASSERT(!pm_math23_task(IC_USED));

				for (i = 1; i < BUF_SIZE-1; ++i) {
					PM_BUF(IC_USED, i) = PM_BUF(IC_USED, i-1)+0x1000; }
				// Last buffer data must be overwriten, i = BUF_SIZ-1
				PM_BUF(IC_USED, i) = PM_BUF(IC_USED, i-1) + 0x2000;

				_IC_(IC_USED, pcur).p.addr = (int*)_IC_(IC_USED, pend);
			}
#endif //__MPLAB_SIM

			if (!PM_IS_OBUF(IC_USED)) break;

			++stage; break; // Next test

		case 2: // + 1 event (overwrite last cell)
			++stage; break;

		case 3: // Stop and analyse buffer

			ASSERT(!PM_IS_OERR(IC_USED));
			PM_STOP(IC_USED); // Stop sampling
			refo_off(); // Turn off REF output

			for (i = 1; i < BUF_SIZE-1; ++i)
				ASSERT(((unsigned)PM_BUF(IC_USED, i) - // = 256 us
						(unsigned)PM_BUF(IC_USED, i-1)) == 0x1000);

			// Last buffer data must be overwriten
			ASSERT(((unsigned)PM_BUF(IC_USED, i) - // i = BUF_SIZ-1
						(unsigned)PM_BUF(IC_USED, i-1)) != 0x1000);

			// Extra buffer data must not be changed
			ASSERT(PM_BUF(IC_USED, BUF_SIZE) == 0xABCD);

			__asm__ volatile ("nop\nnop");
			++stage; break; // Next test

		case 4: // Methematics (statistical algorithm)

			pm_math_init(IC_USED);

			PROFILE_START(SYS_TIMER);
				// Start methematics with 2 ms timeout
				if (_CT3_ != pm_math23_start(IC_USED, _CT3_, 4000))
				{ // Wait time-out error
					__asm__ volatile ("nop\nnop");
				}
			PROFILE_END(SYS_TIMER, tim);
			// 72 us + 5.12 us/period

			__asm__ volatile ("nop\nnop");

			clk = sys_clock();
			do { // Prepare statistics
				PROFILE_START(SYS_TIMER);
					pm_math23_task(IC_USED);
				PROFILE_END(SYS_TIMER, tim);
			} while PM_IS_RUN(IC_USED);
			// 5.32 us per one period
			clk -= sys_clock();

			__asm__ volatile ("nop\nnop");

			{ // Calculate average period (~T~)

				PROFILE_START(SYS_TIMER);
					// ~T~ = S / (2*_N1*(_N1+1))
					sum = pm_math23_sum(IC_USED);
					num = pm_math23_num(IC_USED);
					if (num) period = (double)sum / (num*(num+1));
					else period = 0;
				PROFILE_END(SYS_TIMER, tim); // ~135 us

				ASSERT(period == (2.0 * 0x1000));

				__asm__ volatile ("nop\nnop");

				qmc = pm_math23_qmc(IC_USED);
				ASSERT(qmc == 0); // No STD

				PROFILE_START(SYS_TIMER);
					if (num >= _CT3_) qmc /= (1/QMC_FALL) *
						((3*num+1 -_CT3_) * num) * sqrt(num+1);
					else qmc = 65535;
				PROFILE_END(SYS_TIMER, tim); // ~150 us

				__asm__ volatile ("nop\nnop");
			}

			++stage; break; // Next test

		default:
			PM_DONE(IC_USED);
			break;
	} // switch(stage)
}

// Check names
#undef IC_USED
#define IC_USED	1
//#include "pmeter.c"
