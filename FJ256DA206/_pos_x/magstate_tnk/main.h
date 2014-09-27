#ifndef _MAIN_INCL_
#define _MAIN_INCL_

#include <stdlib.h>

// 20 џэт 1997 15:00:00
#define INITIAL_TIME	853801200L

#define SYS_INTERRUPT(vect) tn_sys_interrupt(vect)

typedef struct _IFUNC { // Interrupt descriptor
	void (*ifunc)(void); // Interrupt function
	void (*hfunc)(void); // System handler
	int ipl;             // Priority level
} IFUNC, *PIFUNC;
typedef void* HIFUNC; // Descriptor handler



#endif /*_MAIN_INCL_*/
