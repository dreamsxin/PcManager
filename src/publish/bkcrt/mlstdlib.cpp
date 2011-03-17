/**
* @file    mlstdlib.cpp
* @brief   ...
* @author  zhangrui
* @date    2009-05-23  19:32
*/

#ifdef _ATL_MIN_CRT

#include <assert.h>
#include <shlwapi.h>
#include "mlstdlib.h"

#pragma warning(disable: 4995)  //warning C4995: 'function': name was marked as #pragma deprecated

#ifdef __cplusplus
extern "C"
{
#endif



//#ifndef _DEBUG
//extern int __cdecl atoi(const char* str)
//{
//    return StrToIntA(str);
//}
//#endif//_DEBUG



static unsigned int s_ml_holdrand = 0;



extern void __cdecl srand(unsigned int seed)
{
    s_ml_holdrand = seed;
}

extern int __cdecl rand(void)
{
    s_ml_holdrand = s_ml_holdrand * 214013L + 2531011L;
    return ((s_ml_holdrand >> 16) & 0x7fff);
}


#ifdef __cplusplus
};
#endif

#endif//_ATL_MIN_CRT