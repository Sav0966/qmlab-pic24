/*
*	User interface functions for SPI1-3 (master mode)
*/
#include <p24Fxxxx.h>
#include <config.h>

#ifdef SPI_MASTER  // Only for used SPI

#include <spimui.h>

static unsigned const char *pTX;
static unsigned char *pRX;
static int len = 0;

void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{
	SPI_CLR_FLAG(SPI_MASTER);

	while (SPI_CAN_READ(SPI_MASTER, 1)) {
	 // Can't read more then had been writen
		if (len-- > 0) SPI_WRITE(SPI_MASTER, *pTX++);
		*pRX++ = SPI_READ8(SPI_MASTER);
	}
}

void SPI_ERR_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{
	SPI_CLR_ERFLAG(SPI_MASTER);
	SPI_CLR_OERR(SPI_MASTER);
}

#endif // SPI_MASTER
