/*
*	Period meter with Input Capture module
*	and methematics (statistical algorithm)
*
*	Compile this module with -O3 option
*/
#ifndef _PMETER_INCL_
#define _PMERT_INCL_

#include <config.h>
#include <timers.h>

#ifdef IC_USED // Only for used Cupture module

#ifndef _NSE
#define _NSE	((float)0.41)
#endif

#include <pmeter.h>
DECL_PMETER_UI(IC_USED);

// Short names for this C module
#define _buf	_IC_(IC_USED, buf)
#define _pcur	_IC_(IC_USED, pcur)
#define _pend	_IC_(IC_USED, pend)
#define _err	_IC_(IC_USED, err)

// Use PM_BUFFER() definition to declare
//  period meter buffer into your module
extern __eds__ int _buf[]; // Must-be

char _IC_(IC_USED, icm); // = 0 (ICM_OFF)
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

#include <pmvars.h>
// Short names for the methematics
static unsigned int _N1 __attribute__((near));
static unsigned int _N2 __attribute__((near));
static unsigned int _N3 __attribute__((near));
static unsigned int *_pT1 __attribute__((near));
static unsigned int *_pT2 __attribute__((near));
static unsigned int *_pT3 __attribute__((near));
static unsigned int *_pQ0 __attribute__((near));
static unsigned int *_pQ1 __attribute__((near));
static unsigned int *_pQ2 __attribute__((near));
static unsigned int *_pQ3 __attribute__((near));
static unsigned long long _S1 __attribute__((near));
static unsigned long long _S2 __attribute__((near));
static unsigned long long _S3 __attribute__((near));
static unsigned long _Sqmc __attribute__((near));
static unsigned long _T02 __attribute__((near));
static unsigned long _T13 __attribute__((near));
static unsigned int _cmp __attribute__((near));

static union { // Durations of _Sn sampling
	unsigned long ul;
	struct { unsigned lo; unsigned hi; } u;
} _dT1 __attribute__((near)), _dT2 __attribute__((near)),
_dT3 __attribute__((near))/*, _dT0 __attribute__((near))*/;

IMPL_PM_MATH_INIT(IC_USED)
{
	PEINT p; p.peds = _buf;

	// First N cells into the buffer are used (has data)
	_pT1 = (unsigned*)p.p.addr; // Pointer up to (1/3)N
	_pT2 = (unsigned*)p.p.addr; // Pointer up to (2/3)N
	_pT3 = (unsigned*)p.p.addr; // Pointer up to N (full)
	_pQ0 = (unsigned*)p.p.addr; // Start pointer of T02
	_pQ1 = (unsigned*)p.p.addr; // Start pointer of T13
	_pQ2 = (unsigned*)p.p.addr; // End pointer of T02
	_pQ3 = (unsigned*)p.p.addr; // End pointer of T13
	_S1 = 0; _S2 = 0; _S3 = 0; // 1/3, 2/3 and full sums
	_N1 = 0; _N2 = 0; _N3 = 0; // Current steps (periods)
	_cmp = 0; // Acceptable deviation of T02,13(~T~*_NSE)
	_T02 = 0; _T13 = 0; // Durations of two correlations
	_dT0.ul = 0; _dT1.ul = 0; _dT2.ul = 0; _dT3.ul = 0;
	_Sqmc = 0; // QMC sum = Sum(|T02 - T13|)

	return(0);
}

IMPL_PM_MATH23_START(IC_USED)
{
	unsigned long T;
	unsigned int t, n, tim =
				(unsigned)TIMER_READ(SYS_TIMER);

	do
	{ // Wait three correlation times

		n = _N3;
		pm_math23_task(IC_USED); // _cmp == 0
		if (n != _N3) // New time-out on new data
			tim = (unsigned)TIMER_READ(SYS_TIMER);

		t = (unsigned)TIMER_READ(SYS_TIMER);
		if (t < tim) t += (unsigned)TIMER_GET_PR(SYS_TIMER);
		if ((t - tim) > timeout) break; // Wait time-out
	}
	while (_N3 < nCT3);

	if (_N3 >= nCT3)
	{ // 3 correlation times have been observed
		_cmp = ((float)pm_math23_sum(IC_USED)/
			((unsigned long)_N1*_N2)) * _NSE;

		_pQ3 += _N1; _pQ1 = _pQ3;
		// Prepare T02,13 for task()
		DSR_PAGE(PM_GET_PAGE(IC_USED));
			for (n = 0; n < _N2; ++n)
			{ _T02 += *_pQ2++; _T13 += *_pQ3++; }
		DSR_LEAVE();

		T = _T13 - _T02; if (T < 0) T = -T;
		_Sqmc += T; // Calculate QMC sum
	}

	return(_N3);
}

IMPL_PM_MATH23_TASK(IC_USED)
{
	unsigned long T;
	unsigned int* _pT = _pT3;

	if (_IC_(IC_USED, icm) == ICM_EDGE)
	if (((typeof(_pT))_pcur.p.addr - 6) <= _pT)
		return(0); // Number of adges must be even

	__asm__ volatile ("push _DSRPAG");
	__asm__ volatile ("push _DSWPAG");
	DSRPAG = PM_GET_PAGE(IC_USED);
	DSWPAG = PM_GET_PAGE(IC_USED);

	// _pcur.p.addr must be >= 3 (it's always true)
	while (((typeof(_pT))_pcur.p.addr - 3) > _pT)
	{
		// Prepare buffer (Period[i])
		//	buf[i] = buf[i+1] - buf[i]
		*_pT = *(_pT+1) - *_pT; ++_pT;
		*_pT = *(_pT+1) - *_pT; ++_pT;
		*_pT = *(_pT+1) - *_pT; ++_pT;

		if (_cmp != 0)		// Task condition
		for(;;) // Check deviation error (NSE)
		{
			_T02 -= *_pQ0++; _T02 += *_pQ2++;
			_T13 -= *_pQ1++; _T13 += *_pQ3++;
			T = _T02 - _T13; if (T < 0) T = -T;
			if (T < _cmp) { _Sqmc += T;
				_T02 -= *_pQ0++; _T02 += *_pQ2++;
				_T13 -= *_pQ1++; _T13 += *_pQ3++;
				T = _T02 - _T13; if (T < 0) T = -T;
				if (T < _cmp) { _Sqmc += T;
					_T02 -= *_pQ0++; _T02 += *_pQ2++;
					_T13 -= *_pQ1++; _T13 += *_pQ3++;
					T = _T02 - _T13; if (T < 0) T = -T;
					if (T < _cmp) { _Sqmc += T; break; }
				}
			}

			__asm__ volatile ("pop _DSWPAG");
			__asm__ volatile ("pop _DSRPAG");
			return(-1); // Deviation error
		}

		// Calculate sums and duration times

		{ // Calculate full sum
			// unsigned long T;	 // Period[i]
			// T = *_pT3++; _dT3.ul += T; _S3 += ++_N3 * T;
			// T = *_pT3++; _dT3.ul += T; _S3 += ++_N3 * T;
			// T = *_pT3++; _dT3.ul += T; _S3 += ++_N3 * T;
			__asm__ volatile(
			"	mov		_pT3, W0			\n" // W0 = _pT3
			"	mov		_N3, W1				\n" // W1 = _N3
			"	mov		#_dT3, W2			\n" // W2 = &_pdT3
			"	mov		#_S3, W3			\n" // W3 = &_S3
			"	clr		W6					\n" // W6 = 0

			"	mov 	[W0++], W4			\n" // W4 = *_pTx++
			"	add 	W4, [W2], [W2++]	\n" // ( W4 = T )
			"	addc	W6, [W2], [W2--]	\n" // _dTx += T
			"	inc 	W1, W1				\n" // ++_Nx
			"	mul.uu	W1, W4, W4			\n" // W4-5 = T *_Nx
			"	add		W4, [W3], [W3++]	\n" //--------------
			"	addc	W5, [W3], [W3++]	\n" //  _Sx +=
			"	addc	W6, [W3], [W3++]	\n" //		++_Nx *
			"	addc	W6, [W3], [W3]		\n" // 		*(_pTx++)
			"	sub		W3, #6, W3			\n" // Restore &_S3

			"	mov 	[W0++], W4			\n" // W4 = *_pTx++
			"	add 	W4, [W2], [W2++]	\n" // ( W4 = T )
			"	addc	W6, [W2], [W2--]	\n" // _dTx += T
			"	inc 	W1, W1				\n" // ++_Nx
			"	mul.uu	W1, W4, W4			\n" // W4-5 = T *_Nx
			"	add		W4, [W3], [W3++]	\n" //--------------
			"	addc	W5, [W3], [W3++]	\n" //  _Sx +=
			"	addc	W6, [W3], [W3++]	\n" //		++_Nx *
			"	addc	W6, [W3], [W3]		\n" // 		*(_pTx++)
			"	sub		W3, #6, W3			\n" // Restore &_S3

			"	mov 	[W0++], W4			\n" // W4 = *_pTx++
			"	add 	W4, [W2], [W2++]	\n" // ( W4 = T )
			"	addc	W6, [W2], [W2--]	\n" // _dTx += T
			"	inc 	W1, W1				\n" // ++_Nx
			"	mul.uu	W1, W4, W4			\n" // W4-5 = T *_Nx
			"	add		W4, [W3], [W3++]	\n" //--------------
			"	addc	W5, [W3], [W3++]	\n" //  _Sx +=
			"	addc	W6, [W3], [W3++]	\n" //		++_Nx *
			"	addc	W6, [W3], [W3]		\n" // 		*(_pTx++)
//			"	sub		W3, #6, W3			\n" // Restore &_S3

			"	mov		W0, _pT3			\n" // _pT3 = W0
			"	mov		W1, _N3				\n" // _N3 = W1
			::: "w0", "w1", "w2", "w3", "w4", "w5", "w6");
		} // _S3 = Sum(i * T[i]), i = 1..N

		{ // Calculate 2/3 sum
			// unsigned long T;	 // Period[i]
			// T = *_pT2++; _dT2.ul += T; _S2 += ++_N2 * T;
			// T = *_pT2++; _dT2.ul += T; _S2 += ++_N2 * T;
			__asm__ volatile(
			"	mov		_pT2, W0			\n" // W0 = _pT2
			"	mov		_N2, W1				\n" // W1 = _N2
			"	mov		#_dT2, W2			\n" // W2 = &_pdT2
			"	mov		#_S2, W3			\n" // W3 = &_S2
//			"	clr		W6					\n" // W6 = 0

			"	mov 	[W0++], W4			\n" // W4 = *_pTx++
			"	add 	W4, [W2], [W2++]	\n" // ( W4 = T )
			"	addc	W6, [W2], [W2--]	\n" // _dTx += T
			"	inc 	W1, W1				\n" // ++_Nx
			"	mul.uu	W1, W4, W4			\n" // W4-5 = T *_Nx
			"	add		W4, [W3], [W3++]	\n" //--------------
			"	addc	W5, [W3], [W3++]	\n" //  _Sx +=
			"	addc	W6, [W3], [W3++]	\n" //		++_Nx *
			"	addc	W6, [W3], [W3]		\n" // 		*(_pTx++)
			"	sub		W3, #6, W3			\n" // Restore &_S3

			"	mov 	[W0++], W4			\n" // W4 = *_pTx++
			"	add 	W4, [W2], [W2++]	\n" // ( W4 = T )
			"	addc	W6, [W2], [W2--]	\n" // _dTx += T
			"	inc 	W1, W1				\n" // ++_Nx
			"	mul.uu	W1, W4, W4			\n" // W4-5 = T *_Nx
			"	add		W4, [W3], [W3++]	\n" //--------------
			"	addc	W5, [W3], [W3++]	\n" //  _Sx +=
			"	addc	W6, [W3], [W3++]	\n" //		++_Nx *
			"	addc	W6, [W3], [W3]		\n" // 		*(_pTx++)
//			"	sub		W3, #6, W3			\n" // Restore &_S3

			"	mov		W0, _pT2			\n" // _pT2 = W0
			"	mov		W1, _N2				\n" // _N2 = W1
			::: "w0", "w1", "w2", "w3", "w4", "w5", "w6");
		} // _S2 = Sum(i * T[i]), i = 1..(2/3)N

		{ // Calculate 1/3 sum
			 // unsigned long T;	 // Period[i]
			 // T = *_pT1++; _dT1.ul += T; _S1 += ++_N1 * T;
			__asm__ volatile(
			"	mov		_pT1, W0			\n" // W0 = _pT1
			"	mov		_N1, W1				\n" // W1 = _N1
			"	mov		#_dT1, W2			\n" // W2 = &_pdT1
			"	mov		#_S1, W3			\n" // W3 = &_S1
//			"	clr		W6					\n" // W6 = 0

			"	mov 	[W0++], W4			\n" // W4 = *_pTx++
			"	add 	W4, [W2], [W2++]	\n" // ( W4 = T )
			"	addc	W6, [W2], [W2--]	\n" // _dTx += T
			"	inc 	W1, W1				\n" // ++_Nx
			"	mul.uu	W1, W4, W4			\n" // W4-5 = T *_Nx
			"	add		W4, [W3], [W3++]	\n" //--------------
			"	addc	W5, [W3], [W3++]	\n" //  _Sx +=
			"	addc	W6, [W3], [W3++]	\n" //		++_Nx *
			"	addc	W6, [W3], [W3]		\n" // 		*(_pTx++)
//			"	sub		W3, #6, W3			\n" // Restore &_S3

			"	mov		W0, _pT1			\n" // _pT1 = W0
			"	mov		W1, _N1				\n" // _N1 = W1
			::: "w0", "w1", "w2", "w3", "w4", "w5", "w6");
		} // _S1 = Sum(i * T[i]), i = 1..(1/3)N

		if (_cmp == 0) break; // Start condition
	}

	__asm__ volatile ("pop _DSWPAG");
	__asm__ volatile ("pop _DSRPAG");
	return(0); // OK
}

IMPL_PM_MATH23_SUM(IC_USED)
{
	unsigned long long dT2, dT3;

	dT2 = _dT2.ul - _dT1.ul; dT3 = _dT3.ul - _dT2.ul;

	return( // Average period ~T~ = SUM / (2*_N1*(_N1+1))
		_S1 + (_N1+1)*dT2 + ((_N3+1)*dT3 - (_S3 - _S2)));
}

IMPL_PM_MATH23_NUM(IC_USED) { return(_N1); }

IMPL_PM_MATH23_QMC(IC_USED) { return(_Sqmc); }

#endif // IC_USED
#endif //_PMETER_INCL_
