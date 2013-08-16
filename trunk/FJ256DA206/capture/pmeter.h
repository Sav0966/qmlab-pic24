/*
*	Переидомер с внутрицикловой обработкой (POS-1)
*/
#ifndef _PMETER_INCL_
#define _PMETER_INCL_

#include <eds.h>
#include <caps.h>

extern __eds__ int _IC_(IC_USED, buf)[BUF_SIZE]
	__attribute__((page, space(eds), noload));

extern volatile PEINT _IC_(IC_USED, pbuf) __attribute__((near));
extern volatile int *_IC_(IC_USED, pend) __attribute__((near));
extern int _IC_(IC_USED, err) __attribute__((near)) /* = 0 */;


#endif //_PMETER_INCL_
