#include "main.h"
#include "clock.h"
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

int t_clock; /* System clock */
long t_time; /* System tyme */

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

void __attribute__((__interrupt__, /*shadow,*/ no_auto_psv))
_T1Interrupt(void)
{
	TIMER_SET_PR(1, PR1_START);

	if ((++t_clock % 100) == 0)
		if (!(++t_time & 0x000F))
		{
			__asm__ volatile ("mov #%0, W0\n add PR1"
					:/* no outputs */: "i" (PR1_CORR));
			
			if (!REFOCONbits.ROON) // If REFO is not used
				__asm__ volatile (/* Mark pulse on REFO */\
				"	btg LATB, #15 ; Toggle latch REFO\n"\
				"	btg LATB, #15 ; Toggle latch REFO\n");
		}
	
	TIMER_CLR_INTFLAG(1);
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

//	delay_ms(5000); /* 22.3 mA @ PRIPLL, Fcy/2=16 MHz */
	/* Disable timer modules only for energy saving: */
	PMD1bits.T1MD=1; PMD1bits.T2MD=1; PMD1bits.T3MD=1;
	PMD1bits.T4MD=1; PMD1bits.T5MD=1; // 21,9 mA
//	delay_ms(5000); // 0.4 mA saving

	/* Then disable all modules */
	PMD1=-1; PMD2=-1; PMD3=-1; PMD4=-1; PMD5=-1; PMD6=-1;
//	delay_ms(5000); // 17.4 mA @ PRIPLL (4.9 mA saving)

	/* Init Timer1 (Fcy/16 = 2 MHz), 10 ms period */
	TIMER_INIT(1, T_MODE_INT | T_PS_8, PR1_START, 1);
	if (TIMER_GET_IPL(1) == 1) // Must be true
		if (TIMER_READ(1) == 0) // Must be true
			TIMER_ON(1); // Run Timer1 by hands
//	delay_ms(5000); // 17,6 mA (+0.2 mA)

	/* Select reference clock = FCY/1 and disable it in */
	refo_init(RO_SSLP | RO_SYS | RODIV_NONE); /* sleep */
//	delay_ms(5000); // 17,65 mA (+0.05 mA), no output

	do { // Main loop


		__asm__ volatile ("pwrsav	#1"); // Idle mode, Ipp:
		// 3mA @FRC, 2.7mA @FRCDIV, 2.6mA @FRC16, 4mA @FRCPLL
		// 2.2mA @SOSC, 5.2mA @PRI, 8.8mA @PRIPLL (Timer1 only)
	} while (1); // Main loop

	TIMER_DONE(1); // Close Timer1

	clr_reset_state(); // Clear uParam and RCON

	for(;;);
	return(0); /* Never return */
}
