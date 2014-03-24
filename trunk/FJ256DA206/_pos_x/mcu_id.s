;
;	 MCU device ID and Revision
;
.include "p24Fxxxx.inc" ; PIC24F only

.equ	DEVID_ADDR, 0xFF0000

.section	.text, code
.global		__mcu_revid		; public
.global		__mcu_long		; public

__mcu_revid:	; unsigned long _mcu_revid(void)
		mov		#tblpage(DEVID_ADDR), w1
		mov		#tbloffset(DEVID_ADDR), w0

__mcu_long:		; unsigned long _mcu_long(_prog_addressT addr)
		push	TBLPAG
		mov		w1, TBLPAG
		mov		w0, w1		; w1 = TblOffset
		tblrdl	[w1++], w0	; w0 = DEVID
		tblrdl	[w1], w1	; w1 = DEVREV
		pop		TBLPAG
		return

.end
