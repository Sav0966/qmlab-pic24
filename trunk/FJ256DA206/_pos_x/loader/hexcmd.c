#include <p24Fxxxx.h>
#include <ctype.h>
#include "main.h"

void hex_init(PUARTBUF buf) { buf->prog.p.type = -1; }

static int hex2dec(int hex)
{
	if (isxdigit(hex)) {
		toupper(hex); hex -= '0';
		if (hex > 9) hex -= 7;
	} else hex = -1;

	return(hex);
}

void hex_command(PUARTBUF buf)
{ // Upload and program hex-files
	int n, sum, dec, bin;

	for (;;) {
		// ":00000001FF" - min  string length and odd
		if ((buf->pos < 11) || !(buf->pos & 1)) break;

		for (sum = 0, n = 1; n < buf->pos; n++)
		{ // Check control sum
			if ((dec = hex2dec(buf->rxd[n])) < 0) break;

			if (n & 1) bin = dec << 4;
			else {		// Byte is read
				bin += dec; sum += bin;
				buf->prog.b[n/2] = bin;
			}
		}

		if ((n != buf->pos) || (sum & 0xFF)) break;


		return; // OK
	} // for(;;)

	++buf->err; // Any error
}
