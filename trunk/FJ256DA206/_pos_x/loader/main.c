#include <p24Fxxxx.h>
#include <libpic30.h>
#include <config.h>
#include <reset.h>
#include <uarts.h>
#include "main.h"

#ifndef _U_SHDN
#define __U_SHDN(n)	_U##n##_SHDN
#define _U_SHDN(n)	__U_SHDN(n)
#endif

int main(void)
{
	pins_init(); // Initialize MCU pins first

	while (!IS_MCU_PROGRAMMED()); // Stay here
	// SIM doesn't clear SRbits.IPL, ICD2 clears it
	SET_CPU_IPL(MAIN_IPL); // Set default by hands

	if ((RCON & RCON_STATUS) == 0)
	{ // Another program clears RCON bits
	} else { /* No programms are in flash */ }

	// Initialize UART module
	//
	UART_DISABLE_TXINT(SYS_UART);
	UART_DISABLE_RXINT(SYS_UART);
	UART_DISABLE_ERINT(SYS_UART);
	_UMD(SYS_UART) = 0;		// Power on UART module
	_U_SHDN(SYS_UART) = 1; // Wake-up RS-232 driver
	//
	// Setup mode (UART disabled). Setup control
	// bits, clear FIFO buffers and receiver errors
	UMODE(SYS_UART) = U_NOPARITY; // 8-bit no parity
	//
	USTA(SYS_UART) = U_TXI_READY | U_RXI_ANY; // Setup status
	UBRG(SYS_UART) = FCY2BRGH(FCY2, 9600);  // 9600 baud rate
	//
	// Clear buffers
	//
	UART_CLR_RXFLAG(SYS_UART); UART_CLR_TXFLAG(SYS_UART);
	UART_CLR_ERFLAG(SYS_UART);	// Clear interrupt flags
	//
	UMODEbits(SYS_UART).UARTEN = 1;	// Enable module
	UART_ENABLE_TX(SYS_UART); // Enable Transmitter
	//___



	for(;;) __asm__ volatile ("CLRWDT\n");
	return(0); /* Never return */
}
