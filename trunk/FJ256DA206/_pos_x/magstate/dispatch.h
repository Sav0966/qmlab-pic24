#ifndef _DISPATCH_INCL_
#define _DISPATCH_INCL_

#define DISP_IPL	2

#include <ints.h>
#define DISP_INT	4	// Dispatch vector
#define DISPATCH()	INT_SET_FLAG(DISP_INT)

#define ENTER_DISP_LEVEL() {\
	__asm__ volatile ("push SR\n");\
	SR = (DISP_IPL << 5)

#define LEAVE_DISP_LEVEL()\
	__asm__ volatile ("pop SR\n"); } ((void)0)

#define _DISABLE_MOD(module, source)\
	ENTER_DISP_LEVEL(); module##_DISABLE_##source
#define DISABLE_MOD(module, source)	_DISABLE_MOD(module, source)

#define _ENABLE_MOD(module, source)\
	module##_ENABLE_##source; LEAVE_DISP_LEVEL()
#define ENABLE_MOD(module, source) _ENABLE_MOD(module, source)

typedef enum { // Masks of dispatched events
DISP_CLOCK, DISP_SYSUER, DISP_SYSURX, DISP_SYSUTX,
DISP_LAST } DISP_EVENT;

void disp_init(void);

#ifndef NULL
#define NULL	((void*)0)
#endif

typedef struct {
	int		task;
	int		message;
	int		wParam;
	long	lParam;
	int		time;
} MSG, *PMSG;

PMSG pop_msg(PMSG);
PMSG push_msg(PMSG pmsg);
void free_msg(PMSG pmsg);
PMSG alloc_msg(void);

typedef struct tagHOOK *PHOOK;

typedef struct tagHOOK {
	int (*pfnevent)(void);
	int (*pfnhook)(int);
	volatile int event;

	PHOOK prev;
	PHOOK next;
} HOOK;

#define PREV_HOOK(phook) phook->prev_hook
#define NEXT_HOOK(phook) phook->next_hook

PHOOK disp_sethook(DISP_EVENT mask, PHOOK phook);
PHOOK disp_delhook(DISP_EVENT mask, PHOOK phook);

#endif /*_DISPATCH_INCL_*/
