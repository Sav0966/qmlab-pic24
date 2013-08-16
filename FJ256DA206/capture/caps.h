/*
*	Input Capture 1-9 modules definitions
*/
#ifndef _IC_INCL_
#define _IC_INCL_
/*
* IC FIFO Buffer
*/
#define _ICBUF(n)		IC##n##BUF
#define ICBUF(n)		_ICBUF(n)

#define IC_READ(n)		(ICBUF(n))
#define IC_FIFO_SIZE	4
/*
* IC Timer Register
*/
#define _ICTMR(n)		IC##n##TMR
#define ICTMR(n)		_ICTMR(n)
/*
* IC Control Register 1
*/
#define _ICCON1(n)		IC##n##CON1
#define _ICCON1bits(n)	IC##n##CON1bits
#define ICCON1bits(n)	_ICCON1bits(n)
#define ICCON1(n)		_ICCON1(n)

#define IC_SIDL		(1 << 13) // Stop operation in Idle mode

// ICTSEL - Input Capture Timer Select bits
// (def) Clock source of Timer3 is the clock source
#define ICT_TIMER3	0    //  of the capture counter
#define ICT_TIMER2	(1 << 10) // -/- Timer2 is source
#define ICT_TIMER4	(2 << 10) // -/- Timer4 is source
#define ICT_TIMER5	(3 << 10) // -/- Timer5 is source
#define ICT_TIMER1	(4 << 10) // -/- Timer1 is source
#define ICT_FCY2	(7 << 10) // System clock is used

// ICI - Number of Captures per Interrupt Select bits
//  (this field is not used if ICM<2:0> = 001 or 111)
#define IC_RXI_ANY		0      // On every capture event
#define IC_RXI_2DATA	(1 << 5) // On every second event	
#define IC_RXI_3DATA	(2 << 5) // On every third event	
#define IC_RXI_4DATA	(3 << 5) // On every fourth event	

// Input Capture Buffer Not Empty Status bit
#define IC_CAN_READ(n)	(ICCON1bits(n).ICBNE != 0)

// Input Capture Overflow Status Flag bit
#define IC_IS_OERR(n)	(ICCON1bits(n).ICOV != 0)
#define IC_CLR_OERR(n)	while (IC_CAN_READ(n)) (void)IC_READ(n)

// ICM - Input Capture Mode Select bits
#define ICM_OFF			0 // Input capture module is turned off
#define ICM_DISABLE		6 // Unused (module is disabled)
#define ICM_EDGE		1 // Every edge, rising and falling
#define ICM_FALL		2 // Every falling edge (simple mode)
#define ICM_RAISE		3 // Every rising edge (simple mode)
#define ICM_PRE4		4 // Every 4th rising edge (prescaler)
#define ICM_PRE16		5 // Every 16th rising edge (prescaler)
#define ICM_INT			7 // Input capture functions as interrupt
// pin only in CPU Sleep and Idle mode (Interrupt mode), rising
// edge detect only, all other control bits are not applicable

#define IC_ENABLE(n, icm)	ICCON1bits(n).ICM = icm
#define IC_DISABLE(n)		ICCON1bits(n).ICM = ICM_DISABLE
#define IC_IS_ENABLE(n)		(ICCON1bits(n).ICM != ICM_DISABLE)

// Reset the overflow condition flag, reset the receive
// capture FIFO to the empty state, reset the prescale count
#define IC_RESET(n)	ICCON1bits(n).ICM = ICM_OFF; IC_DISABLE(n)
/*
* IC Control Register 2
*/
#define _ICCON2(n)			IC##n##CON2
#define _ICCON2bits(n)		IC##n##CON2bits
#define ICCON2bits(n)		_ICCON2bits(n)
#define ICCON2(n)			_ICCON2(n)

// 32-Bit Timer Mode Select bit - The IC32 bit in both
// ODD and EVEN IC must be set to enable Cascade mode
#define IC32	(1 << 8) // Single 32-bit IC module

// ICTRIG - Trigger Operation Select bit
// 1 = Input source used to trigger the input capture timer
// 0 = Input source used to synchronize input capture timer
//        to timer of another module (Synchronization mode)
#define ICTRIG					(1 << 7)

#define IC_SET_TRIGGERED(n)		(ICCON2bits(n).TRIGSTAT = 1)
#define IC_CLR_TRIGGERED(n)		(ICCON2bits(n).TRIGSTAT = 0)
#define IC_IS_TRIGGERED(n)		(ICCON2bits(n).TRIGSTAT != 1)

// SYNCSEL - Input Source Select for
// Synchronization and Trigger Operation bits
#define IC_SYNC_OC(n)	(n)		// Input is OC(1-9)
#define IC_SYNC_IC5		10		// Input is ICAP5
#define IC_SYNC_TMR(n)	(n+10)	// Input is TMR(1-5)
#define IC_SYNC_IC7		18		// Input is ICAP7
#define IC_SYNC_IC8		19		// Input is ICAP8
#define IC_SYNC_IC1		20		// Input is ICAP1
#define IC_SYNC_IC2		21		// Input is ICAP2
#define IC_SYNC_IC3		22		// Input is ICAP3
#define IC_SYNC_IC4		23		// Input is ICAP4

// These options should only be selected as a trigger source _
#define IC_SYNC_CMP(n)	(n+23)	// Input is CMP(1-3)          |
#define IC_SYNC_AD		27		// Input is AD                |
#define IC_SYNC_CTMU	28		// Input is CTMU              |
// These inputs should not be used as a synchronization source

#define IC_SYNC_IC6		29		// Input is ICAP6
#define IC_SYNC_IC9		30		// Input is ICAP9
#define IC_SYNC_NONE	0 		// Default - none
#define IC_SYNC_MASK	31		// Reserved
/*
* Interrupt management of IC module
*/
// Interrupt Priority levels
#define _ICIP(n) _IC##n##IP
// Setup and obtain IC IPL values
#define IC_SET_IPL(n, ipl)		_ICIP(n) = ipl
#define IC_GET_IPL(n)			((int)_ICIP(n))

// Interrupt Enable bits
#define _ICIE(n) _IC##n##IE
// Enable and disable IC interrupts
#define IC_ENABLE_INT(n)		_ICIE(n) = 1
#define IC_DISABLE_INT(n)		_ICIE(n) = 0
#define IC_IS_ENABLE_INT(n)		(_ICIE(n) == 1)

// Interrupt Status bits
#define _ICIF(n) _IC##n##IF
// Clear, Set and Check Interrupt Status
#define IC_CLR_FLAG(n)		_ICIF(n) = 0
#define IC_SET_FLAG(n)		_ICIF(n) = 1
#define IC_IS_FLAG(n)		(_ICIF(n) != 0)
/*
* Power management of IC module (PMDx.ICnMD bit)
*/
#define __ICMD(n)			_IC##n##MD
#define _ICMD(n)			__ICMD(n)
/*
*	Template of IC Interrupt Service Routines
*/
#define _IC_INTFUNC(n, attr)\
__attribute__((__interrupt__, attr)) _IC##n##Interrupt
#define IC_INTFUNC(n, attr) _IC_INTFUNC(n, attr)
/*
*	Initialization functions
*/
#define IC_INIT(n, mode, sync, ipl) {\
	IC_DISABLE_INT(n); /* Disable interrupt */\
	_ICMD(n) = 0;    /* Power on IC module */\
\
	/* Reset ICOV, FIFO and prescale count (Off mode) */\
	ICCON1(n) = (mode) & ~ICM_INT; /* Turn off module */\
	ICCON2(n) = sync; /* Set Sinch\Trigger sources */\
\
	if ((sync) & IC_SYNC_MASK) /* Trigger mode */\
		if ((sync) & ICTRIG) IC_CLR_TRIGGERED(n);\
\
	IC_CLR_FLAG(n); /* Clear interrupt flag */\
\
	if (ipl > 0) { /* Enable interrupt, if used */\
		IC_SET_IPL(n, ipl); IC_ENABLE_INT(n);\
	}\
\
	ICCON1(n) = mode; /* Set desired mode */\
} ((void)0)

#define IC_IS_INIT(n) /* Powered & !(Off mode) */\
	((_ICMD(n) == 0) && (ICCON1bits(n).ICM != ICM_OFF))

#define IC_DONE(n)\
	IC_DISABLE_INT(n); /* Disable interrupt */\
	ICCON1bits(n).ICM = ICM_OFF; /* Disable module */\
	IC_CLR_FLAG(n) /* Clear interrupt status flags */

#define IC_PWOFF(n)\
	IC_DONE(n); /* Disable IC and interrupt */\
	_ICMD(n) = 1 /* Power off IC module */

#endif /*_IC_INCL_*/
