#include <p24Fxxxx.h>
#include <libpic30.h>
#include <config.h>
#include <mcu_id.h>
#include <timers.h>
#include <clock.h>
#include <reset.h>
#include <pins.h>
#include <refo.h>
#include <osc.h>

#include <uart.h>
#include "main.h"

#if (!defined(__DEBUG) || defined(__MPLAB_DEBUGGER_ICD2))
 #define UART_CHECKED	1 // Checked URAT module (hardware)
#else /* For MPLAB SIM and other tools return 0 */
 #define UART_CHECKED	2
#endif

#define U2_INVALID		1 // It's undefined at this time
#define U2_SHDN			U1_SHDN // Map it to UART1 pin
#define U3_INVALID		1
#define U3_SHDN			U1_SHDN
#define U4_INVALID		1
#define U4_SHDN			U1_SHDN

UART_RX_INTFUNC(UART_CHECKED) { UART_CLR_RXFLAG(UART_CHECKED); }
UART_TX_INTFUNC(UART_CHECKED) { UART_CLR_TXFLAG(UART_CHECKED); }
UART_ER_INTFUNC(UART_CHECKED) { UART_CLR_ERFLAG(UART_CHECKED); }

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
	PMD1=-1; PMD2=-1; PMD3=-1; PMD4=-1; PMD5=-1; PMD6=-1;

	// Run Timer1 (2 MHz) whith 10 ms period
	if (clock_init(BIOS_START_TIME) < 0) while (1);
	/* Select reference clock = FCY/1 and disable it in */
	refo_init(RO_SSLP | RO_SYS | RODIV_NONE); /* sleep */

	do { // Main loop

		if (!(sys_clock() & 0x3F))
		{ // Once per 0.64 seccond check UART
			if (!UART_IS_INIT(UART_CHECKED))
				UART_INIT(UART_CHECKED,	// Try to initialize UART
				 U_LPBACK | U_EN,		// 8-bit, no parity; Enabled
				 U_TXEN, FCY2BRG(FCY2, 9600), // TX Enabled; 9600 baud
				 -1, -1, -1); // no interrupts

			if (!UART_IS_INIT(UART_CHECKED)) continue;

			UART_SEND_BREAK(UART_CHECKED);
			UART_WRITE(UART_CHECKED, -1); // Write dummy character
			UART_WRITE(UART_CHECKED, 0x55); // Write Synch character

			
			UART_WRITE(UART_CHECKED, 0xFF); // Write separator
			while (UART_CAN_WRITE(UART_CHECKED))
				UART_WRITE(UART_CHECKED, 0x55);

		}

		__asm__ volatile ("pwrsav	#1"); // Idle mode, Ipp:
		// 3mA @FRC, 2.7mA @FRCDIV, 2.6mA @FRC16, 4mA @FRCPLL
		// 2.2mA @SOSC, 5.2mA @PRI, 8.8mA @PRIPLL (Timer1 only)
	} while (1); // Main loop

	clock_done(); // Disable T1Interrupt & Timer1
	clr_reset_state(); // Clear uParam and RCON

	for(;;);
	return(0); /* Never return */
}
