/**
* @file    bkxdx_config.h
* @brief   ...
* @author  bbcallen
* @date    2009-11-26 18:49
*/

#ifndef BKXDX_CONFIG_H
#define BKXDX_CONFIG_H

#ifdef _ATL_MIN_CRT //--------------------

#ifndef XDX_PRAGMA_USE_CPP_EXCEPTION
#define XDX_PRAGMA_USE_CPP_EXCEPTION    0
#endif//XDX_PRAGMA_USE_CPP_EXCEPTION

#ifndef XDX_PRAGMA_SUPPORT_STL
#define XDX_PRAGMA_SUPPORT_STL          0
#endif//XDX_PRAGMA_SUPPORT_STL

#else//_ATL_MIN_CRT --------------------

#ifndef XDX_PRAGMA_USE_CPP_EXCEPTION
#define XDX_PRAGMA_USE_CPP_EXCEPTION    1
#endif//XDX_PRAGMA_USE_CPP_EXCEPTION

#ifndef XDX_PRAGMA_SUPPORT_STL
#define XDX_PRAGMA_SUPPORT_STL          1
#endif//XDX_PRAGMA_SUPPORT_STL

#endif//_ATL_MIN_CRT --------------------

#endif//BKXDX_CONFIG_H