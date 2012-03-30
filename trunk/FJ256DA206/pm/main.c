#include "main.h" // Must be first (define FCY)
#include <libpic30.h> //__delay_ms() needs FCY
#include <mcu_id.h>
#include <pm.h>

#define ARSIZE(ar) (sizeof(ar)/sizeof(ar[0]))

_CONFIG2(IESO_OFF); // Reset IESO (Errata sheet DS80505)

int main(void)
{
	int i; long addr;
	char buf[PM_PAGE_SIZE];
	unsigned int mcu_id = MCU_ID,
	__attribute__((unused)) mcu_rev = MCU_REV;

	if (mcu_id != 0) /* For hardware configurations */
	__delay_ms(1000);  /* wait for verifying of PM */

	/* Find address of first clean page in program memory */
	for (addr = 0; addr < PM_LAST_PAGE; addr += 2*_FLASH_PAGE)
		if (pm_is_clean(addr, _FLASH_PAGE)) break; /* OK */

	pm_read_page(0x0, buf); /* Read first page in buf[] */
	if (pm_write_page(addr, buf)) /* Create its copy in PM and */
		for (i = 0; i < ARSIZE(buf); addr += 2)  /* verify it */
		{
			union { char c[4]; long l; } u;
			u.c[0] = buf[i++]; u.c[1] = buf[i++];
			u.c[2] = buf[i++]; u.c[3] = 0;
			if ((pm_read_pword(addr) & 0xFFFFFF) != u.l)
				break; /* Comparison error */
		}

	if (i != ARSIZE(buf)) {
		/* Do something on error ... */ addr = -1; }
	else
	{
		addr -= (2*_FLASH_PAGE); /* Return to begin */
		addr += 0x100; /* Address of the AIVT copy */
		if (pm_is_clean(addr, 2)) /* Must be true */
		if (pm_write_pword(addr, 0x123456)) /* Try */
		if (pm_read_pword(addr) == 0x123456) addr = 0;

		if (addr != 0) { /* SIM can't write byte in PM */
			/* Do something on error ... */ addr = -1; }
	}

	for(;;); /* addr must be 0 */
	return(0); /* Never return */
}
