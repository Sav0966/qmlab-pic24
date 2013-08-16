/*
*	Output Compare 1-9 modules definitions
*/
#ifndef _OC_INCL_
#define _OC_INCL_

/*
* OC Control Register 1
*/
#define _OCCON1(n)		OC##n##CON1
#define _OCCON1bits(n)	OC##n##CON1bits
#define OCCON1bits(n)	_OCCON1bits(n)
#define OCCON1(n)		_OCCON1(n)

#define OC_SIDL		(1 << 13) // Stop Output Compare in Idle Mode

// OCTSEL - Output Compare Timer Select bits
// (def) Clock source of Timer2 is the clock source
#define OCT_TIMER2	0    //  of the capture counter
#define OCT_TIMER3	(1 << 10) // -/- Timer3 is source
#define OCT_TIMER4	(2 << 10) // -/- Timer4 is source
#define OCT_TIMER5	(3 << 10) // -/- Timer5 is source
#define OCT_TIMER1	(4 << 10) // (only synchronous clock is supported)
#define OCT_FCY2	(7 << 10) // System clock is used

// Refer to the device data sheet to find the Fault bits mapping
#define ENFLT2		(1 << 9) // Fault 2 Input Enable bit
#define ENFLT1		(1 << 8) // Fault 1 Input Enable bit
#define ENFLT0		(1 << 7) // Fault 0 (corresponds to OCFA pin)
#define OCFLT2		(1 << 6) // PWM Fault 2 Condition Status bit
#define OCFLT1		(1 << 5) // PWM Fault 1 Condition Status bit
#define OCFLT0		(1 << 4) // PWM Fault 0 Condition Status bit

#define TRIGMODE	(1 << 3) // Trigger Status Mode Select bit
// 1 = TRIGSTAT (OCxCON2<6>) is cleared when OCxRS = OCxTMR
// or in software; 0 = TRIGSTAT is cleared only by software

// OCM - Output Compare Mode Select bits
#define OCM_DISABLE		0 // Output compare channel is disabled
#define OCM_SSHOTL		1 // Single Compare Single-Shot mode: Initialize
			// OCx pin low, compare event with OCxR; forces OCx pin high
#define OCM_SSHOTH		2 // Single Compare Single-Shot mode: Initialize
			// OCx pin high, compare event with OCxR; forces OCx pin low
#define OCM_SINGLE		3 // Single Compare mode: Compare events with
						  // OCxR, continuously toggle OCx pin
#define OCM_DSHOT		4 // Double Compare Single-Shot mode: Initialize
// OCx pin low, toggle OCx state on matches of OCxR & OCxRS for one cycle
#define OCM_DOUBLE		5 // Double Compare Continuous Pulse mode:
			// Initialize OCx pin low, toggle OCx state continuously
			// on alternate matches of OCxR and OCxRS
#define OCM_EPWM		6 // Edge-Aligned PWM mode: Output set
		// high when OCxTMR = 0 and set low when OCxTMR = OCxR
#define OCM_CPWM		7 // Center-Aligned PWM mode: Output set
		// high when OCxTMR = OCxR and set low when OCxTMR = OCxRS

#define OC_ENABLE(n, ocm)	OCCON1bits(n).OCM = ocm
#define OC_DISABLE(n)		OCCON1bits(n).OCM = OCM_DISABLE
#define OC_IS_ENABLE(n)		(OCCON1(n) != 0)


/*
* Interrupt management of OC module
*/
// Interrupt Priority levels
#define _OCIP(n) _OC##n##IP
// Setup and obtain OC IPL values
#define OC_SET_IPL(n, ipl)		_OCIP(n) = ipl
#define OC_GET_IPL(n)			((int)_OCIP(n))

// Interrupt Enable bits
#define _OCIE(n) _OC##n##IE
// Enable and disable OC interrupts
#define OC_ENABLE_INT(n)		_OCIE(n) = 1
#define OC_DISABLE_INT(n)		_OCIE(n) = 0
#define OC_IS_ENABLE_INT(n)		(_OCIE(n) == 1)

// Interrupt Status bits
#define _OCIF(n) _OC##n##IF
// Clear, Set and Check Interrupt Status
#define OC_CLR_FLAG(n)		_OCIF(n) = 0
#define OC_SET_FLAG(n)		_OCIF(n) = 1
#define OC_IS_FLAG(n)		(_OCIF(n) != 0)
/*
* Power management of OC module (PMDx.OCnMD bit)
*/
#define __OCMD(n)			_OC##n##MD
#define _OCMD(n)			__OCMD(n)
/*
*	Template of OC Interrupt Service Routines
*/
#define _OC_INTFUNC(n, attr)\
__attribute__((__interrupt__, attr)) _OC##n##Interrupt
#define OC_INTFUNC(n, attr) _OC_INTFUNC(n, attr)

#endif /*_OC_INCL_*/

#ifdef __not_compil__
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

#endif //__not_compil__
