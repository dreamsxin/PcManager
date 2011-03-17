/**
* @file    mlctype_ren.h
* @brief   ...
* @author  zhangrui
* @date    2009-05-24  17:29
*/

#ifndef MLCTYPE_REN_H
#define MLCTYPE_REN_H

#ifdef _ATL_MIN_CRT

#ifndef BKCRT_PREFIX
#define BKCRT_PREFIX(x) ml_##x
#endif//BKCRT_PREFIX

//#define toupper     BKCRT_PREFIX(toupper)
//#define tolower     BKCRT_PREFIX(tolower)

//#define isalpha     BKCRT_PREFIX(isalpha)
//#define isdigit     BKCRT_PREFIX(isdigit)
//#define isxdigit    BKCRT_PREFIX(isxdigit)
//#define isalnum     BKCRT_PREFIX(isalnum)
//#define isspace     BKCRT_PREFIX(isspace)

#endif//_ATL_MIN_CRT

#endif//MLCTYPE_REN_H