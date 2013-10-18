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

static int err;
static unsigned int avep;
static unsigned long num;
static unsigned long long sum;
static double period;
static float qmc;

static char __time__[] = __TIME__; // Rand()
static int seed __attribute__((persistent));

int i, clk, stage = 0; // Test stage

#ifdef __DEBUG
static int tim;
#endif

#ifdef __MPLAB_SIM
static float qmc_ave;
static double std, mean;
static void fill_buffer(int period, int size)
{
	int n;
	for (PM_BUF(IC_USED, 0) = m_rand(), n = 1; n < size; ++n)
	PM_BUF(IC_USED, n) = PM_BUF(IC_USED, n - 1) + period;
}
#endif

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

			refo_div(RODIV_8192); avep = 0x1000;
			refo_on(); // == 256 us period on REFO

			PM_START(IC_USED, BUF_SIZE, ICM_FALL);

			if (PM_GET_MODE(IC_USED) == ICM_PRE4) avep *= 4;
			else if (PM_GET_MODE(IC_USED) == ICM_EDGE) avep /= 2;
			else if (PM_GET_MODE(IC_USED) == ICM_PRE16) ASSERT(0);

			++stage; break; // Next test

		case 1: // Wait while buffer is not full

#ifdef __MPLAB_SIM // Fill buffer by software (SIM)

			_IC_(IC_USED, pcur).peds = _IC_(IC_USED, buf);
			_IC_(IC_USED, pend) =_IC_(IC_USED, pcur).p.addr +
											((BUF_SIZE) - 1);

			pm_math_init(IC_USED); // No data
			ASSERT(!pm_math23_start(IC_USED,_CT3_, 0));

			fill_buffer(avep, _CT3_); // Not enough data
			_IC_(IC_USED, pcur).p.addr = (unsigned*)&PM_BUF(IC_USED, _CT3_-1);

			ASSERT(_CT3_ > pm_math23_start(IC_USED, _CT3_, 4000));
			ASSERT(!pm_math23_task(IC_USED));

			fill_buffer(avep, BUF_SIZE); // Fill buffer
			PM_BUF(IC_USED, BUF_SIZE-1) += avep; // Overwrit it
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
						(unsigned)PM_BUF(IC_USED, i-1)) == avep);

			// Last buffer data must be overwriten
			ASSERT(((unsigned)PM_BUF(IC_USED, i) - // i = BUF_SIZ-1
						(unsigned)PM_BUF(IC_USED, i-1)) != avep);

			// Extra buffer data must not be changed
			ASSERT(PM_BUF(IC_USED, BUF_SIZE) == 0xABCD);

			__asm__ volatile ("nop\nnop");
			++stage; break; // Next test

		case 4: // Methematics (statistical algorithm)

			pm_math_init(IC_USED);

			PROFILE_START(SYS_TIMER);
				// Start methematics with 2 ms timeout
				if (_CT3_ < pm_math23_start(IC_USED, _CT3_, 4000))
				{ // Wait time-out error
					err = 1; __asm__ volatile ("nop\nnop");
				}
			PROFILE_END(SYS_TIMER, tim);
			// 72 us + 5.12 us/period

			__asm__ volatile ("nop\nnop");

			clk = sys_clock();
			PROFILE_START(SYS_TIMER);
				if (!err) do { // Prepare sums
					err = pm_math23_task(IC_USED);
					if (err) break; // Deviation error
				} while (PM_IS_RUN(IC_USED));
				if (!err) pm_math23_post(IC_USED);
			PROFILE_END(SYS_TIMER, tim);
			// 5.48 us per one period
			clk -= sys_clock();

			__asm__ volatile ("nop\nnop");

			{ // Calculate average period (~T~)

				PROFILE_START(SYS_TIMER);
					// ~T~ = S / (2*_N1*(_N1+1))
					sum = pm_math23_sum(IC_USED);
					num = pm_math23_num(IC_USED);
					period = math23_dperiod(sum, num);
					if (PM_GET_MODE(IC_USED) == ICM_PRE4) period /= 8;
					else if (PM_GET_MODE(IC_USED) != ICM_EDGE) period /= 2;
				PROFILE_END(SYS_TIMER, tim); // ~135 us

				ASSERT(period == 0x1000);

				__asm__ volatile ("nop\nnop");

				qmc = pm_math23_qmc(IC_USED);
				ASSERT(qmc == 0); // No STD

				PROFILE_START(SYS_TIMER);
					if (num >= _CT3_) {
						qmc = math23_qmc(qmc, num,_CT3_);
						if (PM_GET_MODE(IC_USED) == ICM_PRE4) qmc /= 4;
						else if (PM_GET_MODE(IC_USED) == ICM_EDGE) qmc *= 2;
					} else qmc = 65535; // Maximum error value
				PROFILE_END(SYS_TIMER, tim); // ~80 us

				__asm__ volatile ("nop\nnop");
			}

			++stage; break; // Next test

		case 5: // Algorithm validation

#ifdef __MPLAB_SIM

			fill_buffer(avep, BUF_SIZE); // Fill buffer
			i = 1; while (i < BUF_SIZE) { // Add deviation
				float d = m_grandf(0, avep/18); // Gaussian
				if ((d < 0) && (d < -(int)avep/3)) continue;
				else if ((d > 0) && (d > avep/3)) continue;
				PM_BUF(IC_USED, i) += (int)d; ++i;
			}

			pm_math_init(IC_USED); err = 0;
			num = pm_math23_start(IC_USED, _CT3_, 4000);
			if (num < _CT3_) err = 1; // Time-out error
			if (!err) do {
				err = pm_math23_task(IC_USED);
				if (err) break; // Deviation error
			} while (PM_IS_RUN(IC_USED));
			if (!err) pm_math23_post(IC_USED);

			if (err < 0) { // NSE error
				i = 3 * pm_math23_num(IC_USED)-1;
				ASSERT(PM_BUF(IC_USED, i) < 5*(avep/3));
				ASSERT(PM_BUF(IC_USED, i) > avep/3);

				for (i += 2; i < BUF_SIZE-1; ++i)
				{ // Buffer must be restored
					err = (unsigned)PM_BUF(IC_USED, i) -
						(unsigned)PM_BUF(IC_USED, i-1);
					if (err < 0) err = -err;
					ASSERT(err < 5*(avep/3));
					ASSERT(err > avep/3);
				}
			}

			period = math23_dperiod( // Doubled period
				pm_math23_sum(IC_USED), pm_math23_num(IC_USED));
			if (PM_GET_MODE(IC_USED) == ICM_PRE4) period /= 8;
			else if (PM_GET_MODE(IC_USED) != ICM_EDGE) period /= 2;

			if (num >= _CT3_) {
				qmc = math23_qmc(pm_math23_qmc(IC_USED),
								 pm_math23_num(IC_USED),_CT3_);
				if (PM_GET_MODE(IC_USED) == ICM_PRE4) qmc /= 4;
				else if (PM_GET_MODE(IC_USED) == ICM_EDGE) qmc *= 2;
			} else qmc = 65535;

			{ // Compute Median and STD
				static double p[256]; double d[ARSIZE(p)];
				static float q[ARSIZE(p)]; 
				static int j = 0;

				if (qmc != 65535) {
					q[j] = qmc; // Real QMC values
					p[j] = period; // Real values of period
					if (++j >= 3) mean = period; // Ready
				}

				if (mean != 0) {
					mean = 0; std = 0; qmc_ave = 0;
					for (i = 0; p[i] != 0; ++i) mean += p[i];
					mean /= i;

					for (i = 0; p[i] != 0; ++i) d[i] = p[i] - mean;
					for (i = 0; p[i] != 0; ++i) std += d[i]*d[i];
					std = sqrt(std/i);

					for (i = 0; p[i] != 0; ++i) qmc_ave += q[i];
					qmc_ave /= i;

					if (j >= (ARSIZE(p)-1)) j = 0; // Circle

					__asm__ volatile ("nop\nnop");
				}
			}

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
