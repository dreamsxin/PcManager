#ifndef _STRUCT_H_
#define _STRUCT_H_
//-----------------------------------------------------------------------------//
#include <windows.h>
//-----------------------------------------------------------------------------//
#define UD_MAX_PATH_COUNT	32

typedef struct _UDHEAD 
{
	ULONG Count;
	PWCHAR P[UD_MAX_PATH_COUNT];
} UDHEAD, *PUDHEAD;
//-----------------------------------------------------------------------------//
#endif