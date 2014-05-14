/*
*	External interrupts 0-4 management functions
*/
#ifndef _INTS_INCL_
#define _INTS_INCL_

// Edge Detect Polarity Select bit
#define _INTEP(i)			_INT##i##EP
#define INT_POSITIVE(i)		_INTEP(i) = 0
#define INT_NEGATIVE(i)		_INTEP(i) = 1
#define INT_IS_POSITIVE(i)	(INT_POSITIVE(i) == 0)

// Interrupt Priority Level
#define _INTIP(i)			_INT##i##IP
// Setup and obtain interrupt pryority level
#define INT_SET_IPL(i, ipl)	_INTIP(i) = ipl
#define INT_GET_IPL(i)		((int)_INTIP(i))

// Interrupt Enable bits
#define _INTIE(i)			_INT##i##IE
// Enable and disable external interrupt
#define INT_ENABLE_INT(i)	_INTIE(i) = 1
#define INT_DISABLE_INT(i)	_INTIE(i) = 0

// IFS register definitions
#define _INTIF(i) _INT##i##IF
// Clear Interrupt Status bit
#define INT_CLR_FLAG(i) _INTIF(i) = 0
#define INT_SET_FLAG(i) _INTIF(i) = 1
#define INT_IS_FLAG(i) (_INTIF(i) != 0)

// External Interrupt Service Routine template
#define _INT_INTFUNC(i, attr) /* interrupt - 0-4 */\
__attribute__((__interrupt__, attr)) _INT##i##Interrupt
#define INT_INTFUNC(i, attr) _INT_INTFUNC(i, attr)
/*
* External Interrupt initialization
*/
#define _INT_DONE(i)\
	INT_DISABLE_INT(i);	/* Disable the interrupt */\
	INT_CLR_FLAG(i)		/* Clear interrupt flag */

#define INT_DONE(i)				_INT_DONE(i)

#define _INT_INIT(i, neg, ipl) {\
	INT_DONE(i); /* Disable interrupt and clear flag */\
\
	if (neg) INT_NEGATIVE(i); else INT_POSITIVE(i);\
\
	if (ipl > 0) {\
		INT_SET_IPL(i, ipl);	 /* Setup IPL */\
		INT_ENABLE_INT(i); /* Enable interrupt */\
	}\
} ((void)0)

#define INT_INIT(i, neg, ipl)	_INT_INIT(i, neg, ipl)
/*
*	Public and protected names of module
*/
#define ___INT_(n, name)	_int_##n##_##name
#define __INT_(n, name)		int_##n##_##name
#define _INT_(n, name)		___INT_(n, name) // Protected
#define INT_(n, name)		__INT_(n, name) // Public

#endif /*_INTS_INCL_*/
