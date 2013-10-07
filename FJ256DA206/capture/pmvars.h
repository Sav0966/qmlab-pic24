/*
*	Period meter names for the methematics
*/
#ifndef _PMVARS_INCL_
#define _PMVARS_INCL_

#include <caps.h>

// Short names for the methematics
#define _pT1	_IC_(IC_USED, pT1)
#define _pT2	_IC_(IC_USED, pT2)
#define _pT3	_IC_(IC_USED, pT3)
#define _dT0	_IC_(IC_USED, dT1)
#define _dT1	_IC_(IC_USED, dT1)
#define _dT2	_IC_(IC_USED, dT2)
#define _dT3	_IC_(IC_USED, dT3)
#define _S1		_IC_(IC_USED, S1)
#define _S2		_IC_(IC_USED, S2)
#define _S3		_IC_(IC_USED, S3)
#define _N1		_IC_(IC_USED, N1)
#define _N2		_IC_(IC_USED, N2)
#define _N3		_IC_(IC_USED, N3)
#define _ave	_IC_(IC_USED, ave)
#define _cmp	_IC_(IC_USED, cmp)
#define _pQ0	_IC_(IC_USED, pQ0)
#define _pQ1	_IC_(IC_USED, pQ1)
#define _pQ2	_IC_(IC_USED, pQ2)
#define _pQ3	_IC_(IC_USED, pQ3)
#define _T02	_IC_(IC_USED, T02)
#define _T13	_IC_(IC_USED, T13)
#define _Tave	_IC_(IC_USED, Tave)
#define _Sqmc	_IC_(IC_USED, Sqmc)

#if (IC_USED == 1)
__asm__("	.equiv	_N1, __ic_1_N1		\n"
		"	.equiv	_N2, __ic_1_N2		\n"
		"	.equiv	_N3, __ic_1_N3		\n"
		"	.equiv	_S1, __ic_1_S1		\n"
		"	.equiv	_S2, __ic_1_S2		\n"
		"	.equiv	_S3, __ic_1_S3		\n"
		"	.equiv	_pT1, __ic_1_pT1	\n"
		"	.equiv	_pT2, __ic_1_pT2	\n"
		"	.equiv	_pT3, __ic_1_pT3	\n"
		"	.equiv	_dT1, __ic_1_dT1	\n"
		"	.equiv	_dT2, __ic_1_dT2	\n"
		"	.equiv	_dT3, __ic_1_dT3	\n");
//		"	.equiv	_ave, __ic_1_ave	\n"
//		"	.equiv	_Tave, __ic_1_Tave	\n"
//		"	.equiv	_ave, __ic_1_cmp	\n");
#elif (IC_USED == 2)
__asm__("	.equiv	_N1, __ic_2_N1		\n"
		"	.equiv	_N2, __ic_2_N2		\n"
		"	.equiv	_N3, __ic_2_N3		\n"
		"	.equiv	_S1, __ic_2_S1		\n"
		"	.equiv	_S2, __ic_2_S2		\n"
		"	.equiv	_S3, __ic_2_S3		\n"
		"	.equiv	_pT1, __ic_2_pT1	\n"
		"	.equiv	_pT2, __ic_2_pT2	\n"
		"	.equiv	_pT3, __ic_2_pT3	\n"
		"	.equiv	_dT1, __ic_2_dT1	\n"
		"	.equiv	_dT2, __ic_2_dT2	\n"
		"	.equiv	_dT3, __ic_2_dT3	\n");
//		"	.equiv	_ave, __ic_2_ave	\n"
//		"	.equiv	_Tave, __ic_2_Tave	\n"
//		"	.equiv	_ave, __ic_2_cmp	\n");
#elif (IC_USED == 3)
__asm__("	.equiv	_N1, __ic_3_N1		\n"
		"	.equiv	_N2, __ic_3_N2		\n"
		"	.equiv	_N3, __ic_3_N3		\n"
		"	.equiv	_S1, __ic_3_S1		\n"
		"	.equiv	_S2, __ic_3_S2		\n"
		"	.equiv	_S3, __ic_3_S3		\n"
		"	.equiv	_pT1, __ic_3_pT1	\n"
		"	.equiv	_pT2, __ic_3_pT2	\n"
		"	.equiv	_pT3, __ic_3_pT3	\n"
		"	.equiv	_dT1, __ic_3_dT1	\n"
		"	.equiv	_dT2, __ic_3_dT2	\n"
		"	.equiv	_dT3, __ic_3_dT3	\n");
//		"	.equiv	_ave, __ic_3_ave	\n"
//		"	.equiv	_Tave, __ic_3_Tave	\n"
//		"	.equiv	_ave, __ic_3_cmp	\n");
#elif (IC_USED == 4)
__asm__("	.equiv	_N1, __ic_4_N1		\n"
		"	.equiv	_N2, __ic_4_N2		\n"
		"	.equiv	_N3, __ic_4_N3		\n"
		"	.equiv	_S1, __ic_4_S1		\n"
		"	.equiv	_S2, __ic_4_S2		\n"
		"	.equiv	_S3, __ic_4_S3		\n"
		"	.equiv	_pT1, __ic_4_pT1	\n"
		"	.equiv	_pT2, __ic_4_pT2	\n"
		"	.equiv	_pT3, __ic_4_pT3	\n"
		"	.equiv	_dT1, __ic_4_dT1	\n"
		"	.equiv	_dT2, __ic_4_dT2	\n"
		"	.equiv	_dT3, __ic_4_dT3	\n");
//		"	.equiv	_ave, __ic_4_ave	\n"
//		"	.equiv	_Tave, __ic_4_Tave	\n"
//		"	.equiv	_ave, __ic_4_cmp	\n");
#elif (IC_USED == 5)
__asm__("	.equiv	_N1, __ic_5_N1		\n"
		"	.equiv	_N2, __ic_5_N2		\n"
		"	.equiv	_N3, __ic_5_N3		\n"
		"	.equiv	_S1, __ic_5_S1		\n"
		"	.equiv	_S2, __ic_5_S2		\n"
		"	.equiv	_S3, __ic_5_S3		\n"
		"	.equiv	_pT1, __ic_5_pT1	\n"
		"	.equiv	_pT2, __ic_5_pT2	\n"
		"	.equiv	_pT3, __ic_5_pT3	\n"
		"	.equiv	_dT1, __ic_5_dT1	\n"
		"	.equiv	_dT2, __ic_5_dT2	\n"
		"	.equiv	_dT3, __ic_5_dT3	\n");
//		"	.equiv	_ave, __ic_5_ave	\n"
//		"	.equiv	_Tave, __ic_5_Tave	\n"
//		"	.equiv	_ave, __ic_5_cmp	\n");
#elif (IC_USED == 6)
__asm__("	.equiv	_N1, __ic_6_N1		\n"
		"	.equiv	_N2, __ic_6_N2		\n"
		"	.equiv	_N3, __ic_6_N3		\n"
		"	.equiv	_S1, __ic_6_S1		\n"
		"	.equiv	_S2, __ic_6_S2		\n"
		"	.equiv	_S3, __ic_6_S3		\n"
		"	.equiv	_pT1, __ic_6_pT1	\n"
		"	.equiv	_pT2, __ic_6_pT2	\n"
		"	.equiv	_pT3, __ic_6_pT3	\n"
		"	.equiv	_dT1, __ic_6_dT1	\n"
		"	.equiv	_dT2, __ic_6_dT2	\n"
		"	.equiv	_dT3, __ic_6_dT3	\n");
//		"	.equiv	_ave, __ic_6_ave	\n"
//		"	.equiv	_Tave, __ic_6_Tave	\n"
//		"	.equiv	_ave, __ic_6_cmp	\n");
#elif (IC_USED == 7)
__asm__("	.equiv	_N1, __ic_7_N1		\n"
		"	.equiv	_N2, __ic_7_N2		\n"
		"	.equiv	_N3, __ic_7_N3		\n"
		"	.equiv	_S1, __ic_7_S1		\n"
		"	.equiv	_S2, __ic_7_S2		\n"
		"	.equiv	_S3, __ic_7_S3		\n"
		"	.equiv	_pT1, __ic_7_pT1	\n"
		"	.equiv	_pT2, __ic_7_pT2	\n"
		"	.equiv	_pT3, __ic_7_pT3	\n"
		"	.equiv	_dT1, __ic_7_dT1	\n"
		"	.equiv	_dT2, __ic_7_dT2	\n"
		"	.equiv	_dT3, __ic_7_dT3	\n");
//		"	.equiv	_ave, __ic_7_ave	\n"
//		"	.equiv	_Tave, __ic_7_Tave	\n"
//		"	.equiv	_ave, __ic_7_cmp	\n");
#elif (IC_USED == 8)
__asm__("	.equiv	_N1, __ic_8_N1		\n"
		"	.equiv	_N2, __ic_8_N2		\n"
		"	.equiv	_N3, __ic_8_N3		\n"
		"	.equiv	_S1, __ic_8_S1		\n"
		"	.equiv	_S2, __ic_8_S2		\n"
		"	.equiv	_S3, __ic_8_S3		\n"
		"	.equiv	_pT1, __ic_8_pT1	\n"
		"	.equiv	_pT2, __ic_8_pT2	\n"
		"	.equiv	_pT3, __ic_8_pT3	\n"
		"	.equiv	_dT1, __ic_8_dT1	\n"
		"	.equiv	_dT2, __ic_8_dT2	\n"
		"	.equiv	_dT3, __ic_8_dT3	\n");
//		"	.equiv	_ave, __ic_8_ave	\n"
//		"	.equiv	_Tave, __ic_8_Tave	\n"
//		"	.equiv	_ave, __ic_8_cmp	\n");
#elif (IC_USED == 9)
__asm__("	.equiv	_N1, __ic_9_N1		\n"
		"	.equiv	_N2, __ic_9_N2		\n"
		"	.equiv	_N3, __ic_9_N3		\n"
		"	.equiv	_S1, __ic_9_S1		\n"
		"	.equiv	_S2, __ic_9_S2		\n"
		"	.equiv	_S3, __ic_9_S3		\n"
		"	.equiv	_pT1, __ic_9_pT1	\n"
		"	.equiv	_pT2, __ic_9_pT2	\n"
		"	.equiv	_pT3, __ic_9_pT3	\n"
		"	.equiv	_dT1, __ic_9_dT1	\n"
		"	.equiv	_dT2, __ic_9_dT2	\n"
		"	.equiv	_dT3, __ic_9_dT3	\n");
//		"	.equiv	_ave, __ic_9_ave	\n"
//		"	.equiv	_Tave, __ic_9_Tave	\n"
//		"	.equiv	_ave, __ic_9_cmp	\n");
#endif // Registers and vars

#endif //_PMVARS_INCL_
