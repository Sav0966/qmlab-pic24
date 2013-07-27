/*
*	User interface functions for SPI1-3 (master mode)
*/
#include <p24Fxxxx.h>
#include <config.h>

#ifdef SPI_MASTER  // Only for used SPI

#include <spimui.h>

volatile unsigned const char *pTX;
volatile unsigned char *pRX;
volatile int len = 0;
volatile int err = 0;

void SPI_ERR_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{
	__asm__ volatile ("nop\nnop"); // breakpoint
	SPI_CLR_ERFLAG(SPI_MASTER);
	SPI_CLR_OERR(SPI_MASTER);
	++err;
}

void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{ // Called on RXI_ANY event (ready to read)
	if (len) // If can read, then can write
	{ SPI_WRITE(SPI_MASTER, *pTX++); --len; }

	SPI_CLR_FLAG(SPI_MASTER);  // Clear flag and
	*pRX++ = SPI_READ8(SPI_MASTER); // read byte

//	do { // Do until RX FIFO is not empty:
		// If can write - write next byte
//		if (len && SPI_CAN_WRITE(SPI_MASTER))
//		{ SPI_WRITE(SPI_MASTER, *pTX++); --len; }

		// If can read - read next byte
//		if (SPI_CAN_READ(SPI_MASTER, 1)) {
//			SPI_CLR_FLAG(SPI_MASTER);
//			*pRX++ = SPI_READ8(SPI_MASTER); }
//	} while (SPI_IS_FLAG(SPI_MASTER));

//if (SPI_TX_COUNT(SPI_MASTER) == 0)
//	__asm__ volatile ("nop\nnop");
}

int spi_send(unsigned char* buf, int n)
{
	if (len > 0) n = -1; // Can't transmit
	else {
		SPI_DISABLE_INT(SPI_MASTER);
		pTX = pRX = buf; len = n-8;
		if (n > 0) 
//			SPI_SET_FLAG(SPI_MASTER);
			SPI_WRITE(SPI_MASTER, *pTX++);

SPI_WRITE(SPI_MASTER, *pTX++);
SPI_WRITE(SPI_MASTER, *pTX++);
SPI_WRITE(SPI_MASTER, *pTX++);
SPI_WRITE(SPI_MASTER, *pTX++);
SPI_WRITE(SPI_MASTER, *pTX++);
SPI_WRITE(SPI_MASTER, *pTX++);
SPI_WRITE(SPI_MASTER, *pTX++);
//SPI_WRITE(SPI_MASTER, *pTX++);
		SPI_ENABLE_INT(SPI_MASTER);
	}

	return n;
}

#endif // SPI_MASTER
