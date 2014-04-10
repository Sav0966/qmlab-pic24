#include <p24Fxxxx.h>
#include "main.h"

void trm_command(PUARTBUF buf)
{ // Commands '#g', '#s' and '#i'
	++buf->err;
}
