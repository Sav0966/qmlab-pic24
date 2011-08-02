#include "main.h"
#include "timers.h"
#include "mcu_id.h"
#include "reset.h"
#include "refo.h"
#include "osc.h"

/* Reinit all persistent and config data */
#define isPOWER_ON(i) (((i) & EXT_RESET) != 0)

unsigned int mcu_id;
unsigned int rst_state; /* Current reset state */
unsigned int __attribute__((persistent)) rst_events;
unsigned int __attribute__((persistent)) rst_num;
unsigned long nOscFail; // Trap error counter

static void power_on_init(void)
{
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
	osc_mode(PRIPLL); /* Set main oscillator mode */

	/* Select reference clock = FCY/1 and disable it in */
	refo_init(RO_SSLP | RO_SYS | RODIV_NONE); /* sleep */

	TIMER_INIT(1, T_MODE_INT, -1, 1);
	if (TIMER_GET_IPL(1) != 1) while(1);
	if (TIMER_READ(1) != 0) while(1);
	TIMER_ON(1);

TIMER_INIT(2, 0, -1, 5);
TIMER_INIT(3, 0, -1, 5);
TIMER_INIT(4, 0, -1, 5);
TIMER_INIT(5, 0, -1, -1);
TIMER_ON(5); TIMER_OFF(5);

	do { // Main loop



	} while (1); // Main loop

	clr_reset_state(); // Clear uParam and RCON

	for(;;);
	return(0); /* Never return */
}
