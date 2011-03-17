/**
* @file    mlstring_ren.h
* @brief   ...
* @author  zhangrui
* @date    2009-05-24  17:30
*/

#ifndef MLSTRING_REN_H
#define MLSTRING_REN_H

#ifdef _ATL_MIN_CRT

#ifndef BKCRT_PREFIX
#define BKCRT_PREFIX(x) ml_##x
#endif//BKCRT_PREFIX

//#define strcpy      BKCRT_PREFIX(strcpy)
//#define wcscpy      BKCRT_PREFIX(wcscpy)
//#define wcsncpy     BKCRT_PREFIX(wcsncpy)
//#define wcsncpy_s   BKCRT_PREFIX(wcsncpy_s)

//#define strcmp      BKCRT_PREFIX(strcmp)
//#define wcscmp      BKCRT_PREFIX(wcscmp)
#define _wcsicmp    BKCRT_PREFIX(_wcsicmp)
//#define strncmp     BKCRT_PREFIX(strncmp)
//#define wcsncmp     BKCRT_PREFIX(wcsncmp)

//#define strlen      BKCRT_PREFIX(strlen)
//#define wcslen      BKCRT_PREFIX(wcslen)
//#define wcsnlen     BKCRT_PREFIX(wcsnlen)

//#define strcat      BKCRT_PREFIX(strcat)
//#define wcscat      BKCRT_PREFIX(wcscat)

//#define strstr      BKCRT_PREFIX(strstr)
//#define wcsstr      BKCRT_PREFIX(wcsstr)

//#define _strlwr_s   BKCRT_PREFIX(_strlwr_s)
//#define _wcslwr_s   BKCRT_PREFIX(_wcslwr_s)

#endif//_ATL_MIN_CRT

#endif//MLSTRING_REN_H