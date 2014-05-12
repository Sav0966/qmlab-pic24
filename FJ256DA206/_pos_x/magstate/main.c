#include <p24Fxxxx.h>
#include <config.h>
#include <power.h>
#include <reset.h>
#include <clock.h>
#include <ints.h>
#include <osc.h>

#include "main.h"
#include "sysuart.h"

// 20 џэт 1997 15:00:00
#define INITIAL_TIME	853801200L

/* Reinit all persistent and config data */
#define isPOWER_ON(i) (((i) & EXT_RESET) != 0)

static void power_on_init(void)
{
#ifdef __DEBUG
	// JTAG must be off to use RB8-11,12,13
	if (MCU_CONFIG1 & ~JTAGEN_OFF) while(1);
	// Check IESO bit in CONFIG2 (must be off)
	if (MCU_CONFIG2 & ~IESO_OFF) while(1);
#endif

#ifdef __DEBUG
	pins_init(); // Initialize pins at startup
	clr_reset_state();		// Needed for MCLR
#else
#error "Check loader code in the flash"
#endif
}

static void on_idle(void)
{

	__asm__ volatile ("pwrsav	#1"); // Idle mode
}

int main(void)
{
	while (!IS_MCU_PROGRAMMED()); // Stay here
	// SIM doesn't clear SRbits.IPL, ICD2 clears it
	SET_CPU_IPL(MAIN_IPL); // Set default by hands

	//	For POR, BOR and MCLR
	if (isPOWER_ON(get_reset_state())) power_on_init(); 

	// Then disable all modules for energy saving
	PMD1 = PMD2 = PMD3 = PMD4 = PMD5 = PMD6 = -1;

	osc_mode(__OSC__); // Select oscillator mode
	// Run Timer1 (2 MHz) with 10 ms elapse period
	if (clock_init(INITIAL_TIME) < 0) while (1);
	// For empty loop and SYS_TIMER using only:
	// 12.6V -> 6.3 mA (PRIPLL) or 2.2mA (FRCPLL)

	power_init(); // Init power module
	sysu_init(); // Init system UART

	INT_INIT(DISP_INT, 0, DISP_IPL);

	for(;;) { // Main loop

		on_idle(); // Idle loop
	} // Main loop

	return(0); // Never return
}

#ifndef __PFVOID
#define __PFVOID
typedef void (*PFVOID)(void);
#endif

typedef enum {
DISP_CLOCK, DISP_SYSUER, DISP_SYSURX, DISP_SYSUTX,
DISP_LAST } DISP_EVENT;

#define ENTER_DISP_LEVEL() {\
	__asm__ volatile ("push SR\n");\
	SR = (DISP_IPL << 5)

#define LEAVE_DISP_LEVEL()\
	__asm__ volatile ("pop SR\n"); } ((void)0)

static void def_hook(void) {}

static int cur_clock __attribute__((near)); // = 0
static int cur_sysrx __attribute__((near)); // = 0
static int cur_systx __attribute__((near)); // = 0

static PFVOID phook[DISP_LAST] __attribute__((near)) =
{ def_hook, def_hook, def_hook, def_hook };

PFVOID pf = def_hook;

PFVOID disp_sethook(DISP_EVENT evt, PFVOID hook)
{
	PFVOID ret;
	ENTER_DISP_LEVEL();
		ret = phook[evt];
		phook[evt] = hook;
	LEAVE_DISP_LEVEL();
	return( ret );
}

void INT_INTFUNC(DISP_INT, auto_psv)()
{
	int evt; // Event
	INT_CLR_FLAG(DISP_INT);

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
