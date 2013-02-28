/*
*	MPLAB DEBUGGER tools
*/
#ifndef __TOOLS_INCL_
#define __TOOLS_INCL_

#ifdef __DEBUG

#if (\
	defined(__MPLAB_DEBUGGER_PK3)		\
 || defined(__MPLAB_DEBUGGER_ICD2)		\
 || defined(__MPLAB_DEBUGGER_ICD3)		\
 || defined(__MPLAB_DEBUGGER_REAL_ICE)	\
/* || defined(__ADD_YOUR_TOOL_HERE__) */)
#define __MPLAB_DEBUGGER_TOOL
#else
#define __MPLAB_SIM
#endif

#endif //__DEBUG

#ifdef __DEBUG
#include <stdio.h>
#include <stdlib.h>

#define VERIFY(f)		ASSERT(f)
#define DEBUG_ONLY(f)	(f)

#ifndef	__mkstr__
#define	__mkstr__(exp)	#exp
#endif

#define AssertFiledLine(f)	(fprintf(stderr,			\
	"Assertion failed in file %s in line %i: %s\n",		\
	__FILE__, __LINE__, __mkstr__(f)), 0)

#ifndef __MPLAB_SIM // Stop at the failed line and no messages
#define ASSERT(f)	if (!(f)) while(1) asm __volatile__("nop");
#define TRACE1(sz, p1)
#define TRACE(sz)
#endif

#else //__DEBUG

#define VERIFY(f)		((void)(f))
#define DEBUG_ONLY(f)	((void)0)
#define TRACE1(sz, p1)
#define TRACE(sz)
#define ASSERT(f)

#endif //__DEBUG

// You must enable UART1 IO in SIM Simulator Settings Dialog
// Check 'Enable UART1 IO', 'Rewind input', 'Output 'Window'
#ifndef TRACE
#define TRACE(sz)		DEBUG_ONLY(fprintf(stderr, "%s", sz))
#endif
#ifndef TRACE1
#define TRACE1(sz, p1)	DEBUG_ONLY(fprintf(stderr, sz, p1))
#endif
#ifndef ASSERT
#define	ASSERT(f) DEBUG_ONLY(\
	(void)((f) || AssertFiledLine(f) || (abort(), 0)))
#endif

#endif /*__TOOLS_INCL_*/
