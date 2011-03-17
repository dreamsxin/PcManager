/**
* @file    mlatoi_ren.h
* @brief   ...
* @author  zhangrui
* @date    2009-05-25  11:18
*/

#ifndef MLATOI_REN_H
#define MLATOI_REN_H

#ifdef _ATL_MIN_CRT

#ifndef BKCRT_PREFIX
#define BKCRT_PREFIX(x) ml_##x
#endif//BKCRT_PREFIX

//#ifndef _DEBUG
//#define atoi        BKCRT_PREFIX(atoi)
//#endif//_DEBUG

#endif//_ATL_MIN_CRT

#endif//MLATOI_REN_H