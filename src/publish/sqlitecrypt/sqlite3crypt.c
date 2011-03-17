/**
* @file    sqlite3encrypt.c
* @brief   ...
* @author  zhangrui
* @date    2009-02-18  11:49
*/

#ifndef SQLITE_HAS_CODEC
#define SQLITE_HAS_CODEC    1
#endif//SQLITE_HAS_CODEC

#ifdef _ATL_MIN_CRT

#undef  BKCRT_PREFIX
#define BKCRT_PREFIX(x) ml_sqlite_##x
#include "bkcrt\mltime_ren.h"
#include "bkcrt\mltime.cpp"
#include "bkcrt\mlatoi_ren.h"
#include "bkcrt\mlatoi.cpp"

#endif//_ATL_MIN_CRT

#pragma warning(disable: 4018)  // 'expression' : signed/unsigned mismatch
#pragma warning(disable: 4100)  // 'identifier' : unreferenced formal parameter
#pragma warning(disable: 4127)  // conditional expression is constant
#pragma warning(disable: 4210)  // nonstandard extension used : function given file scope
#pragma warning(disable: 4244)  // conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable: 4267)  // conversion from 'size_t' to 'type', possible loss of data
#pragma warning(disable: 4701)  // potentially uninitialized local variable 'iCursor' used
#pragma warning(disable: 4706)  // assignment within conditional expression

#define SQLITE_THREADSAFE 1

#include "sqlite\sqlite3.c"

#include "codec.c"
#include "codecext.c"
#include "pager_secure.c"