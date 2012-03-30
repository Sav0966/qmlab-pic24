/*
*   Header for PIC24FJ Reset/Control library functions
*/
#ifndef _RESET_INCL_
#define _RESET_INCL_

#include <p24Fxxxx.h> /* PIC24F only */

#if (defined(__PIC24FK__)) /* PIC24FJ only */
#error -- For PIC24FJ256DA210 Family only --
#endif
/*
* PIC24FJ256DA210 Family Silicon Errata (DS80505B)
* 1. Module: Oscillator (Two-Speed Start-up)
* Two-Speed Start-up is not functional. Leaving the IESO
* Configuration bit in its default state (Two-Speed Start-up
* enabled) may result in unpredictable operation.
*
* Work around: None.
* Always program the IESO Configuration bit to disable the
* feature (CW2<15> = 0).  (Affected Silicon Revisions - A3)
*
* You must always reset IESO in _CONFIG2(), insert in code
* _CONFIG2(IESO_OFF); // Reset IESO (Errata sheet DS80505)
*/
#ifndef IS_MCU_PROGRAMMED
#include <mcu_id.h>
/* The IESO must be cleared at the last step of programming */
#define IS_MCU_PROGRAMMED()	((MCU_CONFIG2 & ~IESO_OFF) == 0)
#endif
/*
* PIC24FJ256DA210 Family Silicon Errata (DS80505B)
* 2. Module: Resets
*	On Brown-out Resets and External (Master Clear)	Resets, the
*	POR bit may also become set. This may cause Brown-out and
*	External Reset conditions to be indistinguishable from a
*	Power-on Reset.         (Affected Silicon Revisions - A3)
*
* For BOR detecting: sleep in Low-Voltage Detect Interrupt, use
* idle mode in loops, set some other SFRs bit in not POR state
* Dot'n use VREGS and SWDTEN bits to detect BOR (ICD 2 tested)
*/

/*
*	Get reset state, uParam is stored in unused RCON bits
*	Software reset device with uParam (store only unused bits)
*/
unsigned int get_reset_state(void);
void do_reset(unsigned int uParam);

void clr_reset_state(void); /* Reset RCON=POR and uParam=0 only */
#define clr_reset_state() {RCON = 3; get_reset_state();} (void) 0;
/*
*	RCON register bit mask definitions
*/
#define RCON_POR	1 /* Power-on reset */
#define RCON_BOR	2 /* Brown-out reset is detected */
#define	RCON_IDLE	4 /* Wake up from Idle mode */
#define	RCON_SLEEP	8 /* Wake up from Sleep mode */

#define RCON_WDTO	16 /* Watchdog time-out has occurred */
#define RCON_SWDTEN	32 /* Software Enable/Disable of WDT bit */
#define RCON_SWR	64 /* RESET instruction or User-type reset */
#define RCON_EXTR	128 /* A Master Clear Reset has occurred */

#define RCON_VREGS	256 /* Voltage Regulator Standby Disable bit */
#define RCON_CM		0x0200 /* Configuration Word Mismatch Reset */
#define RCON_UNUSED	0x3C00 /* Unused RCON bits, to store uParam */
#define RCON_IOPUWR	0x4000 /* An illegal opcode detection, */
/* an illegal address mode or uninitialized W register */
/* is used as an Address Pointer and caused a Reset */
#define RCON_TRAPR	0x8000 /* A Trap Conflict Reset */

#define RCON_BITS		~RCON_UNUSED /* All used RCON bits */

#define INT_RESET		(RCON_WDTO | RCON_SWR)
#define EXT_RESET		(RCON_POR | RCON_BOR | RCON_EXTR)
#define TRAP_RESET		(RCON_TRAPR | RCON_IOPUWR | RCON_CM)
#define RCON_RESETS		(INT_RESET | EXT_RESET | TRAP_RESET)
#define RCON_STATUS		(RCON_RESETS | RCON_IDLE|RCON_SLEEP)

/*
*	As definitions in reset.h for PIC24H peripherals
*/

/* disable interrupts for the given number of cycles */
#define DisableInterrupts(cycles) __builtin_disi(cycles)

/* macros to enable/ disable WDT in software */
#define WDTSWEnable			RCONbits.SWDTEN = 1
#define WDTSWDisable		RCONbits.SWDTEN = 0

/* to check if RESET is due to MCLR */
#define _isMCLR(state)	(((state) & RCON_EXTR) != 0)
#define isMCLR()		_isMCLR(get_reset_state())
/* to check if RESET is due to POR */
#define _isPOR(state)	(((state) & RCON_POR) != 0)
#define isPOR()			_isPOR(get_reset_state())
/* to check if RESET is due to BOR */
#define _isBOR(state)	(((state) & RCON_BOR) != 0)
#define isBOR()			_isBOR(get_reset_state())

/* to check if Software Reset occured */
#define _isSWR(state)	(((state) & RCON_SWR) != 0)
#define isSWR()			_isSWR(get_reset_state())

/* to check if Trap Error occured */
#define _isTRAPR(state)	(((state) & RCON_TRAPR) != 0)
#define isTRAPR()		_isTRAPR(get_reset_state())
/* to check if Illegal Opcode or uninit Write Op occured */
#define _isIOPUWR(state)	(((state) & RCON_IOPUWR) != 0)
#define isIOPUWR()			_isIOPUWR(get_reset_state())
/* to check if Config Mismatched occured */
#define _isCM(state)	(((state) & RCON_CM) != 0)
#define isCM()			_isCM(get_reset_state())

/* to check if WDT timeout occured during normal operation */
#define _isWDTTO(state)		(((state) & RCON_WDTO) != 0)
#define isWDTTO()			_isWDTTO(get_reset_state())

/*
* Then we must check RCON directly and can loss initial
* reset state of RCON register if some wakeup is occurred
* Use the macros after entering in sleep or idle mode
*/

/* to check if WDT timeout occured during sleep */
#define isWDTWU() ((get_reset_state() & (RCON_WDTO|RCON_SLEEP))\
										==(RCON_WDTO|RCON_SLEEP))
/*
* to check for the cause of Wakeup from sleep: 0 if not WU
* 1 - wakeup is due to interrupt, 2 - wakeup is due to MCLR
* 3 - wakeup is due to WDTO, 4 - wakeup is due to BOR
**/
#define isWU() ((get_reset_state() & RCON_SLEEP) != 0)?\
		(isMCLR()? 2: (isWDTTO()? 3: (isBOR()? 4: 1))): 0

#endif /*_RESET_INCL_*/
