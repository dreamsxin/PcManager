/**
* @file    mltime.h
* @brief   ...
* @author  zhangrui
* @date    2009-05-23  20:07
*/

#ifndef MLTIME_H
#define MLTIME_H

#ifdef _ATL_MIN_CRT

#include <crtdefs.h>
#include "mltime.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern errno_t      __cdecl _localtime64_s(struct tm* ptm, const __time64_t* ptime);
//extern __time64_t   __cdecl _time64(__time64_t *timeptr);

#ifdef __cplusplus
};
#endif

#endif//_ATL_MIN_CRT

#endif//MLTIME_H