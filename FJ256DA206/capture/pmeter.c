/*
*	Period meter with Input Capture module
*	and methematics (statistical algorithm)
*
*	Compile this module with -O3 option
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

#include <pmvars.h> // Short names for the methematics

static unsigned int _N1 __attribute__((near));
static unsigned int _N2 __attribute__((near));
static unsigned int _N3 __attribute__((near));
static unsigned int *_pT1 __attribute__((near));
static unsigned int *_pT2 __attribute__((near));
static unsigned int *_pT3 __attribute__((near));
static unsigned long long _S1 __attribute__((near));
static unsigned long long _S2 __attribute__((near));
static unsigned long long _S3 __attribute__((near));
static union { // Durations of _Sn sampling
	unsigned long ul;
	struct { unsigned lo; unsigned hi; } u;
} _dT1 __attribute__((near)), _dT2 __attribute__((near)),
_dT3 __attribute__((near))/*, _dT0 __attribute__((near))*/;

IMPL_PM_MATH_INIT(IC_USED)
{
	register PEINT p; p.peds = _buf;

	// First N cells into the buffer are used (has data)
	_pT1 = (unsigned*)p.p.addr; // Pointer up to (1/3)N
	_pT2 = (unsigned*)p.p.addr; // Pointer up to (2/3)N
	_pT3 = (unsigned*)p.p.addr; // Pointer up to N (full)
	_S1 = 0; _S2 = 0; _S3 = 0; // 1/3, 2/3 and full sums
	_N1 = 0; _N2 = 0; _N3 = 0; // Current steps (periods)
	_dT0.ul = 0; _dT1.ul = 0; _dT2.ul = 0; _dT3.ul = 0;

	return(0);
}

IMPL_PM_MATH23_TASK(IC_USED)
{
	unsigned int* _pT = _pT3;

	__asm__ volatile ("push _DSRPAG");
	__asm__ volatile ("push _DSWPAG");
	DSRPAG = PM_GET_PAGE(IC_USED);
	DSWPAG = PM_GET_PAGE(IC_USED);

	// _pcur.p.addr must be >= 3 (it's always true)
	while (((typeof(_pT3))_pcur.p.addr - 3) > _pT)
	{
		// Prepare buffer (Period[i])
		//	buf[i] = buf[i+1] - buf[i]
		*_pT = *(_pT+1) - *_pT; ++_pT;
		*_pT = *(_pT+1) - *_pT; ++_pT;
		*_pT = *(_pT+1) - *_pT; ++_pT;

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
	}

	__asm__ volatile ("pop _DSWPAG");
	__asm__ volatile ("pop _DSRPAG");
	return(0);
}

IMPL_PM_MATH23_SUM(IC_USED)
{
	unsigned long long dT2, dT3;

	dT2 = _dT2.ul - _dT1.ul; dT3 = _dT3.ul - _dT2.ul;

	return( // Average period ~T~ = SUM / (2*_N1*_N1)
			_S1 + _N1*dT2 + (_N3*dT3 - (_S3 - _S2)));
}

IMPL_PM_MATH23_NUM(IC_USED) { return(_N1); }

#endif // IC_USED
#endif //_PMETER_INCL_

#ifdef __not_compile__// v1.0 -> (3.31us *_N3 + 135us)

IMPL_PM_MATH23_TASK(IC_USED)
{
	__asm__ volatile ("push _DSRPAG");
	DSRPAG = PM_GET_PAGE(IC_USED);

	// _pcur.p.addr must be >= 3 (it's always true)
	while (((typeof(_pT3))_pcur.p.addr - 3) > _pT3)
	{
		unsigned int T1, T2; // Times[i]
		unsigned long T;	 // Period[i]

		// Calculate sums and timer owerflows

		{ // Calculate full sum
			T1 = *_pT3++; T2 = *_pT3++;
			if (T1 > T2) ++_dT3.u.hi;
			T = T2 - T1; T *= ++_N3; _S3 += T;
			T1 = *_pT3++; if (T2 > T1) ++_dT3.u.hi;
			T = T1 - T2; T *= ++_N3; _S3 += T;
			T2 = *_pT3; if (T1 > T2) ++_dT3.u.hi;
			T = T2 - T1; T *= ++_N3; _S3 += T;
		} // _S3 = Sum(i * T[i]), i = 1..N

		{ // Calculate 2/3 sum
			T1 = *_pT2++; T2 = *_pT2++;
			if (T1 > T2) ++_dT2.u.hi;
			T = T2 - T1; T *= ++_N2; _S2 += T;
			T1 = *_pT2; if (T2 > T1) ++_dT2.u.hi;
			T = T1 - T2; T *= ++_N2; _S2 += T;
		} // _S2 = Sum(i * T[i]), i = 1..(2/3)N

		{ // Calculate 1/3 sum
			T1 = *_pT1++; T2 = *_pT1;
			if (T1 > T2) ++_dT1.u.hi;
			T = T2 - T1; T *= ++_N1; _S1 += T;
		} // _S1 = Sum(i * T[i]), i = 1..(1/3)N
	}

	__asm__ volatile ("pop _DSRPAG");
	return(0);
}

IMPL_PM_MATH23_SUM(IC_USED)
{
	unsigned long long dT2, dT3;

	DSR_PAGE(PM_GET_PAGE(IC_USED));
	{ // _dTx.u.hi was determined in task(), _dT0 is't needed
		_dT1.u.lo = *_pT1; _dT2.u.lo = *_pT2; _dT3.u.lo = *_pT3;
	}
	DSR_LEAVE();

	dT2 = _dT2.ul - _dT1.ul; dT3 = _dT3.ul - _dT2.ul;

	return( // Average period ~T~ = SUM / (2*_N1*_N1)
			_S1 + _N1*dT2 + (_N3*dT3 - (_S3 - _S2)));
}

#endif // v1.0
