/*
*	Extended Data Space definitions
*/
#ifndef _EDS_INCL_
#define _EDS_INCL_
#ifdef __HAS_EDS__

#define _EDS_PTR(type) union {\
	__eds__ typeof(type) *peds;\
	struct { typeof(type) *addr; int page; } p; }

typedef _EDS_PTR(char)	PECHAR, *PPECHAR;
typedef _EDS_PTR(long)	PELONG, *PPELONG;
typedef _EDS_PTR(short)	PESHORT, *PPESHORT;
typedef _EDS_PTR(unsigned char)	PEBYTE, *PPEBYTE;
typedef _EDS_PTR(unsigned short)PEWORD, *PPEWORD;
typedef _EDS_PTR(unsigned long)	PEDWORD, *PPEDWORD;
typedef _EDS_PTR(unsigned int)	PEUINT, *PPEUINT;
typedef _EDS_PTR(int)			PEINT, *PPEINT;

#define DSR_PAGE(page)\
	{	int _dsr_to_save = DSRPAG;\
		DSRPAG = page
		// Read data from 'page' in EDS memory
#define DSR_LEAVE() DSRPAG = _dsr_to_save; } ((void)0)

#define DSW_PAGE(page)\
	{	int _dsw_to_save = DSWPAG;\
		DSWPAG = page
		// Write data to 'page' in EDS memory
#define DSW_LEAVE() DSWPAG = _dsw_to_save; } ((void)0)

#define EDS_PAGE(page) DSR_PAGE(page); DSW_PAGE(page)
	// Read-write data from 'page' in EDS memory
#define EDS_LEAVE() DSW_LEAVE(); DSR_LEAVE()

#endif //__HAS_EDS__
#endif //_EDS_INCL_
