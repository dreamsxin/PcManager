/**
* @file    bkxdx_exception.h
* @brief   ...
* @author  bbcallen
* @date    2009-11-26 18:49
*/

#ifndef BKXDX_EXCEPTION_H
#define BKXDX_EXCEPTION_H

#include "bkxdx_base.h"
#include "bkxdx_config.h"

//////////////////////////////////////////////////////////////////////////
// use C++ expcetion or SEH
//------------------------------------------------------------------------
#if 0 != XDX_PRAGMA_USE_CPP_EXCEPTION

NS_BKXDX_BEGIN
class CXdxException {};
NS_BKXDX_END

#define XDX_EXCEPTION_WRAP_BEGIN                                    \
    try {

#define XDX_EXCEPTION_WRAP_END                                      \
    }                                                               \
    catch (CXdxException)                                           \
    {                                                               \
    };

#define XDX_RAISE_EXCEPTION()   throw CXdxException()

#else //------------------------------------------------------------------

#define XDX_EXCEPTION_WRAP_BEGIN                                    \
    __try {

#define XDX_EXCEPTION_WRAP_END                                      \
    }                                                               \
    __except (0 == GetExceptionCode() ?                             \
        EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)      \
    {                                                               \
    };

#define XDX_RAISE_EXCEPTION()   RaiseException(0, 0, 0, 0);

#endif
//------------------------------------------------------------------------


#endif//BKXDX_EXCEPTION_H