/*
*	User interface functions for SPI1-3 (master mode)
*/
#include <p24Fxxxx.h>
#include <config.h>

#ifdef SPI_MASTER  // Only for used SPI

#include <spimui.h>
#include <_tools.h>

#ifndef SPI_DUMMY
#define SPI_DUMMY	0
#endif // Dummy byte

volatile unsigned const char *pTX;
volatile unsigned char *pRX;
volatile int len = 0;
volatile int err = 0;

void SPI_ERR_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{
	SPI_CLR_ERFLAG(SPI_MASTER);
	SPI_CLR_OERR(SPI_MASTER);

#ifdef __DEBUG
__asm__ volatile ("nop\nnop"); // breakpoint
#endif

	++err; // Needs to decrease speed
}

void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{ // Called on RXI_6DATA event (6 data is ready)
	SPI_CLR_FLAG(SPI_MASTER);
	if (len > 8) {
		while (SPI_CAN_WRITE(SPI_MASTER) &&
				SPI_CAN_READ(SPI_MASTER, 1)) {
//			if (len) { 	SPI_WRITE(SPI_MASTER, *pTX++); --len; }
//			if (SPI_CAN_READ(SPI_MASTER, 1))
//				*pRX++ = SPI_READ8(SPI_MASTER);
			len = 0;
		}
	} else { len = 0; }

//	SPI_CLR_FLAG(SPI_MASTER);  // Clear flag and
//	*pRX++ = SPI_READ8(SPI_MASTER); // read byte

} // 27,9% free time @ 4MHz SPI

#ifdef _not_compil_
void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{ // Called on RXI_ANY event (ready to read)
	if (len) // If can read, then can write
	{ SPI_WRITE(SPI_MASTER, *pTX++); --len; }

	SPI_CLR_FLAG(SPI_MASTER);  // Clear flag and
	*pRX++ = SPI_READ8(SPI_MASTER); // read byte
} // 27,9% free time @ 4MHz SPI

void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{ // Called on RXI_ANY event (ready to read)
	if (len) // If can read, then can write
	{ SPI_WRITE(SPI_MASTER, *pRX); --len; }

	SPI_CLR_FLAG(SPI_MASTER);  // Clear flag and
	*pRX++ = SPI_READ8(SPI_MASTER); // read byte
} // 32,6% free time @ 4MHz SPI

void SPI_INTFUNC(SPI_MASTER, no_auto_psv)(void)
{ // Called on RXI_ANY event (ready to read)
	if (len) // If can read, then can write
	{ SPI_WRITE(SPI_MASTER, SPI_DUMMY); --len; }

	SPI_CLR_FLAG(SPI_MASTER);  // Clear flag and
	*pRX++ = SPI_READ8(SPI_MASTER); // read byte
} // 40,7% free time @ 4MHz SPI
#endif

int spi_send(unsigned char* buf, int n)
{
	if (len > 0) n = -1; // Can't transmit
	else {
		SPI_DISABLE_INT(SPI_MASTER);
		pTX = pRX = buf; len = (n-8);
		if (n > 0) 
			SPI_SET_FLAG(SPI_MASTER);
			SPI_WRITE(SPI_MASTER, *pTX++);

SPI_WRITE(SPI_MASTER, *pTX++);
//SPI_WRITE(SPI_MASTER, *pTX++);
//SPI_WRITE(SPI_MASTER, *pTX++);
//SPI_WRITE(SPI_MASTER, *pTX++);
//SPI_WRITE(SPI_MASTER, *pTX++);
//SPI_WRITE(SPI_MASTER, *pTX++);
//SPI_WRITE(SPI_MASTER, *pTX++);
//SPI_WRITE(SPI_MASTER, *pTX++);
		SPI_ENABLE_INT(SPI_MASTER);
	}

	return n;
}

#endif // SPI_MASTER
