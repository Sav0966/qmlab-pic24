#ifndef _DISPATCH_INCL_
#define _DISPATCH_INCL_

#include <config.h>

typedef enum {
DISP_CLOCK, DISP_SYSUER, DISP_SYSURX, DISP_SYSUTX,
DISP_LAST } DISP_EVENT;

#define ENTER_DISP_LEVEL() {\
	__asm__ volatile ("push SR\n");\
	SR = (DISP_IPL << 5)

#define LEAVE_DISP_LEVEL()\
	__asm__ volatile ("pop SR\n"); } ((void)0)

void disp_init(void);

#ifndef __PFVOID
#define __PFVOID
typedef void (*PFVOID)(void);
#endif

#ifndef NULL
#define NULL	((void*)0)
#endif

typedef struct tagMSG {
	char			task;
	char			message;
	unsigned int	wParam;
	long			lParam;
	int				time;
} MSG, *PMSG;

PFVOID disp_sethook(DISP_EVENT evt, PFVOID hook);

#endif /*_DISPATCH_INCL_*/
