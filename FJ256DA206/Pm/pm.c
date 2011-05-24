/*
*	Functions to serve programm memory read-write operations
*/
#include "pm.h"

_prog_addressT _user_addr(_prog_addressT addr) /* Even */
{ return(addr & 0x7FFFFE); } /* address in user memory */

_prog_addressT _page_addr(_prog_addressT addr) /* First */
{ return(addr & 0x7FFC00); } /* address of program page */

_prog_addressT _row_addr(_prog_addressT addr) /* First */
{ return(addr & 0x7FFF80); } /* address of program row */

_prog_addressT pm_read_row(_prog_addressT addr, void *buf)
{ return(_memcpy_p2d24(buf, _row_addr(addr), PM_ROW_SIZE)); }

_prog_addressT pm_read_page(_prog_addressT addr, void *buf)
{ return(_memcpy_p2d24(buf, _page_addr(addr), PM_PAGE_SIZE)); }

int pm_read_word(_prog_addressT addr)
{ return((int)pm_read_pword(addr)); }

long pm_read_pword(_prog_addressT addr)
{
	unsigned char buf[4];
	_memcpy_p2d24(buf, _user_addr(addr), 3);
	if ((buf[0]&buf[1]&buf[2]) == 0xFF) buf[3] = 0xFF;
	else buf[3] = 0; /* Return -1 if cells are clean */
	return(*(long*)buf);
}

int pm_is_clean(_prog_addressT addr, int len)
{
	for(;len-- != 0; addr += 2)
	{	/* break if any of cells are dirty */
		if (pm_read_pword(addr) > 0) return(0);
	} return(1);
}

static _prog_addressT write_helper(
_prog_addressT addr, long data, char mode)
{
	addr = _user_addr(addr);
	if (pm_read_pword(addr) > 0) return(0); /* Not clear */

	if (mode) _write_flash_word24(addr, data); /* LWHB */
	else _write_flash_word16(addr, (int)data);  /* LW */
	__asm__ volatile( /* _flash_helpers doesn't wait */
	"btsc	NVMCON, #15	\n" /* We wait for it to be */
	"bra	$-2			"); /* completed (WR == 0) */

	/* Return next address if no error */
	return(NVMCONbits.WRERR? 0: addr + 2);
}

_prog_addressT pm_write_pword(_prog_addressT addr, long data)
{ return write_helper(addr, data, 1); }

_prog_addressT pm_write_word(_prog_addressT addr, int data)
{ return write_helper(addr, (long)data, 0); }

static int is_valid(_prog_addressT page)
{
	_prog_addressT end_page; /* Can't erase or write itself */
	end_page = _page_addr(__builtin_tbladdress(&_PROGRAM_END));
	return((page > end_page)? 1: 0);
}

_prog_addressT pm_erase_page(_prog_addressT addr)
{
	addr = _page_addr(addr);
	if (!is_valid(addr)) return(0); /* Can't erase itself */

	_erase_flash(addr); /* Erase a page of program memory */
	__asm__ volatile( /* _flash_helpers doesn't wait */
	"btsc	NVMCON, #15	\n" /* We wait for it to be */
	"bra	$-2			"); /* completed (WR == 0) */

	/* Check error, return next page address if no */
	return(NVMCONbits.WRERR? 0: addr + 2*_FLASH_PAGE);
}

static void copy_buf(char* lbuf, char* buf, int nzise)
{
	__asm__ volatile (
	"repeat	#2				\n"
	"mov.b	[w1++], [w0++]	\n" /* Copy PWORD from packed */
	"clr.b	[w0++]			\n" /* array to array of long */
	"dec	w2, w2			\n" /* integers nsize times */
	"bra	nz, $-8			\n");
}

_prog_addressT pm_write_row(_prog_addressT addr, void *buf)
{
	long lbuf[_FLASH_ROW];

	addr = _row_addr(addr);
	/* Be shure that the row is erased before */
	if (!pm_is_clean(addr, _FLASH_ROW)) return(0);

	copy_buf((char*)lbuf, (char*)buf, _FLASH_ROW); /* Prepare */
	_write_flash24(addr, lbuf);  /* for library page writing */
	__asm__ volatile( /* _flash_helpers doesn't wait */
	"btsc	NVMCON, #15	\n" /* We wait for it to be */
	"bra	$-2			"); /* completed (WR == 0) */

	/* Check error, return next row address if no */
	return(NVMCONbits.WRERR? 0: addr + 2*_FLASH_ROW);
}

_prog_addressT pm_write_page(_prog_addressT addr, void *buf)
{
	addr = _page_addr(addr); /* First row address */

	do { /* while don't reach next program flash page */
		addr = pm_write_row(addr, buf); /* Write rows */
		if (addr == 0) break; /* If row writing error */
		buf = (char*)buf + 3*_FLASH_ROW; /* buf++ */
	} while (addr != _page_addr(addr)); 

	return(addr); /* = next page or 0 */
}
