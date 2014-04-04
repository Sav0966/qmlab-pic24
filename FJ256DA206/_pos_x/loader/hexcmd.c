#include <p24Fxxxx.h>
#include <ctype.h>
#include "main.h"

void hex_init(PUARTBUF buf)
{ buf->prog.p.state = PROG_INVALID; }

static int hex2dec(int hex)
{
	if (isxdigit(hex)) {
		hex = toupper(hex) - '0';
		if (hex > 9) hex -= 7;
	} else hex = -1;

	return(hex);
}

static int mk_word(unsigned char* str)
{
	union {
		int ret;
		struct { char lo; char hi; } ch;
	} u;

	u.ch.hi = *str++; u.ch.lo = *str;
	return( u.ret );
}

void hex_command(PUARTBUF buf)
{ // Upload and program hex-files
	int n, sum, dec, bin;

	for (;;) {
		if (buf->prog.p.state == PROG_BUSY)
		{ // Wait the end of programming
			__asm__ volatile( /* _flash_helpers doesn't wait */
			"btsc	NVMCON, #15	\n" /* We wait for it to be */
			"bra	$-2			"); /* completed (WR == 0) */
		}

		hex_init(buf); // Reset state (invalid)

		// ':00000001FF' - min of string length (odd)
		if ((buf->pos < 11) || !(buf->pos & 1)) break;

		for (sum = 0, n = 1; n < buf->pos; n++)
		{ // Move data from RxD buffer to PROG structure
			if ((dec = hex2dec(buf->rxd[n])) < 0) break;

			if (n & 1) bin = dec << 4;
			else { // Byte is read
				bin += dec; sum += bin;
				buf->prog.b[n/2] = bin;
			}
		}

		// Check control sum
		if ((n != buf->pos) || (sum & 0xFF)) break; // and length
		if (n != 2 * buf->prog.p.cb + sizeof("00000001FF")) break;

		// Swap prog.p.addr (prog.b[2] & prog.b[3])
		buf->prog.p.addr = mk_word(&buf->prog.b[2]);

		// Switch command
		if (buf->prog.p.type == 1)
		{ // the end of the file

		} else if (buf->prog.p.type == 4)
		{ // extended address
			if (buf->prog.p.cb != 2) break;
			if (buf->prog.p.addr != 0) break;
			buf->xaddr = mk_word(&buf->prog.b[5]);
		} else if (buf->prog.p.type != 0) break;

		buf->prog.p.state = PROG_VALID;
		return; // OK
	} // for(;;)

	++buf->err; // Any error
}
