/*
*	System clock functions (based on Timer1)
*/
#ifndef _CLOCK_INCL_
#define _CLOCK_INCL_
#include "config.h"

#define BIOS_START_TIME	853801200L /* 20 ��� 1997 15:00:00 */
#define CLK_TIM		10E-3 /* ������� ������ ������� � ��� */
#define CORR_TIM	16 /* ��� � 16 ��� ��������� ������� */

#define CORR_CLK ((unsigned long)(CORR_TIM / CLK_TIM))
#define PR1_PERT(t) ((unsigned long)((t * (FCY2 / 8))))
#define PR1_START ((unsigned int)(PR1_PERT(CLK_TIM))-1)
#define PR1_CORR ((unsigned int)\
(FCY /* *(CORR_TIM/16) */ - CORR_CLK * PR1_PERT(CLK_TIM)))

#endif /*_CLOCK_INCL_*/
