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
 #define ASSERT(b, msg) if (!b) /* Stop here */\
 do { __asm__ volatile ("nop\n nop\n"); } while(1)
#else
 #define ASSERT(b, msg)
#endif //__DEBUG

#endif /*__TOOLS_INCL_*/
