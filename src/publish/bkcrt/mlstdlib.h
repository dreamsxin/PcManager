/**
* @file    mlstdlib.h
* @brief   ...
* @author  zhangrui
* @date    2009-05-23  19:31
*/

#ifndef MLSTDLIB_H
#define MLSTDLIB_H

#ifdef _ATL_MIN_CRT

#include <crtdefs.h>
#include "mlstdlib.h"

#ifdef __cplusplus
extern "C"
{
#endif

//#ifndef _DEBUG
//extern int      __cdecl atoi(const char* str);
//#endif//_DEBUG

extern void     __cdecl srand(unsigned int seed);
extern int      __cdecl rand(void);

#ifdef __cplusplus
};
#endif

#endif//_ATL_MIN_CRT

#endif//MLSTDLIB_H