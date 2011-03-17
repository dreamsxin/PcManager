/**
* @file    bkxdx_exchange_c.h
* @brief   ...
* @author  bbcallen
* @date    2009-11-19 23:42
*/

#ifndef BKXDX_EXCHANGE_C_H
#define BKXDX_EXCHANGE_C_H

#include "bkxdx_base.h"
#include "bkxdx_macro.h"

NS_BKXDX_BEGIN


/// ...
#define XDX_CLASS_LEX_EXCHANGE(class__)                                     \
template<class T_Cursor>                                                    \
inline void ObjectExchange(T_Cursor& cursor, class__& value)                \
{                                                                           \
    CLexValue lexValue;                                                     \
    if (cursor.IsWriteMode())                                               \
    {                                                                       \
        lexValue.SetValue(value);                                           \
        cursor.SetLexValue(lexValue.GetLexValue());                         \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        lexValue.SetLexValue(cursor.GetLexValue());                         \
        lexValue.GetValue(value);                                           \
    }                                                                       \
}

XDX_CLASS_LEX_EXCHANGE(signed short)
XDX_CLASS_LEX_EXCHANGE(unsigned short)
XDX_CLASS_LEX_EXCHANGE(signed int)
XDX_CLASS_LEX_EXCHANGE(unsigned int)
XDX_CLASS_LEX_EXCHANGE(signed long)
XDX_CLASS_LEX_EXCHANGE(unsigned long)

XDX_CLASS_LEX_EXCHANGE(signed __int64)
XDX_CLASS_LEX_EXCHANGE(unsigned __int64)

XDX_CLASS_LEX_EXCHANGE(char)
XDX_CLASS_LEX_EXCHANGE(signed char)
XDX_CLASS_LEX_EXCHANGE(unsigned char)

#ifdef _NATIVE_WCHAR_T_DEFINED
XDX_CLASS_LEX_EXCHANGE(wchar_t)
#endif


NS_BKXDX_END

#endif//BKXDX_EXCHANGE_C_H