/**
* @file    mlstdio.h
* @brief   ...
* @author  zhangrui
* @date    2009-05-26  13:56
*/

#ifndef MLSTDIO_H
#define MLSTDIO_H

#ifdef _ATL_MIN_CRT

#include <crtdefs.h>
#include "mlstdlib.h"

#ifdef __cplusplus
extern "C"
{
#endif

//extern int __cdecl swprintf_s(wchar_t *string, size_t sizeInWords, const wchar_t *format, ...);

extern int __cdecl _putws(__in_z const wchar_t * str);


#ifndef _DEBUG

extern int __cdecl _vsnwprintf_s (
    wchar_t *string,
    size_t sizeInWords,
    size_t count,
    const wchar_t *format,
    va_list ap);

#endif//_DEBUG


#ifdef __cplusplus
};
#endif

#endif//_ATL_MIN_CRT

#endif//MLSTDIO_H