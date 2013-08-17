/*
*	Переидомер с внутрицикловой обработкой (POS-1)
*/
#ifndef _PMETER_INCL_
#define _PMETER_INCL_

#include <eds.h>
#include <caps.h>

#define BUF_SIZE	1024

#define DECL_PMETER_UI(n)\
	extern __eds__ int _IC_(n, buf)[BUF_SIZE]\
		__attribute__((page, space(eds), noload));\
	extern volatile PEINT _IC_(n, pbuf) __attribute__((near));\
	extern volatile int *_IC_(n, pend) __attribute__((near));\
	extern volatile int _IC_(n, err) __attribute__((near))

#define INIT_BUF(n, size) /* &buf[0] - &buf[size-2] */\
	_IC_(n, pbuf).peds = _IC_(n, buf);\
	_IC_(n, pend) =_IC_(n, pbuf).p.addr + ((size) - 1)

#define IS_BUFERR(n)	(_IC_(n, pend) == _IC_(n, pbuf).p.addr)
#define IS_OERR(n)		(_IC_(n, err)) /* Overrun FIFO */

#endif //_PMETER_INCL_
