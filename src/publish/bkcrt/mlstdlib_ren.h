/**
* @file    mlstdlib_ren.h
* @brief   ...
* @author  zhangrui
* @date    2009-05-24  17:30
*/

#ifndef MLSTDLIB_REN_H
#define MLSTDLIB_REN_H

#ifdef _ATL_MIN_CRT

#ifndef BKCRT_PREFIX
#define BKCRT_PREFIX(x) ml_##x
#endif//BKCRT_PREFIX

//#ifndef _DEBUG
//#define atoi        BKCRT_PREFIX(atoi)
//#endif//_DEBUG

#define srand       BKCRT_PREFIX(srand)
#define rand        BKCRT_PREFIX(rand)

#endif//_ATL_MIN_CRT

#endif//MLSTDLIB_REN_H