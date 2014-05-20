#include <p24Fxxxx.h>
#include <config.h>
#include <buffer.h>
#include <clock.h>
#include <ints.h>

#include "sysuart.h"
#include "dispatch.h"

#define IO_IPL		(DISP_IPL-1)
#define IO_INT		(DISP_INT-1)

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
static HOOK _def_hook = { def_eventfn, def_hookfn, 0,NULL,NULL };

static volatile PHOOK phooks[DISP_LAST] __attribute__((near));

void disp_init(void)
{
	INT_DISABLE_INT(DISP_INT); // Disable the interrupt
	INT_DISABLE_INT(DISP_INT-1); // Disable the interrupt
	ENTER_DISP_LEVEL();
	{
		int i;
		for (i = 0; i < DISP_LAST; ++i) phooks[i] = &_def_hook;
		for (i = 0; i < ARSIZE(_msg); ++i) _msg[i].message = 0;
		QUEBUF_INIT(MSG); // Reset the queue of messages
	} // Disp-level
	LEAVE_DISP_LEVEL();
	INT_SET_IPL(DISP_INT, DISP_IPL);   // Set IPL
	INT_SET_IPL(DISP_INT-1, IO_IPL);   // Set IPL
	INT_CLR_FLAG(DISP_INT); // Clear interrupt flag
	INT_CLR_FLAG(DISP_INT-1); // Clear interrupt flag
	INT_ENABLE_INT(DISP_INT-1); // Enable interrupt
	INT_ENABLE_INT(DISP_INT); // Enable interrupt
}

void free_msg(PMSG pmsg)
{
	ENTER_DISP_LEVEL();
		pmsg->message = 0;
	LEAVE_DISP_LEVEL();
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
	PMSG ret = alloc_msg();
	if (ret != NULL)
	{
		*ret = *pmsg;
		ENTER_DISP_LEVEL();
		{
			if (QUEBUF_LEN(MSG) != QUEBUF_SIZE(MSG))
				_QUEBUF_PUSH(MSG, ret);
		} // Disp-level
		LEAVE_DISP_LEVEL();
	}
	return( ret );
}

PMSG pop_msg(PMSG pmsg)
{
	PMSG ret = NULL;
	ENTER_DISP_LEVEL();
		if (QUEBUF_LEN(MSG) != 0) {
			_QUEBUF_POP(MSG, ret);
			*pmsg = *ret;
			free_msg(ret);
		}
	LEAVE_DISP_LEVEL();
	return( ret );
}

PHOOK disp_sethook(DISP_EVENT mask, PHOOK phook)
{
	PHOOK prev;
	ENTER_DISP_LEVEL();
		prev = phooks[mask];
		phooks[mask] = phook;
		phook->prev = prev;
		phook->next = NULL;
	LEAVE_DISP_LEVEL();
	return( (PHOOK)prev );
}

PHOOK disp_delhook(DISP_EVENT mask, PHOOK phook)
{
	ENTER_DISP_LEVEL();
		phook->prev->next = phook->next;
		if (!phook->next) phooks[mask] = phook->prev;
		else phook->next->prev = phook->prev;
	LEAVE_DISP_LEVEL();
	return( phook->prev );
}

static int def_eventfn(void) { return( 0 ); }

static int def_hookfn(int evt) { return( evt ); }

void INT_INTFUNC(DISP_INT, auto_psv)()
{
	int i; // Current event
	INT_CLR_FLAG(DISP_INT); // Clear flag

	for (i = 0; i < DISP_LAST; ++i)
	{
		int event = phooks[i]->pfnevent();
		if (phooks[i]->event != event) // Check event
			phooks[i]->event = phooks[i]->pfnhook(event);
	}
}
