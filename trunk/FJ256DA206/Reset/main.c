#include "main.h"
#include "reset.h"
#include "pm.h"

_CONFIG2(IESO_OFF); // Reset IESO (Errata sheet DS80505)

/* Reinit all persistent and config data */
#define isPOWER_ON(i) (((i) & EXT_RESET)!=0)

unsigned int rst_state; /* Current reset state */
unsigned int __attribute__((persistent)) rst_events;
unsigned int __attribute__((persistent)) rst_num;

static void power_on_init(void)
{ rst_events = 0; rst_num = 0; }

/* Save RCON states, taken places during programming */
/* It works correctly if you link main.o before reset.o */
void __attribute__((user_init)) save_RCON_in_flash(void)
{
	long addr; int i, uninit = 0;
	long buf[16]; /* Set limit for observing */

	if (MCU_ID == 0) return; /* For hardware only */

	/* We can't do any writes in last page of the program */
	/* because verification will fail during programming */
	/* Select first free page located over the programm */
	addr = _page_addr(PM_CLEAN + 2*_FLASH_PAGE);

	/* Find address of first clean 'pword' */
	for (i = 0; addr < PM_LAST_PAGE; addr += 2)
	{
		buf[i] = pm_read_pword(addr); /* Save for a view */
		if (buf[i] < 0) break; /* Break if find clean cell */
		if (++i == (sizeof(buf)/sizeof(buf[0]))) --i;
	} /* 'addr' points to free cell */

	/* PM is clean first time after erasing part */
	if (i == 0) power_on_init(); /* We init all */
	else if (buf[i-1] == 0)
	{
		/* We stop saving states in flash memory */
		if (buf[0] == 0x000083); /* This is the states */
		if (buf[1] == 0x000080); /* observed during ICD2 */
		if (buf[2] == 0x000083); /* programming of device */
	
		if (buf[3] == 0xFF0083); /* Debugger starts whith */
		if (buf[4] == 0); /* RCON = EXT_RESET, rst_num=-1 */

		addr = 0; /* Set breakpoint here to view buf[] */
		return; /* in debbuger after any device reset */
	}

	pm_write_pword(addr, RCON + /* Save RCON & rst_num */
			(((unsigned long)rst_num) << 16)); addr += 2;
	__asm__ volatile ("push RCON");

	RCON = 0; /* Is saved, clear all bits */
	/* Only buf[1] = 0xC2DF if we set -1 */

	if (!IS_MCU_PROGRAMMED()) // while(1);
	/* Wait 300ms min while program - verify */
		for (i = 0; i < 10; i++) __delay_ms(100);
		/* Then reset if programming continues */

	__asm__ volatile ("pop RCON");
	/* We are here in debuger once it starts */
	pm_write_pword(addr, 0); /* Stop writing */

	/* We did initialization after erasing part, but */
	/* debugger reset persistent data once at startup */
	if (rst_num != 0) { uninit = 1; rst_num = -1; }
}

int main(void)
{
	if (!IS_MCU_PROGRAMMED()) __delay_ms(1000);
	/* Stop while the device is programming */

	if (_isMCLR(rst_state = get_reset_state()))
	{
		/* Do something on MCLR reset ... */

#ifdef __MPLAB_DEBUGGER_ICD2
		/* ICD 2 set MCLR bit after program */
		/* memory and reset device commands */
		clr_reset_state();  /* Reinit all */
#endif
	}

	/*	Initialize data at POR, BOR and MCLR */
	if (isPOWER_ON(rst_state)) power_on_init();

	rst_events |= rst_state; /* As RCON register */
	++rst_num; /* Calculate session reset number */

	if (!_isWDTTO(rst_events))
	{
		RCONbits.SWDTEN = 1; while (1);
		/* Wait watchdog time-out once */
	}

	for(;;);
	return(0); /* Never return */
}
