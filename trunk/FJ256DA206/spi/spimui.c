/*
*	User interface functions for SPI1-3 (master mode)
*/
#include <p24Fxxxx.h>
#include <config.h>

#ifdef SPI_MASTER  // Only for used SPI

#include <spimui.h>

unsigned const char *pTX;
unsigned char *pRX;
volatile int len = 0;
volatile int err = 0;

void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{
	SPI_CLR_FLAG(SPI_MASTER);

	// Internal clock (SCK) only
	while (SPI_CAN_READ(SPI_MASTER, 1)) {
	 // Can't read more then have been writen
		if (len-- > 0) SPI_WRITE(SPI_MASTER, *pTX++);
		*pRX++ = SPI_READ8(SPI_MASTER);
	}

//while (SPI_CAN_WRITE(SPI_MASTER))
 //if (len-- > 0) SPI_WRITE(SPI_MASTER, *pTX++);
}

void SPI_ERR_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{
	__asm__ volatile ("nop\nnop"); // breakpoint
	SPI_CLR_ERFLAG(SPI_MASTER);
	SPI_CLR_OERR(SPI_MASTER);
	++err;
}

int spi_send(unsigned char* buf, int n)
{
	if (len > 0) n = -1; // Can't transmit
	else {
		SPI_DISABLE_INT(SPI_MASTER);
		pTX = pRX = buf; len = n - 4;
//		if (n > 0) SPI_WRITE(SPI_MASTER, *pTX++);
SPI_WRITE(SPI_MASTER, *pTX++);
SPI_WRITE(SPI_MASTER, *pTX++);
SPI_WRITE(SPI_MASTER, *pTX++);
SPI_WRITE(SPI_MASTER, *pTX++);

		SPI_ENABLE_INT(SPI_MASTER);
	}

	return n;
}

#endif // SPI_MASTER
