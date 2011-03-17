/**
* @file    bkxdxbase.h
* @brief   ...
* @author  bbcallen
* @date    2009-11-19 23:29
*/

#ifndef BKXDXBASE_H
#define BKXDXBASE_H

#include <assert.h>

#ifndef NS_BKXDX_BEGIN
#define NS_BKXDX_BEGIN    namespace BkXDX {     ///< declare namespace BkXdx
#define NS_BKXDX_END      };                    ///< declare namespace BkXdx
#endif//NS_BKXDX_BEGIN

#include "bkxdx_config.h"
#include "bkxdx_exception.h"

template<class T_Value>
inline void NoDefaultExchangeImplementFound(T_Value&);

template<class T_Value>
inline void NotSupportedByCLexValue(T_Value&);

#endif//BKXDXBASE_H