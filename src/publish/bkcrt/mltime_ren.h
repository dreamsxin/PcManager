/**
* @file    mltime_ren.h
* @brief   ...
* @author  zhangrui
* @date    2009-05-24  17:30
*/

#ifndef MLTIME_REN_H
#define MLTIME_REN_H

#ifdef _ATL_MIN_CRT

#ifndef BKCRT_PREFIX
#define BKCRT_PREFIX(x) ml_##x
#endif//BKCRT_PREFIX

#define _localtime64_s  BKCRT_PREFIX(_localtime64_s)
//#define _time64         BKCRT_PREFIX(_time64)

#endif//_ATL_MIN_CRT

#endif//MLTIME_REN_H