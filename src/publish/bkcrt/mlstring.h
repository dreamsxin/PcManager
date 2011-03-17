/**
* @file    mlstring.h
* @brief   ...
* @author  zhangrui
* @date    2008-12-21  14:15
*/

#ifndef MLSTRING_H
#define MLSTRING_H

#ifdef _ATL_MIN_CRT

#include <crtdefs.h>

#pragma warning(disable: 4995)  //warning C4995: 'function': name was marked as #pragma deprecated

#ifdef __cplusplus
extern "C"
{
#endif

//extern char*        __cdecl strcpy(char* dst, const char* src);
//extern wchar_t*     __cdecl wcscpy(wchar_t* dst, const wchar_t* src);
//extern wchar_t*     __cdecl wcsncpy(wchar_t* dst, const wchar_t* src, size_t len);
//extern errno_t      __cdecl wcsncpy_s(wchar_t *strDest, size_t numberOfElements, const wchar_t *strSource, size_t count);

//extern int          __cdecl strcmp(const char* dst, const char* src);
//extern int          __cdecl wcscmp(const wchar_t* dst, const wchar_t* src);
extern int          __cdecl _wcsicmp(const wchar_t* dst, const wchar_t* src);
//extern int          __cdecl strncmp(const char* dst, const char* src, size_t len);
//extern int          __cdecl wcsncmp(const wchar_t* dst, const wchar_t* src, size_t len);

//extern size_t       __cdecl strlen(const char* str);
//extern size_t       __cdecl wcslen(const wchar_t* wcs);
//extern size_t       __cdecl wcsnlen(const wchar_t *str, size_t numberOfElements);

//extern char*        __cdecl strcat(char* dst, const char* src);
//extern wchar_t*     __cdecl wcscat(wchar_t* dst, const wchar_t* src);

//extern const char*      __cdecl strstr(const char* str, const char* substr);
//extern const wchar_t*   __cdecl wcsstr(const wchar_t* str, const wchar_t* substr);


//extern errno_t      __cdecl _strlwr_s(char *str, size_t numberOfElements);
//extern errno_t      __cdecl _wcslwr_s(wchar_t *str, size_t numberOfElements);


#ifdef __cplusplus
};
#endif

#endif//_ATL_MIN_CRT

#endif//MLSTRING_H