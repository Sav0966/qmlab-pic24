#include <p24Fxxxx.h>
#include <_tools.h>
#include <config.h>
#include <uartui.h>
#include <clock.h>

#include <stdio.h>
#include <errno.h>

#ifndef ENODEV
#define ENODEV	ENOENT
#endif

#define UART_USED		1	// Checked UART module
#define UART_RXBUF_SIZE	16	// Size of Receiver queue
#define UART_TXBUF_SIZE	16	// Size of Transmitter queue
//#include "uartui.c" Insert code directly at the bottom

DECL_UART_UI(UART_USED); // Declare UART UI

#if (UART_USED == 1)	// UART1 is used for tracing
#undef ASSERT
#undef TRACE
#undef TRACE1
#undef TRACE2
#define ASSERT(f)
#define TRACE(sz)
#define TRACE1(sz, p1)
#define TRACE2(sz, p1, p2)
// Not defined in <pincfg.h>
#define U1_VALID	1 // Is valid
#define U1_SHDN_SET()	((void)0)
#define U1_SHDN_CLR()	((void)0)
#endif

#ifndef ARSIZE
#define ARSIZE(buf) (sizeof(buf)/sizeof(buf[0]))
#endif

#ifdef __MPLAB_SIM // For MPLAB SIM
 #define U_LPBACK		0x0040 // Loopback Mode Select bit
#endif

#define uart_is_valid()	UART_IS_VALID(UART_USED)
#define uart_is_init()	UART_IS_INIT(UART_USED)
#define uart_done()		UART_PWOFF(UART_USED)

static char _buf[64];
static int _ch, _cnt; // Temp vars
static int stage = 0; // Test stage

int uart_init(void)
{
	UART_INIT(UART_USED,	// Try to initialize UART

#if (defined(__MPLAB_SIM) && (UART_USED == 2))
/* =!= It works with UART2 too if set LPBACK here */	U_LPBACK |
#endif // SIM supports UART1 only (SIM: UART1 IO must be enabled)

		U_NOPARITY | U_EN,		// 8-bit, no parity; Enabled

		U_TXI_READY | U_RXI_ANY | // Prevent dummy TX interrupt
		U_TXEN, FCY2BRG(FCY2, 9600), // TX Enabled; 9600 baud
		2, 2 // All interrupts are enabled
	);

// !!! Ошибка - нужно чиcтить буфера в UART_INIT !!!
	if (UART_IS_INIT(UART_USED)) {
		uart_tx_purge(UART_USED); uart_rx_purge(UART_USED);
	} else return(ENODEV); // Error: pin ~INVALID = '0'

	return(0);
}

#ifdef __DEBUG
void _dump_rx_buf(void)
{
	int c, i = 0;
	while ((c = uart_getc(UART_USED)) != EOF) {
		TRACE1("%2.2X ", (unsigned)(_buf[i] = (char)c));
		if (++i == ARSIZE(_buf)) break;
	} TRACE("\n");
}
#endif //__DEBUG
#define DUMP_RXB()	DEBUG_ONLY(_dump_rx_buf())

void uart_test(void)
{ // Called from Main Loop more often than once per 10 ms
#ifdef __MPLAB_SIM // Poll error bits and set ERFLAG
 if (UART_IS_INIT(UART_USED)) // Check OERR and call IFR
  if (UART_IS_RXERR(UART_USED)) UART_SET_ERFLAG(UART_USED);
#endif // SIM doesn't check receiver errors, but set OERR

	if (sys_clock() & 0x3F) return;
	// Once per 0.64 seccond test UART

	if (!uart_is_init()) uart_init(); // Try to init
	if (!uart_is_valid()) { // Power off UART if the
		uart_done(); return; } // pin ~INVALID = '0'

	// UART_IS_INIT() == TRUE

	// When the UTXEN bit is set, the UxTXIF flag bit will also be set,
	// after two cycles, if UTXISEL<1:0> = 00, since the transmit buffer
	// is not yet full (can move transmit data to the UxTXREG register).

	// =!= So, TX interrupt will be called if UART initialization done

#if (defined(__MPLAB_SIM) && (UART_USED > 2)) // UART3-4
/* The next code checks compiller errors only */ return;
#endif // SIM does not support UART3-4

	switch(stage) {
		case 0: TRACE("Transmit BREAK sequence");

			UART_SET_LPBACK(UART_USED); // Loopback mode (TxD->RxD)

			// BREAK sequence:
			while (!UART_IS_TXEND(UART_USED)); // Wait for TX to be Idle
			// The user should wait for the transmitter to be Idle (TRMT = 1)
			// before setting the UTXBRK. The UTXBRK overrides any other
			// transmitter activity. If the user clears the UTXBRK bit prior
			// to sequence completion, unexpected module behavior can result.
			// Sending a Break character does not generate a transmit interrupt
			UART_SET_BREAK(UART_USED); // Set UTXBRK bit to send Break char
			UART_WRITE(UART_USED, 0); // Write dummy character (Send Break)
			UART_WRITE(UART_USED, 0x55); // Write Synch character
			// Don't clear UTXBRK bit manualy

			stage = 1; break; // Next test

		case 1: // BREAK sequence receiving

			if (!UART_IS_TXEND(UART_USED)) break; // While the sequence ends
			if (!UART_IS_RXIDLE(UART_USED)) break; // Wait for RX to be Idle

			// Check number of received bytes and save first character
			_cnt = uart_rx_count(UART_USED); _ch = uart_getc(UART_USED);
			TRACE2(" => receive %i bytes: %.2X ", _cnt, _ch);
			DUMP_RXB(); // Dump receiver buffer

#ifndef __MPLAB_SIM // Hardware test - first char is '0' whith FERR
			ASSERT((_cnt==1)&(_ch==0x55)); // Only 0x55 is received
			ASSERT(uart_er_count(UART_USED) == 1); // Frame error
#endif// MPLAB SIM doesn't send valid BREAK sequence and set FERR

			uart_rx_purge(UART_USED); // Clear RX buffer errors

			__asm__ volatile ("nop\nnop\nnop"); // breakpoint
			++stage; break; // Next test

		case 2: TRACE("Overrun transmitter FIFO");

			for (_ch = 1; UART_CAN_WRITE(UART_USED); _ch++)
								UART_WRITE(UART_USED, _ch);
			UART_WRITE(UART_USED, 0); // Overrun TX FIFO

			ASSERT(_ch == // Five bytes: TSR + FIFO
						((1 + UART_FIFO_SIZE)+1));

			while (!UART_IS_TXEND(UART_USED)); // Wait

			// Check number of received bytes and save first character
			_cnt = uart_rx_count(UART_USED); _ch = uart_getc(UART_USED);
			TRACE2(" => receive %i bytes: %.2X ", _cnt, _ch);
			DUMP_RXB(); // Dump receiver buffer

			// Only five bytes are received
			ASSERT(_cnt == (1 + UART_FIFO_SIZE));
			--_cnt; // First byte was readed into _ch
			while (_cnt != 0) ASSERT(_buf[--_cnt] != 0);
			ASSERT(_ch != 0); // Don't receive zero byte

			__asm__ volatile ("nop\nnop\nnop"); // breakpoint
			++stage; break; // Next test

		case 3: TRACE("Try to overrun transmitter buffer");
			
			for (_ch = 'A'; ; _ch++) { // Fill TSR, FIFO, buffer
					if (uart_putc(_ch, UART_USED) == EOF) break; 
			} // Send string ABCDE...

			ASSERT(_ch == // TSR + FIFO + TX buffer size
				(1 + UART_FIFO_SIZE + UART_TXBUF_SIZE) + 'A');

			while (uart_tx_full(UART_USED)); // Wait TXB is ready 
			uart_putc(0, UART_USED); // Zero terminator of string

			_cnt = 0; // Will be used as index of buffer

			++stage; break; // Next test

		case 4: // Receve transmitted string

			// Read any bytes into buffer and move index
			_cnt += uart_read(UART_USED, &_buf[_cnt], ARSIZE(_buf));
			if (_cnt < (1 + UART_FIFO_SIZE + UART_TXBUF_SIZE) + 1)
				break; // Not all bytes are received, read it later

			ASSERT(_cnt = (1 + UART_FIFO_SIZE + UART_TXBUF_SIZE) + 1);
			ASSERT(_buf[_cnt-1] == 0); // Full string was received
			TRACE1(" => receive string: %s\n", _buf);

			__asm__ volatile ("nop\nnop\nnop"); // breakpoint
			++stage; break; // Next test

		case 5:	++stage; break; // Skip it
		case 6: // RX buffer must be empty

			ASSERT(uart_rx_empty(UART_USED));

			__asm__ volatile ("nop\nnop\nnop"); // breakpoint
			++stage; break; // Next test

		default: ASSERT(! "Invalid stage value");
	}
} 

// #include "uart.c"
// Insert code directly for debugging

#ifdef UART_USED
 #ifdef	RXB
  #undef	RXB
  #undef	TXB
 #endif
 // Define Buffers IDs
 #if (UART_USED == 1)
  #define RXB		U1RXB
  #define TXB		U1TXB
 #elif (UART_USED == 2)
  #define RXB		U2RXB
  #define TXB		U2TXB
 #elif (UART_USED == 3)
  #define RXB		U3RXB
  #define TXB		U3TXB
 #elif (UART_USED == 4)
  #define RXB		U4RXB
  #define TXB		U4TXB
 #else	// UART_USED == 1-4
  #undef UART_USED // Error
 #endif
#else
// No UART is used - no code will be produced
#endif

#ifdef UART_USED // Only for used UART

#include <_tools.h>
#include <buffer.h>
#include <uartui.h>

#ifndef UART_TXBUF_SIZE
// Default size of Transmitter queue
#define UART_TXBUF_SIZE				32
#endif

#ifndef UART_RXBUF_SIZE
// Default size of Receiver queue
#define UART_RXBUF_SIZE				32
#endif

#ifndef EOF
#define EOF -1
#endif

// Receiver and transmitterqueue queues
static char QUEBUF(RXB, UART_RXBUF_SIZE);
static char QUEBUF(TXB, UART_TXBUF_SIZE);

#define _UART_ERR_NUM(n)	uart_##n##_RXERR_num
#define UART_ERR_NUM(n)	_UART_ERR_NUM(n)
static int UART_ERR_NUM(UART_USED) = 0;

IMPL_UBUF_COUNT(UART_USED, TX) { return QUE_BUF_LEN(TXB); }
IMPL_UBUF_COUNT(UART_USED, RX) { return QUE_BUF_LEN(RXB); }
IMPL_UBUF_COUNT(UART_USED, ER) { return UART_ERR_NUM(UART_USED); }

IMPL_UBUF_FULL(UART_USED, TX)  { return QUE_BUF_FULL(TXB); }
IMPL_UBUF_FULL(UART_USED, RX)  { return QUE_BUF_FULL(RXB); }

IMPL_UBUF_PURGE(UART_USED, RX)
{ // Reset RX queue and RX FIFO
	ASSERT(SRbits.IPL == MAIN_IPL); // Access from main thread only
	ASSERT(UART_IS_ENABLE_RXINT(UART_USED)); // UART must be init

	UART_DISABLE_RXINT(UART_USED); // Lock receiver thread and clear all
	while (UART_CAN_READ(UART_USED)) UART_READ9(UART_USED); QUE_BUF_RESET(RXB);
	UART_ERR_NUM(UART_USED) = 0; // Reset receiver error counter (no errors)
	UART_ENABLE_RXINT(UART_USED); // Unlock receiver thread
}

IMPL_UBUF_PURGE(UART_USED, TX)
{ // Reset TX queue and TX FIFO
	ASSERT(SRbits.IPL == MAIN_IPL); // Access from main thread only
	ASSERT(UART_IS_ENABLE_TXINT(UART_USED)); // UART must be init
	ASSERT(UART_IS_ENABLE_TX(UART_USED));   // UART must be init

	UART_DISABLE_TXINT(UART_USED); // Lock transmitter thread and clear all
	QUE_BUF_RESET(TXB); UART_DISABLE_TX(UART_USED); UART_ENABLE_TX(UART_USED);
	UART_ENABLE_TXINT(UART_USED); // Unlock receiver thread
}

IMPL_UART_GETC(UART_USED)
{
	int c;
	// Access from main thread only
	ASSERT(SRbits.IPL == MAIN_IPL);

	if (QUE_BUF_EMPTY(RXB)) c = EOF;
	else { c = QUE_BUF_POP(RXB); }

	return(c);
}

IMPL_UART_READ(UART_USED)
{
	int c, n;
	for (n = 0; n != len; n++) {
		if ((c = uart_getc(UART_USED)) == EOF) break;
		*buf++ = (unsigned char)c;
	}

	return(n);
}

IMPL_UART_PUTC(UART_USED)
{
	// Access from main thread only
	ASSERT(SRbits.IPL == MAIN_IPL);

	if (QUE_BUF_FULL(TXB)) c = EOF;
	else {
		QUE_BUF_PUSH(TXB, (char)c);
		UART_SET_TXFLAG(UART_USED);
	}

	return(c);
}

IMPL_UART_WRITE(UART_USED)
{
	int n;
	for (n = 0; n != len; n++) {
		if (uart_putc(*buf++, UART_USED) == EOF) break;
	}

	return(n);
}

// Transmitter Interrupt Service Routine
void UART_INTFUNC(UART_USED, TX)(void)
{
	int i;
	// Clear Interrupt flag
	UART_CLR_TXFLAG(UART_USED);

	switch (QUE_BUF_LEN(TXB)) {
		case 0: i = U_TXI_END; break;
		case 1: i = U_TXI_READY; break;
		default: i = U_TXI_EMPTY; // We'll fill FIFO
	}

	UART_SET_TXI(UART_USED, i);

	while (!QUE_BUF_EMPTY(TXB)) {
	 // Load TX queue and fill TX FIFO
		if (UART_CAN_WRITE(UART_USED)) {
			i = QUE_BUF_IPOP(TXB);
			UART_WRITE(UART_USED, i);
		} else break; // FIFO is full
	}

#ifdef __MPLAB_SIM // Poll error bits and set ERFLAG
 if (UART_IS_RXERR(UART_USED)) UART_SET_ERFLAG(UART_USED);
#endif // SIM doesn't check receiver errors, but set OERR
}

// Receiver Interrupt Service Routine
void UART_INTFUNC(UART_USED, RX)(void)
{
	// Clear Interrupt flag
	UART_CLR_RXFLAG(UART_USED);

	if (UART_IS_RXERR(UART_USED)) { // It's not my job
				UART_SET_ERFLAG(UART_USED); return; }

	// No errors at the top of FIFO

	while (UART_CAN_READ(UART_USED)) {
	 // Read all bytes from FIFO
		if (!QUE_BUF_FULL(RXB)) {
			// and write readed bytes into RX buffer
			UART_DISABLE_ERINT(UART_USED); // Lock
			QUE_BUF_IPUSH(RXB, UART_READ8(UART_USED));
			UART_ENABLE_ERINT(UART_USED); // Unlock
		} else {
			// Receiver queue is full

			// Ignore received character
			// and leave it into RX FIFO

			break; // Calculate  errors in OERR handler
		}
	}
}

// Error Interrupt Service Routine
void UART_INTFUNC(UART_USED, Err)(void)
{
	// Clear Interrupt flag
	UART_CLR_ERFLAG(UART_USED);

	while (UART_IS_RXERR(UART_USED)) {
		if (UART_IS_OERR(UART_USED)) {
			// Rx FIFO Buffer overrun error:
			while (!QUE_BUF_FULL(RXB)) { // Try to store
				if (UART_CAN_READ(UART_USED)) { // RX FIFO
					UART_DISABLE_RXINT(UART_USED); // Lock
					QUE_BUF_PUSH(RXB, UART_READ8(UART_USED));
					UART_ENABLE_RXINT(UART_USED); // Unlock
				} else break;
			}

			// Clear FIFO and OERR
			UART_CLR_OERR(UART_USED);

			// No errors at this point (FIFO is empty)

			// Calculate errors
			++UART_ERR_NUM(UART_USED);
		} else {
			if (UART_IS_FERR(UART_USED)) {
				// Frame error at the top of FIFO:
				// Read Data and check Break codition
				if (0 == UART_READ9(UART_USED)) {
					// Break character is received:
					__asm__("nop"); // TODO: AutoBaud
					__asm__("nop"); // Mode can be started
					__asm__("nop"); // in this section
	 			}	

				// Calculate errors
				++UART_ERR_NUM(UART_USED);
			}

			if (UART_IS_PERR(UART_USED)) {
				// Parity error at the top of FIFO:
				// Read the byte from the top of FIFO
				UART_READ9(UART_USED);  // Dummy read

				// Calculate errors
				++UART_ERR_NUM(UART_USED);
			}
		}
	} //  while (UART_IS_ERR(UART_USED))

	if (UART_CAN_READ(UART_USED)) // There is any bytes to read
		 UART_SET_RXFLAG(UART_USED); // Set interrupt flag only
}

#endif // UART_USED

// The same for UART3-4

#undef UART_USED
#define UART_USED	3
#undef UART_RXBUF_SIZE
#undef UART_TXBUF_SIZE
#define UART_RXBUF_SIZE	8
#define UART_TXBUF_SIZE	8	// Use new sizes
#include "uartui.c"

#undef UART_USED
#define UART_USED	4
#undef UART_RXBUF_SIZE
#undef UART_TXBUF_SIZE	// Use default sizes
#include "uartui.c"
