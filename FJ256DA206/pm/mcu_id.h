/*
*	MCU divice ID, Revision and Configuration Words
*/
#ifndef _MCU_ID_INCL_
#define _MCU_ID_INCL_
#include <p24Fxxxx.h> /* PIC24F only */
#include <libpic30.h> /*_prog_addressT */

/* Return two words, started from 'addr' */
unsigned long _mcu_long(_prog_addressT addr);

#define MCU_CONFIG12	_mcu_long(__builtin_tbladdress(&_CONFIG2))
#define MCU_CONFIG23	_mcu_long(__builtin_tbladdress(&_CONFIG3))
#define MCU_CONFIG1		((unsigned int)(MCU_CONFIG12 >> 16))
#define MCU_CONFIG2		((unsigned int)MCU_CONFIG12)
#define MCU_CONFIG3		((unsigned int)MCU_CONFIG23)

#if (!defined(__DEBUG) || defined(__MPLAB_DEBUGGER_ICD2))
extern unsigned long _mcu_revid(void); /* For hardware */
 #define MCU_REV	((unsigned int)(_mcu_revid()>>16))
 #define MCU_ID		((unsigned int)_mcu_revid())
#else /* For MPLAB SIM and other tools return 0 */
 #define MCU_REV	0
 #define MCU_ID		0
#endif

#endif /*_MCU_ID_INCL_*/
