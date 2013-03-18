/*
*	User interface functions for SPI1-3 (slave mode)
*/
#include <p24Fxxxx.h>
#include <config.h>

#ifdef SPI_SLAVE  // Only for used SPI

#include <spisui.h>

void SPI_INTFUNC(SPI_SLAVE, no_auto_psv)(void)
{
	SPI_CLR_FLAG(SPI_SLAVE);
	while (SPI_CAN_READ(SPI_SLAVE, 1))
				SPI_READ8(SPI_SLAVE);
}

void SPI_ERR_INTFUNC(SPI_SLAVE, no_auto_psv)(void)
{
	SPI_CLR_OERR(SPI_SLAVE);
	SPI_CLR_ERFLAG(SPI_SLAVE);
}

#endif // SPI_SLAVE
