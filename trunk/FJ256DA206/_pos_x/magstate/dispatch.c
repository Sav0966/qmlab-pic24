#include <p24Fxxxx.h>
#include <config.h>
#include <buffer.h>
#include <clock.h>
#include <ints.h>

#include "sysuart.h"
#include "dispatch.h"

#define MAX_MESSAGE	16

static volatile PMSG QUEBUF(MSG, MAX_MESSAGE);

static int cur_clock __attribute__((near)); // = 0
static int cur_sysrx __attribute__((near)); // = 0
static int cur_systx __attribute__((near)); // = 0

static volatile PFVOID phook[DISP_LAST] __attribute__((near));

static void def_hook(void) {}

void disp_init(void)
{
	int i;
	INT_DISABLE_INT(DISP_INT); // Disable the interrupt
	ENTER_DISP_LEVEL();
	{
		QUEBUF_INIT(MSG); // Reset the queue of messages
		for (i = 0; i < DISP_LAST; ++i) phook[i] = def_hook;
	} // Disp-level
	LEAVE_DISP_LEVEL();
	INT_SET_IPL(DISP_INT, DISP_IPL);   // Set IPL
	INT_CLR_FLAG(DISP_INT); // Clear interrupt flag
	INT_ENABLE_INT(DISP_INT); // Enable interrupt
}

PFVOID disp_sethook(DISP_EVENT evt, PFVOID hook)
{
	PFVOID ret;
	ENTER_DISP_LEVEL();
		ret = phook[evt];
		phook[evt] = hook;
	LEAVE_DISP_LEVEL();
	return( ret );
}

void push_msg(PMSG pmsg)
{
	ENTER_DISP_LEVEL();
		if (QUEBUF_LEN(MSG) != QUEBUF_SIZE(MSG))
		{
			QUEBUF_PUSH(MSG, pmsg);
		}
	LEAVE_DISP_LEVEL();
}

PMSG pop_msg()
{
	PMSG pmsg = NULL;
	ENTER_DISP_LEVEL();
		if (QUEBUF_LEN(MSG) != 0)
			_QUEBUF_POP(MSG, pmsg);
	LEAVE_DISP_LEVEL();
	return( pmsg );
}

void INT_INTFUNC(DISP_INT, auto_psv)()
{
	int evt; // Current event
	INT_CLR_FLAG(DISP_INT); // Clear flag

	evt = sys_clock();
	if (cur_clock != evt) // Check clock
	{ cur_clock = evt; phook[DISP_CLOCK](); }

	if (sysu_error()) // UART-ER
			phook[DISP_SYSUER]();

	evt = sysu_rxcount();
	if (cur_sysrx != evt) // Check UART-RX
	{ cur_sysrx = evt; phook[DISP_SYSURX](); }

	evt = sysu_txevt();
	if (cur_systx != evt) {
		cur_systx = evt; // Check UART-TX
		if (sysu_txcount() == 0) phook[DISP_SYSUTX]();
	}
}
