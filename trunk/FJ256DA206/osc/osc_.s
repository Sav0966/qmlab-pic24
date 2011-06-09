;
;	MCU Osccillator functions
;
.include "p24Fxxxx.inc" ; PIC24F only

.section	.text, code
.global		__osc_switch	; public

__osc_switch:	; void _osc_switch(FNOSC_VALUE mode) // mode = 0-7
;	swap	w0	; Place the new oscillator selection in W0 low byte

	mov 	#OSCCONH, w1
	mov 	#0x78, w2
	mov 	#0x9A, w3
	disi	#3
	mov.b	w2, [w1]		; OSCCONH (high byte)
	mov.b	w3, [w1]		; unlock Sequence
	mov.b	WREG, OSCCONH	; Set new oscillator selection

	mov 	#OSCCONL, w1
	mov 	#0x46, w2
	mov 	#0x57, w3
	disi	#3
	mov.b	w2, [w1]		; OSCCONL (low byte)
	mov.b	w3, [w1]		; unlock sequence
	bset	OSCCON, #0	; Start oscillator switch operation
	return

.end
