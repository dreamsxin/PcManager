/**
* @file    bkxdx_macro.h
* @brief   ...
* @author  bbcallen
* @date    2009-11-19 23:45
*/

#ifndef BKXDX_MACRO_H
#define BKXDX_MACRO_H

#include "bkxdx_base.h"

/// ...
#define XDX_CLASS_BEGIN(class__)                                            \
NS_BKXDX_BEGIN                                                              \
template<class T_Cursor>                                                    \
inline void ObjectExchange(T_Cursor& xdx_cursor, class__& xdx_value)        \
{


/// ...
#define XDX_MEMBER_OBJECT(name__, member__)                                 \
    xdx_cursor.RaiseIfError();                                              \
    xdx_cursor.BeginChild(name__);                                          \
    xdx_cursor.AccessObject(xdx_value.member__);                            \
    xdx_cursor.EndChild();                                                  \
    xdx_cursor.RaiseIfError();



/// ...
#define XDX_MEMBER(member__)                                                \
    XDX_MEMBER_OBJECT(L#member__, member__)



/// ...
#define XDX_MEMBER_HELPER(name__, member__, helper__)                       \
    xdx_cursor.RaiseIfError();                                              \
    xdx_cursor.BeginChild(name__);                                          \
    helper__::AccessObject(xdx_cursor, xdx_value.member__);                 \
    xdx_cursor.EndChild();                                                  \
    xdx_cursor.RaiseIfError();



/// ...
#define XDX_MEMBER_DICTIONARY(name__, member__)                             \
    XDX_MEMBER_HELPER(name__, member__, CDictionaryHelper)



/// ...
#define XDX_CLASS_END                                                       \
}                                                                           \
NS_BKXDX_END




#endif//BKXDX_MACRO_H