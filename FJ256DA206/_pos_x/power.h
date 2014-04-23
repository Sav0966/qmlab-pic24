#ifndef _POWER_INCL_
#define _POWER_INCL_

#include <pinscfg.h>

// Power management
void power_init(void);

#define power_on(fOn) { _PW_SHDN = (fOn) ? 1 : 0; } ((void)0)
#define is_power_on() (_PW_SHDN != 0)

#endif /*_POWER_INCL_*/
