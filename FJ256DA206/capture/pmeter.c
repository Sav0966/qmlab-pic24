/*
*	Period meter with Input Capture module
*	and methematics (statistical algorithm)
*/
#ifndef _PMETER_INCL_
#define _PMERT_INCL_

#include <config.h>

#ifdef IC_USED // Only for used Cupture module

#include <pmeter.h>

// Short names for this C module
#define _buf	_IC_(IC_USED, buf)
#define _pcur	_IC_(IC_USED, pcur)
#define _pend	_IC_(IC_USED, pend)
#define _err	_IC_(IC_USED, err)

// Use PM_BUFFER() definition to declare
//  period meter buffer into your module
extern __eds__ int _buf[]; // Must-be

volatile PEINT _pcur __attribute__((near));
volatile int *_pend __attribute__((near));
volatile int _err __attribute__((near));

void IC_INTFUNC(IC_USED, no_auto_psv)(void)
{
	IC_CLR_FLAG(IC_USED);

	__asm__ volatile ("push _DSWPAG");
	DSWPAG = PM_GET_PAGE(IC_USED);

	if (IC_IS_OERR(IC_USED)) ++_err;

	while (IC_CAN_READ(IC_USED)) {
		*_pcur.p.addr = IC_READ(IC_USED);
		if (_pcur.p.addr < _pend) ++_pcur.p.addr;
	} // This loop clears OERR flag (if needed)

	__asm__ volatile ("pop _DSWPAG");
}

// Short names for the methematics
#define _pT1	_IC_(IC_USED, pT1)
#define _pT2	_IC_(IC_USED, pT2)
#define _pT3	_IC_(IC_USED, pT3)
#define _S1		_IC_(IC_USED, S1)
#define _S2		_IC_(IC_USED, S2)
#define _S3		_IC_(IC_USED, S3)
#define _N1		_IC_(IC_USED, N1)
#define _N2		_IC_(IC_USED, N2)
#define _N3		_IC_(IC_USED, N3)

static unsigned int _N1 __attribute__((near));
static unsigned int _N2 __attribute__((near));
static unsigned int _N3 __attribute__((near));
static unsigned int *_pT1 __attribute__((near));
static unsigned int *_pT2 __attribute__((near));
static unsigned int *_pT3 __attribute__((near));
static unsigned long long _S1 __attribute__((near));
static unsigned long long _S2 __attribute__((near));
static unsigned long long _S3 __attribute__((near));

IMPL_PM_MATH_INIT(IC_USED)
{
	register PEINT p; p.peds = _buf;

	// First N cells into the buffer are used (has data)
	_pT1 = (unsigned*)p.p.addr; // Pointer up to (1/3)N
	_pT2 = (unsigned*)p.p.addr; // Pointer up to (2/3)N
	_pT3 = (unsigned*)p.p.addr; // Pointer up to N (full)
	_S1 = 0; _S2 = 0; _S3 = 0; // 1/3, 2/3 and full sums
	_N1 = 0; _N2 = 0; _N3 = 0; // Current steps (periods)

	return(0);
}

IMPL_PM_MATH23_TASK(IC_USED)
{
	__asm__ volatile ("push _DSRPAG");
	DSRPAG = PM_GET_PAGE(IC_USED);

	// _pcur.p.addr must be >= 3 (it's always true)
	while (((typeof(_pT3))_pcur.p.addr - 3) > _pT3)
	{
		register unsigned int T1, T2; // Times[i]
		register unsigned long T;	 // Period[i]

		{ // Calculate full sum
			T1 = *_pT3++; T2 = *_pT3++;
			T = T2 - T1; T *= ++_N3; _S3 += T;
			T1 = *_pT3++; T = T1 - T2; T *= ++_N3; _S3 += T;
			T2 = *_pT3; T = T2 - T1; T *= ++_N3; _S3 += T;
			// _S3 = Sum(i * T[i]), where i = 1 ... N
		}

		{ // Calculate 2/3 sum
			T1 = *_pT2++; T2 = *_pT2++;
			T = T2 - T1; T *= ++_N2; _S2 += T;
			T1 = *_pT2; T = T1 - T2; T *= ++_N2; _S2 += T;
 			// _S2 = Sum(i * T[i]), where i = 1 ... (2/3)N
		}

		{ // Calculate 1/3 sum
			T1 = *_pT1++; T2 = *_pT1;
			T = T2 - T1; T *= ++_N1; _S1 += T;
 			// _S1 = Sum(i * T[i]), where i = 1 ... (1/3)N
		}
	}

	__asm__ volatile ("pop _DSRPAG");
	return(0);
}

static unsigned long dif_time(unsigned* pT1, unsigned* pT2)
{
	register union {
		unsigned long ul;
		struct { unsigned lo; unsigned hi; } u;
	} tim;

	// Calculate the counter overflows
	for (tim.ul = *pT2; pT2 > pT1; --pT2)
		if (*pT2 < *(pT2-1)) ++tim.u.hi;

	return(tim.ul - (unsigned long)(*pT1));
}

IMPL_PM_MATH23_SUM(IC_USED)
{
	unsigned long long dT2, dT3;

	DSR_PAGE(PM_GET_PAGE(IC_USED));
	{
		dT2 = dif_time(_pT1, _pT2);
		dT3 = dif_time(_pT2, _pT3);
	}
	DSR_LEAVE();

	return( // Average period ~T~ = Sum / (2*_N1*_N1)
			_S1 + _N1*dT2 + (_N3*dT3 - (_S3 - _S2)));
}

IMPL_PM_MATH23_NUM(IC_USED) { return(_N1); }

#endif // IC_USED
#endif //_PMETER_INCL_
