#include <p24Fxxxx.h>
#include <config.h>
#include <string.h>

#include "main.h"
#include "sysuart.h"

#define _STR_ "It's a simple string\n"

int sysu_test(void)
{
	int i;
	char buf[sizeof(_STR_)];

	if ((sysu_txsize() - sysu_txcount()) > sizeof(_STR_))
		if (sysu_write(_STR_, sizeof(_STR_)) != sizeof(_STR_))
			return(0);

	if (sysu_rxcount() >= sizeof(_STR_))
	{
		for (i = 0; i < sizeof(_STR_); ++i)
			if ((buf[i] = sysu_getc()) == 0) break;

		if (i != (sizeof(_STR_)-1)) return(-1);
		if (buf[i] != 0) return (-1);

		if (strcmp(buf, _STR_) != 0) return(-1);			
	}

	return(sysu_error()? 0: 1);
}
