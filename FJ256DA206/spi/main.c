#include <p24Fxxxx.h>
#include <libpic30.h>
#include <config.h>
#include <mcu_id.h>
#include <timers.h>
#include <clock.h>
#include <reset.h>
#include <pins.h>
#include <refo.h>
#include <uart.h>
#include <osc.h>
#include <spi.h>

#include "main.h"

/* 20 џэт 1997 15:00:00 */
#define BIOS_START_TIME	853801200L

/* Reinit all persistent and config data */
#define isPOWER_ON(i) (((i) & EXT_RESET) != 0)

unsigned int mcu_id;
unsigned int rst_state; /* Current reset state */
unsigned int __attribute__((persistent)) rst_events;
unsigned int __attribute__((persistent)) rst_num;
unsigned long nOscFail; // Trap error counter

static void power_on_init(void)
{
	/* JTAG must be off to use RB8-11,12,13 */
	if (MCU_CONFIG1  & ~JTAGEN_OFF) while(1);

	pins_init(); // Initialize pins at startup
	clr_reset_state(); // Needed for MCLR
 	rst_events = 0; rst_num = 0;
}

/* Oscillator fail trap handler */
void __attribute__((__interrupt__, auto_psv))
_OscillatorFail(void)
{
	// Now we have FRC oscillator mode
	osc_mode(OSCCON >> 8); // Try to restore NOSC mode
	INTCON1bits.OSCFAIL = 0; // Clear interrupt flag
	++nOscFail;
}

int main(void)
{
	int cfg;

	if (!IS_MCU_PROGRAMMED()) /* Stay in programming */
	__delay32((unsigned long)((1000)*(FCY_UP2)/1000UL));

	/* SIM doesn't clear SRbits.IPL, ICD2 clears it */
	SET_CPU_IPL(MAIN_IPL); /* Set default by hands */

	mcu_id = MCU_ID;
	if (isPOWER_ON(rst_state = get_reset_state()))
		power_on_init(); /*	For POR, BOR and MCLR */

	rst_events |= rst_state; /* As RCON register */
	++rst_num; /* Calculate session reset number */

	cfg = MCU_CONFIG2; /* Check IESO bit in CONFIG2 */
	if (cfg  & ~IESO_OFF) while(1); /* Must be off */
	osc_mode(__OSC__);  /* Select oscillator mode */

	/* Then disable all modules for energy saving */
	PMD1 = PMD2 = PMD3 = PMD4 = PMD5 = PMD6 = -1;

	// Run Timer1 (2 MHz) whith 10 ms period
	if (clock_init(BIOS_START_TIME) < 0) while (1);
	/* Select reference clock = FCY/1 and disable it in */
	refo_init(RO_SSLP | RO_SYS | RODIV_NONE); /* sleep */

	do { // Main loop

		_CS0 = 0;
		__asm__ volatile ("nop\nnop\nnop"); // Breakpoint
		_CS0 = 1;

		__asm__ volatile ("pwrsav	#1"); // Idle mode
	} while (1); // Main loop

	clock_done(); // Disable T1Interrupt & Timer1
	clr_reset_state(); // Clear uParam and RCON

	for(;;);
	return(0); /* Never return */
}
