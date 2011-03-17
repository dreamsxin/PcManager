/**
* @file    mlstring.cpp
* @brief   ...
* @author  zhangrui
* @date    2008-12-19  17:47
*/


#ifdef _ATL_MIN_CRT

#include <assert.h>
#include <errno.h>
#include <shlwapi.h>
#include <strsafe.h>
#include "mlstring.h"

#pragma comment(lib, "shlwapi.lib")

#ifdef __cplusplus
extern "C"
{
#endif



//extern char* __cdecl strcpy(char* dst, const char* src)
//{
//    return StrCpyA(dst, src);
//}
//
//extern wchar_t* __cdecl wcscpy(wchar_t* dst, const wchar_t* src)
//{
//    return StrCpyW(dst, src);
//}
//
//extern wchar_t* __cdecl wcsncpy(wchar_t* dst, const wchar_t* src, size_t len)
//{
//    assert(dst);
//    assert(src);
//    assert(len <= INT_MAX);
//    return StrCpyNW(dst, src, (int)len);
//}
//
//
//
//extern errno_t __cdecl wcsncpy_s(wchar_t *strDest, size_t numberOfElements, const wchar_t *strSource, size_t count)
//{
//    if (NULL == strDest)
//        return EINVAL;
//
//    if (0 == numberOfElements)
//        return EINVAL;
//
//    if (NULL == strSource)
//    {
//        strDest[0] = L'\0';
//        return EINVAL;
//    }
//
//    int nCopy = (int)min(numberOfElements, count);
//    StrCpyNW(strDest, strSource, nCopy);
//    if (count <= numberOfElements - 1)
//    {
//        strDest[count] = L'\0';
//    }
//    else
//    {
//        strDest[numberOfElements - 1] = L'\0';
//    }
//
//    
//    return 0;
//}
//




//extern int __cdecl strcmp(const char* dst, const char* src)
//{
//    assert(dst);
//    assert(src);
//
//    return StrCmpA(dst, src);
//}
//
//extern int __cdecl wcscmp(const wchar_t* dst, const wchar_t* src)
//{
//    assert(dst);
//    assert(src);
//
//    return StrCmpW(dst, src);
//}

extern int __cdecl _wcsicmp(const wchar_t* dst, const wchar_t* src)
{
    assert(dst);
    assert(src);

    return StrCmpIW(dst, src);
}

//extern int __cdecl strncmp(const char* dst, const char* src, size_t len)
//{
//    assert(dst);
//    assert(src);
//    assert(len <= INT_MAX);
//
//    return StrCmpNA(dst, src, (int)len);
//}
//
//extern int __cdecl wcsncmp(const wchar_t* dst, const wchar_t* src, size_t len)
//{
//    assert(dst);
//    assert(src);
//    assert(len <= INT_MAX);
//
//    return StrCmpNW(dst, src, (int)len);
//}






//extern size_t __cdecl strlen(const char* str)
//{
//    return lstrlenA(str);
//}
//
//extern size_t __cdecl wcslen(const wchar_t* wcs)
//{
//    return lstrlenW(wcs);
//}




//extern size_t __cdecl wcsnlen(const wchar_t *str, size_t numberOfElements)
//{
//    size_t cch = 0;
//    HRESULT hr = StringCchLengthW(str, numberOfElements, &cch);
//    if (FAILED(hr))
//        return (size_t)-1;
//
//    return cch;
//}


//extern char* __cdecl strcat(char* dst, const char* src)
//{
//    return lstrcatA(dst, src);
//}
//
//extern wchar_t* __cdecl wcscat(wchar_t* dst, const wchar_t* src)
//{
//    return lstrcatW(dst, src);
//}



//extern const char* __cdecl strstr(const char* str, const char* substr)
//{
//    return StrStrA(str, substr);
//}
//
//extern const wchar_t* __cdecl wcsstr(const wchar_t* str, const wchar_t* substr)
//{
//    return StrStrW(str, substr);
//}




//extern errno_t __cdecl _strlwr_s(char *str, size_t numberOfElements)
//{
//    if (NULL == str)
//        return EINVAL;
//
//    CharLowerBuffA(str, numberOfElements);
//    return 0;
//}
//
//extern errno_t __cdecl _wcslwr_s(wchar_t *str, size_t numberOfElements)
//{
//    if (NULL == str)
//        return EINVAL;
//
//    CharLowerBuffW(str, numberOfElements);
//    return 0;
//}



#ifdef __cplusplus
};
#endif

#endif//_ATL_MIN_CRT