#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <timers.h>
#include <clock.h>
#include <refo.h>

#include <mathlib.h>

#include "pmeter.h"

#define BUF_SIZE	3026
PM_BUFFER(IC_USED, BUF_SIZE+1); // +1 for checking
DECL_PMETER_UI(IC_USED); // Declare user interfase

// Number of periods into three correlation times
#define _CT3_	24 

static unsigned long num, err;
static unsigned long long sum;
static double period;
float qmc;

static char __time__[] = __TIME__; // Rand()
static int seed __attribute__((persistent));

static int i, clk, stage = 0; // Test stage

#ifdef __DEBUG
static int tim;
#endif

#ifdef __MPLAB_SIM
static float qmc_ave;
static double std, mean;
static void fill_buffer(int period, int size)
{
	int n;
	for (n = 1; n < size; ++n)
	PM_BUF(IC_USED, n) = PM_BUF(IC_USED, n-1) + period;
}
#endif

static double math23_dperiod(unsigned long long s, unsigned int n)
{ // ~T~ = S / (2*_N1*(_N1+1)), return doubled period (n = (1/3)N)
	return((n != 0)? (double)s / ((unsigned long)n * (n+1)): 0.0);
}

static float math23_qmc(unsigned long s, unsigned int n)
{ // QMC calculation constants = sqrt(pi)/4
	return( ((float)(_rPI/4)) * (float)s /
	(((3 * (unsigned long)n + 1 -_CT3_) * n) * sqrtf(n + 1)) );
}

void cap_test(void)
{ // Called from Main Loop more often than once per 10 ms

	if (!PM_IS_INIT(IC_USED)) {
		PM_INIT(IC_USED, IC_RXI_3DATA, 0, SYSCLK_IPL+1);
		PM_BUF(IC_USED, BUF_SIZE) = 0xABCD; // Check ptr

		for (i = 0; __time__[i] != 0; ++i) seed +=__time__[i];
		m_srand(sys_clock() + seed); // Initialize rand()

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

#ifdef __MPLAB_SIM // Fill buffer by software (SIM)

			_IC_(IC_USED, pcur).peds = _IC_(IC_USED, buf);
			_IC_(IC_USED, pend) =_IC_(IC_USED, pcur).p.addr +
											((BUF_SIZE) - 1);

			pm_math_init(IC_USED); // No data
			ASSERT(!pm_math23_start(IC_USED,_CT3_, 0));

			PM_BUF(IC_USED, 0) = m_rand();
			fill_buffer(0x1000, _CT3_); // Not enough data
			_IC_(IC_USED, pcur).p.addr = (unsigned*)&PM_BUF(IC_USED, _CT3_-1);

			ASSERT(_CT3_ > pm_math23_start(IC_USED, _CT3_, 4000));
			ASSERT(!pm_math23_task(IC_USED));

			PM_BUF(IC_USED, 0) = m_rand();
			fill_buffer(0x1000, BUF_SIZE); // Fill buffer
			PM_BUF(IC_USED, BUF_SIZE-1) += 0x1000; // Overwrit it
			_IC_(IC_USED, pcur).p.addr = (unsigned*)_IC_(IC_USED, pend);

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
			PROFILE_START(SYS_TIMER);
				do { // Prepare statistics
						pm_math23_task(IC_USED);
				} while (PM_IS_RUN(IC_USED));
				pm_math23_task(IC_USED);
			PROFILE_END(SYS_TIMER, tim);
			// 5.32 us per one period
			clk -= sys_clock();

			__asm__ volatile ("nop\nnop");

			{ // Calculate average period (~T~)

				PROFILE_START(SYS_TIMER);
					// ~T~ = S / (2*_N1*(_N1+1))
					sum = pm_math23_sum(IC_USED);
					num = pm_math23_num(IC_USED);
					period = math23_dperiod(sum, num);
				PROFILE_END(SYS_TIMER, tim); // ~135 us

				ASSERT(period == (2.0 * 0x1000));

				__asm__ volatile ("nop\nnop");

				qmc = pm_math23_qmc(IC_USED);
				ASSERT(qmc == 0); // No STD

				PROFILE_START(SYS_TIMER);
					if (num >= _CT3_) qmc = math23_qmc(qmc, num);
					else qmc = 65535; // Maximum error value
				PROFILE_END(SYS_TIMER, tim); // ~70 us

				__asm__ volatile ("nop\nnop");
			}

			++stage; break; // Next test

		case 5: // Algorithm validation

#ifdef __MPLAB_SIM

			PM_BUF(IC_USED, 0) = m_rand();
			fill_buffer(0x1000, BUF_SIZE); // Fill buffer
			for (i = 1; i < BUF_SIZE; ++i) {
				int d = (int)m_grandf(0, 0x1000);
				if ((d < -0x200) || (d > 0x200))continue;
				PM_BUF(IC_USED, i) += d; // Add deviation
			}

			pm_math_init(IC_USED); err = 0;
			num = pm_math23_start(IC_USED, _CT3_, 4000);
			do {
				if (pm_math23_task(IC_USED) < 0)
				{ // Deviation error
					err = 1; break; 
				}
			} while (PM_IS_RUN(IC_USED));
			if (!err) pm_math23_task(IC_USED);

			period = math23_dperiod( // Doubled period
				pm_math23_sum(IC_USED), pm_math23_num(IC_USED));

			if (num >= _CT3_) qmc = math23_qmc(
				pm_math23_qmc(IC_USED), pm_math23_num(IC_USED));
			else qmc = 65535;

			{ // Compute Median and STD
				static double p[15]; static float q[15];
				static int j = 0; double d[15];

				if (qmc != 65535) {
					p[j] = (period / 2);	// Real periods
					q[j] = qmc;			// Store QMC values
					if (++j >= ARSIZE(p)) { j = 0; mean = period; }
				}

				if (mean != 0) {
					mean = 0; std = 0; qmc_ave = 0;
					for (i = 0; i < ARSIZE(p); ++i) mean += p[i];
					mean /= ARSIZE(p);

					for (i = 0; i < ARSIZE(p); ++i) d[i] = p[i] - mean;
					for (i = 0; i < ARSIZE(p); ++i) std += d[i]*d[i];
					std = sqrt(std/ARSIZE(p));

					for (i = 0; i < ARSIZE(q); ++i) qmc_ave += q[i];
					qmc_ave /= ARSIZE(q);
				}
			}

			__asm__ volatile ("nop\nnop");

#endif //__MPLAB_SIM

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
