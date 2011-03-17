/**
* @file    mlstdio.cpp
* @brief   ...
* @author  zhangrui
* @date    2009-05-26  13:56
*/

#ifdef _ATL_MIN_CRT

#include <assert.h>
#include <stdarg.h>
#include <strsafe.h>
#include "mlstdio.h"

#pragma warning(disable: 4100)

#ifdef __cplusplus
extern "C"
{
#endif

//extern int __cdecl swprintf_s(wchar_t *string, size_t sizeInWords, const wchar_t *format, ...)
//{
//    va_list arglist;
//
//    va_start(arglist, format);
//
//    return StringCchVPrintfW(string, sizeInWords, format, arglist);
//
//    va_end(arglist)
//}

extern int __cdecl _putws(__in_z const wchar_t * str)
{
    return 0;
}

#ifndef _DEBUG

extern int __cdecl _vsnwprintf_s (
    wchar_t *string,
    size_t sizeInWords,
    size_t count,
    const wchar_t *format,
    va_list ap)
{
    if (string && sizeInWords)
        string[0] = L'\0';

    return 0;
}

#endif//_DEBUG

#ifdef __cplusplus
};
#endif

#endif//_ATL_MIN_CRT