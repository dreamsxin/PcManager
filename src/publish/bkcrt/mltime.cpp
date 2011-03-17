/**
* @file    mltime.cpp
* @brief   ...
* @author  zhangrui
* @date    2009-05-23  20:08
*/

#ifdef _ATL_MIN_CRT

#include <assert.h>
#include <errno.h>
#include <time.h>
#include <shlwapi.h>
#include "mltime.h"

#ifdef __cplusplus
extern "C"
{
#endif

//////////////////////////////////////////////////////////////////////////
// from ctime.h
#define _MAX__TIME64_T     0x793406fffi64       /* number of seconds from
                                                   00:00:00, 01/01/1970 UTC to
                                                   23:59:59. 12/31/3000 UTC */



//////////////////////////////////////////////////////////////////////////
// from loctime64.c
static BOOL ml__isleapyear(int year)
{
    /* 
    // leap year:
    // is divisable by 4 and not by 100
    // is divisable by 400 
    */
    if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        return TRUE; /* leap */
    else
        return FALSE; /* no leap */
}

extern errno_t __cdecl _localtime64_s(struct tm* ptm, const __time64_t* ptime)
{
    FILETIME    srcTime;
    FILETIME    localTm;
    SYSTEMTIME  sysTm;
    ULONGLONG   t64 = (*ptime + 11644473600) * 10000000;
    // day per month
    static char dayofmon[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31};
    int i    = 0;
    int year = 0;
    int yday = 0;
    int mon  = 0;


    if (NULL == ptm)
        return EINVAL;

    if (NULL == ptime)
        return EINVAL;

    memset(ptm, 0xff, sizeof( struct tm ) );

    /*
     * Check for illegal __time64_t value
     */
    if (*ptime < 0)
        return EINVAL;

    srcTime.dwLowDateTime   = (DWORD)(t64 & 0xFFFFFFFF);
    srcTime.dwHighDateTime  = (DWORD)(t64 >> 32);
    FileTimeToLocalFileTime(&srcTime, &localTm);
    FileTimeToSystemTime(&localTm, &sysTm);
    ptm->tm_year    = sysTm.wYear - 1900;
    ptm->tm_mon     = sysTm.wMonth - 1;
    ptm->tm_wday    = sysTm.wDayOfWeek;
    ptm->tm_mday    = sysTm.wDay;
    ptm->tm_hour    = sysTm.wHour;
    ptm->tm_min     = sysTm.wMinute;
    ptm->tm_sec     = sysTm.wSecond;
    ptm->tm_isdst   = -1;   // unknown;




    // sum day of every month
    yday = 0;
    mon  = ptm->tm_mon; // tm_mon is (0 - 11)
    for (i = 0; i < mon; ++i)
    {
        yday += dayofmon[i];
    }

    // sum current month
    yday += ptm->tm_mday;
    yday -= 1;      // tm_mday is (1 - 31)


    // think about leap year
    year = ptm->tm_year + 1900;
    if (ml__isleapyear(year))
    {
        yday += 1;
    }

    ptm->tm_yday = yday;
    return 0;
}


//////////////////////////////////////////////////////////////////////////
// from time64.c
//
///*
//* Number of 100 nanosecond units from 1/1/1601 to 1/1/1970
//*/
//#define EPOCH_BIAS  116444736000000000i64
//
///*
//* Union to facilitate converting from FILETIME to unsigned __int64
//*/
//typedef union {
//    unsigned __int64 ft_scalar;
//    FILETIME ft_struct;
//} FT;
//
//extern __time64_t __cdecl _time64(__time64_t *timeptr)
//{
//    __time64_t tim;
//    FT nt_time;
//
//    GetSystemTimeAsFileTime( &(nt_time.ft_struct) );
//
//    tim = (__time64_t)((nt_time.ft_scalar - EPOCH_BIAS) / 10000000i64);
//
//
//    if (timeptr)
//        *timeptr = tim;         /* store time if requested */
//
//    return tim;
//}


#ifdef __cplusplus
};
#endif

#endif//_ATL_MIN_CRT