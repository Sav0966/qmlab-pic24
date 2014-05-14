#include <p24Fxxxx.h>
#include <config.h>
#include <buffer.h>
#include <clock.h>
#include <ints.h>

#include "sysuart.h"
#include "dispatch.h"

#define MAX_MESSAGES	16

#ifndef ARSIZE
#define ARSIZE(ar) (sizeof(ar)/sizeof(ar[0]))
#endif

static volatile PMSG QUEBUF(MSG, MAX_MESSAGES);
static volatile MSG _msg[MAX_MESSAGES] __attribute__((noload));

static volatile int cur_clock __attribute__((near)); // = 0
static volatile int cur_syser __attribute__((near)); // = 0
static volatile int cur_sysrx __attribute__((near)); // = 0
static volatile int cur_systx __attribute__((near)); // = 0

static int def_hookfn(int);
static int def_eventfn(void);
static HOOK _def_hook = { 0, def_eventfn, def_hookfn };

static volatile PHOOK phook[DISP_LAST] __attribute__((near));

void disp_init(void)
{
	INT_DISABLE_INT(DISP_INT); // Disable the interrupt
	ENTER_DISP_LEVEL();
	{
		int i;
		for (i = 0; i < DISP_LAST; ++i) phook[i] = &_def_hook;
		for (i = 0; i < ARSIZE(_msg); ++i) _msg[i].message = 0;
		QUEBUF_INIT(MSG); // Reset the queue of messages
	} // Disp-level
	LEAVE_DISP_LEVEL();
	INT_SET_IPL(DISP_INT, DISP_IPL);   // Set IPL
	INT_CLR_FLAG(DISP_INT); // Clear interrupt flag
	INT_ENABLE_INT(DISP_INT); // Enable interrupt
}

void free_msg(PMSG pmsg)
{
//	ENTER_DISP_LEVEL();
		pmsg->message = 0;
//	LEAVE_DISP_LEVEL();
}

PMSG alloc_msg(void)
{
	PMSG ret = (PMSG)_msg;
	ENTER_DISP_LEVEL();
	{
		int i;
		for (i = 0; i < ARSIZE(_msg); ++i, ++ret)
		if (ret->message == 0) { ret->message = -1; break; }

		if (i == ARSIZE(_msg)) ret = NULL;
	} // Disp-level
	LEAVE_DISP_LEVEL();
	return( ret );
}

PMSG push_msg(PMSG pmsg)
{
	PMSG ret = NULL;
	ENTER_DISP_LEVEL();
	{
		if (QUEBUF_LEN(MSG) != QUEBUF_SIZE(MSG))
		{
			ret = pmsg;
			_QUEBUF_PUSH(MSG, ret);
		}
	} // Disp-level
	LEAVE_DISP_LEVEL();
	return( ret );
}

PMSG pop_msg()
{
	PMSG ret = NULL;
	ENTER_DISP_LEVEL();
		if (QUEBUF_LEN(MSG) != 0)
			_QUEBUF_POP(MSG, ret);
	LEAVE_DISP_LEVEL();
	return( ret );
}

PHOOK disp_sethook(DISP_EVENT mask, PHOOK hook)
{
	PHOOK ret;
	ENTER_DISP_LEVEL();
		ret = phook[mask];
		phook[mask] = hook;
	LEAVE_DISP_LEVEL();
	return( ret );
}

static int def_eventfn(void) { return( 0 ); }

static int def_hookfn(int evt) { return( evt ); }

void INT_INTFUNC(DISP_INT, auto_psv)()
{
	int i; // Current event
	INT_CLR_FLAG(DISP_INT); // Clear flag

	for (i = 0; i < DISP_LAST; ++i)
	{
		int event = phook[i]->pfnevent();
		if (phook[i]->event != event) // Check event
			phook[i]->event = phook[i]->pfnhook(event);
	}
}
