/************************************************************/
/*   Header for PIC24FJ programm memory library functions   */
/************************************************************/
#ifndef _PM_INCL_
#define _PM_INCL_
#include <p24Fxxxx.h> /* PIC24F only */
#include <libpic30.h> /* _FLASH_x constants */

#if ((_FLASH_PAGE != 512) || (_FLASH_ROW != 64))
 #error -- For 512 instructions of page size only --
 #error -- For 64 instructions of row size only --
#endif

#define PM_ROW_SIZE (3*_FLASH_ROW) /* Size in bytes for */
#define PM_PAGE_SIZE (3*_FLASH_PAGE) /* array definition */
/*
* Read and write program memory functions, buffer
* must be PM_XXXX_SIZE bytes of size.Return NULL
* on error or next address (page, row or pword).
* To recognize type of error (address or writing)
* check state of the WRERR bit in NVMCON register.
*/
int pm_is_clean(_prog_addressT addr, int len);
_prog_addressT pm_erase_page(_prog_addressT addr);
_prog_addressT pm_read_row(_prog_addressT addr, void *buf);
_prog_addressT pm_read_page(_prog_addressT addr, void *buf);
_prog_addressT pm_write_row(_prog_addressT addr, void *buf);
_prog_addressT pm_write_page(_prog_addressT addr, void *buf);
_prog_addressT pm_write_pword(_prog_addressT addr, long data);
_prog_addressT pm_write_word(_prog_addressT addr, int data);
long pm_read_pword(_prog_addressT addr); /* -1 if is clean */
int pm_read_word(_prog_addressT addr); /* 2 bytes only */

/* Address usage helpers */
_prog_addressT _user_addr(_prog_addressT addr); /* &= 0x7FFFFE */
_prog_addressT _page_addr(_prog_addressT addr); /* &= 0x7FFC00 */
_prog_addressT _row_addr(_prog_addressT addr); /* &= 0x7FFF80 */

#define PM_LAST_PAGE _page_addr(__builtin_tbladdress(&_CONFIG1))
#define PM_CLEAN _user_addr(__builtin_tbladdress(&_PROGRAM_END))

#endif /*_PM_INCL_*/
