#ifdef __DEBUG
/*
*	MPLAB DEBUGGER tools
*/
#ifndef __TOOLS_INCL_
#define __TOOLS_INCL_

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

#endif /*__TOOLS_INCL_*/
#endif /*__DEBUG */
