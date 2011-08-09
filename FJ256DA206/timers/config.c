/*
*	MCU Configuration Words
*/
#include "config.h"
#if (defined(__PIC24FK__)) /* PIC24FJ only */
#error -- For PIC24FJ256DA210 Family only --
#endif

_CONFIG3(CONFIG3_INIT);
_CONFIG2(CONFIG2_INIT);
_CONFIG1(CONFIG1_INIT);
