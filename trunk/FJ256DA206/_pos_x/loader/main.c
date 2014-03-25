#include <p24Fxxxx.h>
#include <libpic30.h>
#include <config.h>
// #include <mcu_id.h>
// #include <reset.h>
// #include <pins.h>

#include "main.h"

/* Condition of initialization of persistent data */
#define isPOWER_ON(state) (((state) & EXT_RESET) != 0)

// unsigned int mcu_id;
// unsigned int rst_state; /* Current reset state */
// unsigned int __attribute__((persistent)) rst_events;
// unsigned int __attribute__((persistent)) rst_num;
// unsigned long nOscFail; // Trap error counter

// static void power_on_init(void)
// {
//	clr_reset_state(); // Needed for MCLR
//	rst_events = 0; rst_num = 0;
// }

int main(void)
{
//	if (!IS_MCU_PROGRAMMED()) /* Stay in programming */
//	__delay32((unsigned long)((1000)*(FCY_UP2)/1000UL));

	/* SIM doesn't clear SRbits.IPL, ICD2 clears it */
//	SET_CPU_IPL(MAIN_IPL); /* Set default by hands */

//	mcu_id = MCU_ID;
//	if (isPOWER_ON(rst_state = get_reset_state()))
//		power_on_init(); /*	For POR, BOR and MCLR */

//	rst_events |= rst_state; /* As RCON register */
//	++rst_num; /* Calculate session reset number */

//	if (mcu_id != 0) while(1); // For simulator only !!!
//	pins_init(); /* Initialize MCU pins configuration */

	for(;;);
	return(0); /* Never return */
}
