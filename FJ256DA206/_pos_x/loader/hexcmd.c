#include <p24Fxxxx.h>
#include <ctype.h>
#include "main.h"

static int hex2dec(char* str)
{
if (!isxdigit(*str)) return(-1);
	if ((*str >= '0') && (*str <= '9')) *str -= '0';
	else if ((*str >= 'A') && (*str <= 'F')) *str -= 'A'-10;
	else if ((*str >= 'a') && (*str <= 'f')) *str -= 'a'-10;
	else return(-1);

	return(0);
}

//static char hex2bin(unsigned char* str)
//{ return( ((*str - '0') << 8) + (*(str+1) - '0')); }

void hex_command(void)
{ // Upload and program hex-files
	int n, sum;

	// ":00000001FF" - min length string
	if ((buf.pos < 11) || !(buf.pos & 1))
	{ ++buf.err; return; } // and odd

	sum = 0; // Check control sum
	for (n = 1; n < buf.pos; n += 2)
	{
		if (hex2dec((char*)&buf.rxd[n])) break;
		if (hex2dec((char*)&buf.rxd[n+1])) break;
//		sum += 
	}

	if (n != buf.pos)
	{ ++buf.err; return; } // Error
}
